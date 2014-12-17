#include "main/local/NDLoginCallBack.h"

//#include "world\NDWorld.h"
#include "main/local/NDLoginServer.h"


NDLoginCallBack::NDLoginCallBack(void)
{
	//NDRegisterCallBackMACRO( sNDLoginServer.dataProcess(), CMD_DISCONNECT_NOTIFY, this )
	//NDRegisterCallBackMACRO( sNDLoginServer.dataProcess(), CMD_TIMER_NOTIFY, this )
}

NDLoginCallBack::~NDLoginCallBack(void)
{
}

NDBool NDLoginCallBack::Process( NDIStream& rIStream, NDProtocolHeader& protocolHeader )
{
	NDBool bRet = NDFalse;

	//switch (protocolHeader.m_nProtocolID)
	//{
	//case CMD_DISCONNECT_NOTIFY:
	//	{
	//		bRet = disconnectNotifyDispose( rIStream, protocolHeader );
	//	}
	//	break;
	//case CMD_TIMER_NOTIFY:
	//	{
	//		bRet = timerNotifyDispose( rIStream, protocolHeader );
	//	}
	//	break;
	//default:
	//	{

	//	}
	//	break;
	//}

	return bRet;
}

//NDBool NDLoginCallBack::disconnectNotifyDispose( NDIStream& rIStream, NDProtocolHeader& protocolHeader )
//{
//	//check NDCenterServer offline;
//	NDRemoteServerInfo* pCenterServerInfo = sNDLoginServer.worldManager()->getRemoteServerInfo( protocolHeader.m_nSessionID );
//	if ( NULL == pCenterServerInfo )
//	{
//		return NDFalse;
//	}
//
//	NDWorld* pWorld = (NDWorld*)pCenterServerInfo;
//	const NDNetAddress& rNetAddress = pWorld->getNetAddress();
//
//	std::ostringstream oStr;
//	oStr		<< " "
//				<< pWorld->getServerName()				<< "("
//				<< rNetAddress.getIP()					<< ":"
//				<< rNetAddress.getPort()				<< ")"
//				<< "(WorldName:"
//				<< pWorld->getWorldName()				<< ")"
//				<< "(WorldID:"
//				<< pWorld->getWorldID()					<< ")"
//				<< " offline! ";
//	NDTOTAL_LOG_ERROR( oStr.str().c_str() )
//	oStr.clear();
//
//	sNDLoginServer.worldManager()->removeRemoteServer( protocolHeader.m_nSessionID );
//	
//	return NDTrue;
//}
//
//NDBool NDLoginCallBack::timerNotifyDispose( NDIStream& rIStream, NDProtocolHeader& protocolHeader )
//{
//	//每500毫秒受到此消息
//	return NDTrue;
//}
