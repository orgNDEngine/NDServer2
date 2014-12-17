#include "file/NDCLogFile.h"

#include "NDCLibFun.h"
#include "NDShareBaseGlobal.h"
#include "thread/NDMutex.h"
#include "file/NDCFile.h"


_NDSHAREBASE_BEGIN

const NDCLogFile::TRANS NDCLogFile::priorities[9] = {
	{"emerg",   APLOG_EMERG},
	{"alert",   APLOG_ALERT},
	{"crit",    APLOG_CRIT},
	{"error",   APLOG_ERR},
	{"warn",    APLOG_WARNING},
	{"notice",  APLOG_NOTICE},
	{"info",    APLOG_INFO},
	{"debug",   APLOG_DEBUG},
	{NULL,      -1},
};

NDCLogFile::NDCLogFile( const char* pFileName, const char* pOpenFlag, NDUint32 nCacheSize ) : m_nTotalSize(0), m_nCacheSize(nCacheSize), m_nCacheLogPos(0), 
																							  m_szLogCache(new char[nCacheSize]), m_pNDCFile( new NDCFile ), m_pLogMutex( new NDMutexLock )
{
	if (NULL != m_pNDCFile)
	{
		m_pNDCFile->open( pFileName, pOpenFlag );
	}
}

NDCLogFile::~NDCLogFile(void)
{
	close();
}

NDBool NDCLogFile::open() const
{
	if ( NULL == m_pNDCFile ) return NDFalse;
	return m_pNDCFile->isopen();
}

void NDCLogFile::close()
{
	if ( NULL == m_pNDCFile ) return;

	flush();

	m_pNDCFile->flush();
	m_pNDCFile->close();

	if ( NULL != m_szLogCache )
	{
		delete m_szLogCache;
		m_szLogCache = NULL;
	}

	delete m_pNDCFile;
	m_pNDCFile = NULL;

	if ( NULL != m_pLogMutex )
	{
		delete m_pLogMutex;
		m_pLogMutex = NULL;
	}
}

NDUint32 NDCLogFile::size()
{
	if ( NULL == m_pNDCFile ) return 0;
	return m_nTotalSize;	//return m_pNDCFile->size();
}

NDBool NDCLogFile::write( const char* pFile, NDInt32 nLine, NDInt32 nLevel, const char* pFormat, ... )
{
	if ( NULL == m_pNDCFile ) return NDFalse;
	if ( NDFalse == open() )  return NDFalse;

	va_list ap;
	va_start( ap, pFormat );
	string strContent( getWriteContent( pFile, nLine, nLevel, pFormat, ap ) );
	va_end(ap);

	const char* pBuf = strContent.c_str();
	NDUint32 nlen	 = (NDUint32)strContent.length();
	if ( (nlen + m_nCacheLogPos) >= m_nCacheSize )
	{	//写入的数据大于缓存大小;
		m_nTotalSize += nlen;
		//将缓存日志内存数据写入文件;
		flush();
		//add locker;
		NDGuardLock logLocker( *m_pLogMutex );
		return ( nlen == m_pNDCFile->write( pBuf, nlen ) );		
	}
	//add locker;
	NDGuardLock logLocker( *m_pLogMutex );
	memcpy( m_szLogCache + m_nCacheLogPos, pBuf, nlen );
	m_nCacheLogPos	+= nlen;
	m_nTotalSize	+= nlen;
	return NDTrue;
}

string  NDCLogFile::getWriteContent( const char* pFile, NDInt32 nLine, NDInt32 nLevel, const char* pFormat, va_list ap )
{
	char szBuf[MAX_STRING_LEN] = {0};
	NDInt32 nLevelMask	= (nLevel & APLOG_LEVELMASK);

	string strTime(NDShareBaseGlobal::nd_getLocalSecondTimeStr());

	NDUint32 nlen  = ND_SNPRINTF( szBuf, MAX_STRING_LEN - 1, "\r\n[%s] ", strTime.c_str() );
	nlen += ND_SNPRINTF( (szBuf + nlen), (MAX_STRING_LEN - nlen - 1), "[%s] ", priorities[nLevelMask].t_name );
	nlen += ND_SNPRINTF( (szBuf + nlen), (MAX_STRING_LEN - nlen - 1), "[%s:%d]: ", pFile, nLine );
	ND_VSNPRINTF( (szBuf + nlen), (MAX_STRING_LEN - nlen - 1), pFormat, ap );


	return string(szBuf);
}

NDBool NDCLogFile::write( const char* pFile, NDInt32 nLine, NDInt32 nLevel, const char* pFormat, va_list ap )
{
	if ( NULL == m_pNDCFile ) return NDFalse;
	if ( NDFalse == open() )  return NDFalse;

	string strContent( getWriteContent( pFile, nLine, nLevel, pFormat, ap ) );

	const char* pBuf = strContent.c_str();
	NDUint32 nlen	 = (NDUint32)strContent.length();
	if ( (nlen + m_nCacheLogPos) >= m_nCacheSize )
	{	//写入的数据大于缓存大小;
		m_nTotalSize += nlen;
		//将缓存日志内存数据写入文件;
		flush();
		//add locker;
		NDGuardLock logLocker( *m_pLogMutex );
		return ( nlen == m_pNDCFile->write( pBuf, nlen ) );		
	}
	//add locker;
	NDGuardLock logLocker( *m_pLogMutex );
	memcpy( m_szLogCache + m_nCacheLogPos, pBuf, nlen );
	m_nCacheLogPos	+= nlen;
	m_nTotalSize	+= nlen;
	return NDTrue;
}

NDBool NDCLogFile::flush()
{
	bool bRet = NDFalse;
	NDGuardLock logLocker( *m_pLogMutex );
	if ( m_nCacheLogPos > 0 && ( m_nCacheLogPos == m_pNDCFile->write( m_szLogCache, m_nCacheLogPos ) ) )
	{
		m_nCacheLogPos  = 0;
		bRet			= NDTrue;

		//这个到底要不要强制flush,有待探讨;
		m_pNDCFile->flush();
	}
	return bRet;
}


////////////////////////////////////////////////////////////////////////////////////////////////
NDCLogManager* g_pLogManager = NULL;

NDCLogManager::NDCLogManager( const char* pszLogPath, NDUint32 nLogMaxSize )
{
	memset( m_szLogPath, 0, sizeof(m_szLogPath) );
	NDUint8 nLen = (NDUint8)strlen(pszLogPath);
	if ( nLen >= MAX_PATH_LEN )
	{
		nLen = MAX_PATH_LEN - 1 ;
	}
	memcpy( m_szLogPath, pszLogPath, nLen );

	m_nLogMaxSize = nLogMaxSize;

	string strLogFilePath(m_szLogPath);
	strLogFilePath += "(";
	strLogFilePath += NDShareBaseGlobal::nd_getLocalSecondTimeStr2();
	strLogFilePath += ").log";

	m_pLogFile = new NDCLogFile( strLogFilePath.c_str(), "ab" );
}

NDCLogManager::~NDCLogManager()
{
	if ( NULL != m_pLogFile )
	{
		delete m_pLogFile;
		m_pLogFile = NULL;
	}
}

NDBool NDCLogManager::write( const char* pFile, NDInt32 nLine, NDInt32 nLevel, const char* pFormat, ... )
{
	if ( NULL == m_pLogFile )			 return NDFalse;
	if ( NDFalse == m_pLogFile->open() ) return NDFalse;

	if ( m_pLogFile->size() >= m_nLogMaxSize  )
	{
		delete m_pLogFile;
		m_pLogFile = NULL;

		string strLogFilePath(m_szLogPath);
		strLogFilePath += "(";
		strLogFilePath += NDShareBaseGlobal::nd_getLocalSecondTimeStr2();
		strLogFilePath += ").log";

		m_pLogFile = new NDCLogFile( strLogFilePath.c_str(), "ab" );
	}

	va_list ap;
	va_start( ap, pFormat );
	NDBool bRet = m_pLogFile->write( pFile, nLine, nLevel, pFormat, ap );
	va_end(ap);
	return bRet;
}

void NDCLogManager::close()
{
	if ( NULL != m_pLogFile )
	{
		m_pLogFile->close();
	}
}

NDBool NDCLogManager::flush()
{
	if ( NULL == m_pLogFile )
	{
		return NDFalse;
	}

	return m_pLogFile->flush();
}





_NDSHAREBASE_END

