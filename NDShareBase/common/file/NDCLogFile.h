/********************************************************************
	created:	2014/10/01
	filename: 	d:\code\NDServer2\NDShareBase\common\file\NDCLogFile.h
	file base:	NDCLogFile
	purpose:	record log file operation;
	author:		fanxiangdong;
	mail:		fanxiangdong7@126.com;
	qq:			435337751;
*********************************************************************/
#ifndef __SHARE_BASE_ND_C_LOG_FILE_H__
#define __SHARE_BASE_ND_C_LOG_FILE_H__

#include <stdarg.h>

#include "NDTypes.h"
#include "NDShareBaseMacros.h"


_NDSHAREBASE_BEGIN

class NDCFile;
class NDMutexLock;
class NDCLogFile
{
private:
	NDUint32		m_nTotalSize;		//缓存+已经写入文件的大小;
	NDUint32		m_nCacheSize;		//缓存大小;
	NDUint32		m_nCacheLogPos;		//缓存中的数据大小;
	char*			m_szLogCache;		//缓存内存实体;
	NDCFile*		m_pNDCFile;			//真实的文件读写指针;
	NDMutexLock*	m_pLogMutex;

private:
	typedef struct {
		char	*t_name;
		NDInt32  t_val;
	} TRANS;
	static const TRANS priorities[9];

public:
	NDCLogFile( const char* pFileName, const char* pOpenFlag, NDUint32 nCacheSize=DEFAULT_LOG_CACHE_SIZE );
	~NDCLogFile(void);

	NDBool		open() const;
	void		close();
	//将缓存日志内存数据写入文件;
	NDBool		flush();
	NDUint32	size();

	NDBool	write( const char* pFile, NDInt32 nLine, NDInt32 nLevel, const char* pFormat, ... );
	NDBool  write( const char* pFile, NDInt32 nLine, NDInt32 nLevel, const char* pFormat, va_list ap );

private:
    string  getWriteContent( const char* pFile, NDInt32 nLine, NDInt32 nLevel, const char* pFormat, va_list ap );
	
};

class NDCLogManager
{
public:
	NDCLogManager( const char* pszLogPath, NDUint32 nLogMaxSize=DEFAULT_LOG_FILE_MAX_SIZE );
	~NDCLogManager();
	
	NDBool	write( const char* pFile, NDInt32 nLine, NDInt32 nLevel, const char* pFormat, ... );

	//将缓存日志内存数据写入文件;
	NDBool	flush();
	//主动关闭;
	void	close();

private:
	NDCLogManager(const NDCLogManager&);
	NDCLogManager& operator = (const NDCLogManager&);

private:
	char					m_szLogPath[MAX_PATH_LEN];
	NDUint32				m_nLogMaxSize;
	NDCLogFile*				m_pLogFile;
};


/* info : char* it mustn't null */
/* pLogManager : NDCLogManager* it mustn't null */
extern NDCLogManager* g_pLogManager;
#define SET_NDTOTAL_LOG_MGR(pLogManager) \
	g_pLogManager = pLogManager;

#define NDTOTAL_LOG_INFO( info ) \
	NDLOG_INFO << info << NDLOG_END; \
	if(NULL != g_pLogManager)			\
	{ g_pLogManager->write( APLOG_MARK, APLOG_INFO, info ); }

#define NDTOTAL_LOG_WARNGING( info ) \
	NDLOG_WARNGING << info << NDLOG_END; \
	if(NULL != g_pLogManager)				\
	{ g_pLogManager->write( APLOG_MARK, APLOG_WARNING, info ); }

#define NDTOTAL_LOG_ERROR( info ) \
	NDLOG_ERROR << info << NDLOG_END; \
	if(NULL != g_pLogManager)			\
	{ g_pLogManager->write( APLOG_MARK, APLOG_ERR, info ); }

_NDSHAREBASE_END
#endif
