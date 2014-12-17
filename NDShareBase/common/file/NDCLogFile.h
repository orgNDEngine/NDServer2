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
	NDUint32		m_nTotalSize;		//����+�Ѿ�д���ļ��Ĵ�С;
	NDUint32		m_nCacheSize;		//�����С;
	NDUint32		m_nCacheLogPos;		//�����е����ݴ�С;
	char*			m_szLogCache;		//�����ڴ�ʵ��;
	NDCFile*		m_pNDCFile;			//��ʵ���ļ���дָ��;
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
	//��������־�ڴ�����д���ļ�;
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

	//��������־�ڴ�����д���ļ�;
	NDBool	flush();
	//�����ر�;
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
