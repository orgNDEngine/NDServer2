#include <iostream>
#include <cstdlib>

#include <Windows.h>

#include <time.h>

using namespace std;

#include "NDShareBaseGlobal.h"
#include "NDCLibFun.h"
#include "file/NDCFile.h"
#include "file/NDConfig.h"
#include "net/session/NDSessionManager.h"
#include "net/process/NDServerTask.h"


#include "event/timerEvent/NDTimerEventManager.h"

#include "special/NDSpecialProtocol.h"

#include "NDMessageManager.h"


map<NDUint32, NDClientNetIO*> gClientMap;

int main(int argc, char ** argv)
{
	NDUint32 nDayofweek = NDShareBaseGlobal::nd_getdayofweek( 2012, 11, 4 );

	char szBuf[64] = {0};
	

	NDShareBaseGlobal::nd_init();

	NDUint64 nCurMSSeconds	= NDShareBaseGlobal::nd_getCurMSTimeOfLoacl();
	time_t nCurSeconds	= NDShareBaseGlobal::nd_getCurSecondTimeOfUTC();
	
	struct tm* localTM = localtime( &nCurSeconds );

	NDUint32 nCurSeconds2	= mktime( localTM );

	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 
	printf( "%4d/%02d/%02d %02d:%02d:%02d.%03d ÐÇÆÚ%1d\n",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds,sys.wDayOfWeek);

	string strMoudle = NDShareBaseGlobal::nd_getMoudlePath();
	strMoudle += "/NDClientNetIOTest.conf";

	NDConfigFile configFile;
	if ( NDTrue == configFile.setSource( strMoudle.c_str() ) )
	{
		string strIP	= configFile.getStringDefault( "serverInfo", "address", "127.0.0.1" );
		NDUint16 nPort	= (NDUint16)configFile.getIntDefault( "serverInfo", "port", 5503 );

		string  strEchoData = configFile.getStringDefault( "echo", "data", "hello, sweetheart, wolaiye! IOCP server is OK!" );

		NDClientNetIO::setProcessor( NDMessageManager::getInstance()->getDataProcess() );
		NDMessageManager::getInstance()->RegisterMessage();


		for ( NDUint32 i = 0; i < 1; ++i )
		{
			NDClientNetIO* pClientNetIO = new NDClientNetIO;
			//NDProtocol* pPingProtocol	= new NDPingProtocol;
			pClientNetIO->setPingProtocol( new NDPingProtocol );
			if (pClientNetIO->connect( strIP.c_str(), nPort ))
			{
				NDEchoProtocol echoProtocol;
				echoProtocol.m_strData = strEchoData;
				
				pClientNetIO->sendToServer( echoProtocol, NDFalse, NDFalse, NDTrue );

				gClientMap[ pClientNetIO->getSessionID() ] = pClientNetIO;

				NDSessionManager::getInstance()->eventLoop();

				NDServerTask::getInstance()->taskProcess();

				NDShareBase::NDTimerManager::getInstance()->detectTimerList();
			}
			else
			{
				delete pClientNetIO;
				pClientNetIO = NULL;
			}
		}


		while ( 1 )
		{
			NDSessionManager::getInstance()->eventLoop();

			NDServerTask::getInstance()->taskProcess();

			NDShareBase::NDTimerManager::getInstance()->detectTimerList();

			Sleep( 100 );
		}
	}
	else
	{
		std::cout << " read config file error! " << std::endl;
	}

	system("pause");
	return 0;
};
