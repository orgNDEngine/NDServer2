#include <iostream>
#include <cstdlib>

using namespace std;

#include <sys/timeb.h>

#include "NDShareBaseGlobal.h"
#include "file/NDConfig.h"
#include "net/session/NDSessionManager.h"
#include "net/process/NDServerTask.h"

#include "event/timerEvent/NDTimerEventManager.h"

#include "NDMessageManager.h"

using namespace NDShareBase;

NDServerNetIO pServerNetIO;

int main(int argc, char ** argv)
{
	NDShareBaseGlobal::nd_init();

	NDInt64 nInt64 = 0x10;
	printf(" nInt64 = %lld. \n", nInt64 );

	struct timeb	tb;
	ftime( &tb );
	printf( " tb.time = %lld. tb.millitm = %d. \n", tb.time, tb.millitm );

	printf( " time(NULL)= %lld. \n", time(NULL) );

	NDTime nCurMSTimesOfUTC = NDShareBaseGlobal::nd_getCurMSTimeOfUTC();
	printf( " nCurMSTimesOfUTC= %lld, second=%d.\n ", nCurMSTimesOfUTC, nCurMSTimesOfUTC/1000 );

	string strMoudle = NDShareBaseGlobal::nd_getMoudlePath();
	strMoudle += "/NDServerNetIOTest.conf";

	NDConfigFile configFile;
	if ( NDTrue == configFile.setSource( strMoudle.c_str() ) )
	{
		string strIP	= configFile.getStringDefault( "serverInfo", "address", "127.0.0.1" );
		NDUint16 nPort	= (NDUint16)configFile.getIntDefault( "serverInfo", "port", 5503 );
		pServerNetIO.setProcessor( NDMessageManager::getInstance()->getDataProcess() );
		pServerNetIO.startServer( strIP.c_str(), nPort );
		//pServerNetIO.SetCheckSessionTimer();

		NDMessageManager::getInstance()->RegisterMessage();

		NDSessionManager::getInstance();

		do 
		{
			pServerNetIO.accept( 100 );

			pServerNetIO.checkSessionTimerFunction();

			NDSessionManager::getInstance()->eventLoop();

			NDServerTask::getInstance()->taskProcess();

			NDShareBase::NDTimerManager::getInstance()->detectTimerList();

			//printf(" sleep before = %lld. \n", NDShareBaseGlobal::nd_getCurMSTimeOfLoacl() );
			NDShareBaseGlobal::nd_sleep( 500 );
			//printf(" sleep after  = %lld. \n", NDShareBaseGlobal::nd_getCurMSTimeOfLoacl() );
		} while (1);
	}
	else
	{
		std::cout << " read config file error! " << std::endl;
	}
	


	system("pause");
	return 0;
};

