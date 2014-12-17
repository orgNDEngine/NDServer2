#include "function/NDConsoleCMDManager.h"

#include "NDShareBaseGlobal.h"
#include "file/NDConsoleLog.h"



_NDSHAREBASE_BEGIN

NDConsoleCMDManager::NDConsoleCMDManager()
{
	m_consoleCMDMap.clear();
}

NDConsoleCMDManager::~NDConsoleCMDManager()
{
	for ( ConsoleCMDMapIter iterBegin = m_consoleCMDMap.begin(),
							iterEnd	  = m_consoleCMDMap.end();
							iterBegin!= iterEnd;
							++iterBegin )
	{
		SAFE_DELETE( iterBegin->second );
	}
	m_consoleCMDMap.clear();
}

void NDConsoleCMDManager::registerCommad( const string& strKey, NDConsoleCommand* pConsoleCMD )
{
	ConsoleCMDMapIter iterFind = m_consoleCMDMap.find( strKey );
	if ( iterFind == m_consoleCMDMap.end() )
	{
		m_consoleCMDMap.insert( std::make_pair( strKey, pConsoleCMD ) );
	}
}

NDBool NDConsoleCMDManager::process( const string& strMsg )
{
	vector<string> strVec;
	NDShareBaseGlobal::nd_strsplit( strMsg, string(" "), strVec );

	if (strVec.empty())
	{
		NDLOG_INFO << " 消息：" << strMsg.c_str() << " 消息没有处理.." << NDLOG_END;

		return NDFalse;
	}

	ConsoleCMDMapIter iterFind = m_consoleCMDMap.find( strVec[0] );
	if ( iterFind != m_consoleCMDMap.end() )
	{
		iterFind->second->process( strVec );
		return NDTrue;
	}
	else
	{
		NDLOG_INFO << " 消息：" << strMsg.c_str() << " 消息没有处理.." << NDLOG_END;
	}

	return NDFalse;
}


_NDSHAREBASE_END
