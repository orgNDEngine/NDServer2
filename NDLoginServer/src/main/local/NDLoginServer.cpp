
#ifdef WIN32
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif


#include "main/local/NDLoginServer.h"

initialiseSingleton(NDLoginServer);

NDRefCounted<NDServerManager>	refNDServerManager( new NDServerManager );
NDRefCounted<NDLoginServer>		refNDLoginServer( new NDLoginServer );

#ifdef	WIN32
BOOL WINAPI handlerRoutine( DWORD dwCtrlType );
#endif

void	exceptionDispose( void *pParam );
int		mainWapper( int argc, char** argv );

int main(int argc, char** argv)
{
	ND_EXCEPTION_PROTECTED_BEGIN()

	mainWapper( argc, argv );

	ND_EXCEPTION_PROTECTED_CATCH( sNDLoginServer.getDumpFilePath(), exceptionDispose )

	return 0;
}


int mainWapper( int argc, char** argv )
{
#ifdef	WIN32
	//for output memory leak report;
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	if( !SetConsoleCtrlHandler( (PHANDLER_ROUTINE)handlerRoutine, TRUE ) ) 
	{
		NDTOTAL_LOG_ERROR( " SetConsoleCtrlHandler failed! " )
		return 1;
	}
#endif

	if ( argc <= 1 )
	{
		NDTOTAL_LOG_ERROR( " argc number less than or equal 1. " )
		return 1;
	}

	NDInt32 nServerID = atoi( argv[1] );
	if ( nServerID <= 0 || nServerID > 200 )
	{
		NDTOTAL_LOG_ERROR( " nServerID less than zero or greater than 200. " )
		return 1;
	}

	//read config file and initialize;
	if ( NDFalse == sNDLoginServer.initialize( LOGIN_SERVER, nServerID, "ServerConfig.xml") )
	{
		NDTOTAL_LOG_ERROR( " NDLoginServer initialize failed! " )
		return 1;
	}

	//set log manager;
	SET_NDTOTAL_LOG_MGR( sNDLoginServer.logMgr() )

	//start server service;
	if ( NDFalse == NDServerManager::getSingleton().startServer( NDLoginServer::getSingletonPtr(), NDFalse ) )
	{
		NDTOTAL_LOG_ERROR( " NDLoginServer startServer failed! " )
		return 1;
	}

	//set server started state;
	sNDLoginServer.setState( NDLocalServer::eServState_Started );


	NDServerManager::getSingleton().mainLoop();

	return 0;
}


void exceptionDispose( void *pParam )
{
	NDTOTAL_LOG_ERROR( " exception has generated! " )
	sNDLoginServer.setState( NDLocalServer::eServState_Stopping );
	sNDLoginServer.logMgr()->flush();
}

#ifdef	WIN32
//�ص�����;
BOOL WINAPI handlerRoutine( DWORD dwCtrlType )
{
	NDLoginServer* pLoginServer		= NDLoginServer::getSingletonPtr();
	NDServerManager* pServerMgr		= NDServerManager::getSingletonPtr();
	if ( NULL == pLoginServer || NULL == pServerMgr )
	{
		return FALSE;
	}

	pLoginServer->setState( NDLocalServer::eServState_Stopping );

	switch( dwCtrlType )
	{
	case CTRL_C_EVENT:
		NDTOTAL_LOG_WARNGING( " CTRL_C_EVENT " )
		break;
	case CTRL_BREAK_EVENT:
		NDTOTAL_LOG_WARNGING( " CTRL_BREAK_EVENT " )
		break;
	case CTRL_CLOSE_EVENT:
		NDTOTAL_LOG_WARNGING( " CTRL_CLOSE_EVENT " )
		break;
	case CTRL_LOGOFF_EVENT:
		NDTOTAL_LOG_WARNGING( " CTRL_LOGOFF_EVENT " )
		break;
	case CTRL_SHUTDOWN_EVENT:
		NDTOTAL_LOG_WARNGING( " CTRL_SHUTDOWN_EVENT " )
		break;
	default:
		NDTOTAL_LOG_WARNGING( " default " )
		break;
	}

	pLoginServer->logMgr()->close();

	SAFE_DELETE( pServerMgr )
	SAFE_DELETE( pLoginServer )

	return TRUE;
}
#endif

