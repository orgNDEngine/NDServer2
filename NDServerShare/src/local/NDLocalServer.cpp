#include "local/NDLocalServer.h"

#include <sstream>

#include "NDShareBaseMacros.h"
#include "NDShareBaseGlobal.h"
#include "file/NDCLogFile.h"

#include "net/process/NDDataProcess.h"
#include "function/NDConsoleCMDManager.h "

#include "event/timerEvent/NDTimerEventManager.h"

#include "local/NDServerInfo.h"
#include "local/NDXmlConfigServerCommon.h"
#include "local/NDServerManager.h"


NDLocalServer::NDLocalServer()
:m_eState(eServState_Null),
 m_pLocalServerInfo(NULL),
 m_pDataProcess(NULL),
 m_pConfig(NULL),
 m_pConnProcess(NULL),
 m_pCallBack(NULL),
 m_pConsoleCMDMagager(NULL),
 m_pCLogManager(NULL),
 m_nStartServerSecondTimeOfUTC( (NDUint32)ND_INVALID_ID )
{
	memset( &m_serverCfgSystemInfo, 0, sizeof(m_serverCfgSystemInfo) );
	memset( m_szDumpFilePath, 0, sizeof(m_szDumpFilePath) );

	m_postringstream = new ostringstream;

	NDShareBase::NDTimerManager::getInstance();
}

NDLocalServer::~NDLocalServer()
{
	SAFE_DELETE(m_pLocalServerInfo)
	SAFE_DELETE(m_pDataProcess);
	SAFE_DELETE(m_pConfig);
	SAFE_DELETE(m_pConnProcess);
	SAFE_DELETE(m_pCallBack);
	SAFE_DELETE(m_pConsoleCMDMagager);
	SAFE_DELETE(m_pCLogManager);

	SAFE_DELETE(m_postringstream)

	NDShareBase::NDTimerManager::releaseInstance();
}	


NDBool NDLocalServer::initialize( SERVERTYPE eType, NDUint32 nServerID, const char* szXmlConfig )
{
	if ( eType < LOGIN_SERVER || eType >= MANAGE_SERVER || nServerID <= 0 || NULL == szXmlConfig || '\0' == szXmlConfig[0] )
	{
		return NDFalse;
	}

	if ( (NULL == m_pLocalServerInfo) || (NULL == m_pDataProcess) || (NULL == m_pConfig) || (NULL ==m_pCallBack) )
	{
		return NDFalse;
	}

	m_pLocalServerInfo->setServerType( eType );
	m_pLocalServerInfo->setServerID( nServerID );

	m_pConfig->setServerCfgSytemInfo( &m_serverCfgSystemInfo );
	m_pConfig->setLocalServerInfo( m_pLocalServerInfo );

	if( NDFalse == m_pConfig->readXmlConfig( szXmlConfig ) )
	{
		return NDFalse;
	}

	if ( NDFalse == InitLogMgr() )
	{
		return NDFalse;
	}

	if ( NDFalse == InitDumpFilePath() )
	{
		return NDFalse;
	}

	m_pLocalServerInfo->setLocalRemote( NDServerInfo::eLR_Local );
	
	m_nStartServerSecondTimeOfUTC = NDShareBaseGlobal::nd_getCurSecondTimeOfUTC();

	return NDTrue;
}

NDBool NDLocalServer::InitLogMgr()
{
	if ( NULL == m_pLocalServerInfo )
	{
		return NDFalse;
	}
	const char* szServerName = m_pLocalServerInfo->getServerName();

	string strLogFilePath( m_pLocalServerInfo->getLogPath() );
	strLogFilePath += szServerName;
	strLogFilePath += "/";
	strLogFilePath += szServerName;

	m_pCLogManager = new NDCLogManager( strLogFilePath.c_str() );

	return NDTrue;
}

NDDataProcess* NDLocalServer::dataProcess()
{
	return m_pDataProcess;
}

NDXmlConfigServerCommon* NDLocalServer::config()
{
	return m_pConfig;
}

NDConnectProcess* NDLocalServer::connectProcess()
{
	return m_pConnProcess;
}

NDConsoleCMDManager* NDLocalServer::consoleCMDMgr()
{
	return m_pConsoleCMDMagager;
}

NDLocalServer::eServState_Types NDLocalServer::getState() const
{
	return m_eState;
}

void NDLocalServer::setState( const NDLocalServer::eServState_Types& _eState )
{
	m_eState = _eState;
}

NDCLogManager* NDLocalServer::logMgr()
{
	return m_pCLogManager;
}

const char* NDLocalServer::getDumpFilePath()
{
	return m_szDumpFilePath;
}

NDTime NDLocalServer::getStartServerSecondTimeOfUTC() const
{
	return m_nStartServerSecondTimeOfUTC;
}

NDBool NDLocalServer::InitDumpFilePath()
{
	if ( NULL == m_pLocalServerInfo )
	{
		return NDFalse;
	}

	const char* szServerName = m_pLocalServerInfo->getServerName();

	string strDumpFilePath( m_pLocalServerInfo->getDumpPath() );
	strDumpFilePath += szServerName;
	strDumpFilePath += "/";
	strDumpFilePath += szServerName;
	strDumpFilePath += "(";
	strDumpFilePath += NDShareBaseGlobal::nd_getLocalSecondTimeStr2();
	strDumpFilePath += ").dmp";

	
	NDUint32 nPathSize = (NDUint32)strDumpFilePath.length();
	if ( nPathSize >= MAX_PATH_LEN )
	{
		return NDFalse;
	}

	memcpy( m_szDumpFilePath, strDumpFilePath.c_str(), nPathSize );
	m_szDumpFilePath[nPathSize] = '\0';

	return NDTrue; 
}

void NDLocalServer::run()
{
	NDShareBase::NDTimerManager::getInstance()->detectTimerList();
}

NDShareBase::NDTimerBoundSlotConn NDLocalServer::addTimer( const NDSubFunctorSlot& refSubFunctorSlot, const NDTimerEventArgs& refTimerEventArgs )
{
	return NDShareBase::NDTimerManager::getInstance()->addTimer( refSubFunctorSlot, refTimerEventArgs );
}

const NDServerCfgSytemInfo& NDLocalServer::getServerCfgSytemInfo() const
{
	return m_serverCfgSystemInfo;
}

NDLocalServerInfo* NDLocalServer::getLocalServerInfo()
{
	return m_pLocalServerInfo;
}

ostringstream& NDLocalServer::getostringstream()
{
	return *m_postringstream;
}

