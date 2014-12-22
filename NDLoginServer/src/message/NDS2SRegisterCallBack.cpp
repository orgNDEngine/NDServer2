#include "message/NDS2SRegisterCallBack.h"

#include <sstream>

#include "protocol/NDWorldToNDLogin/NDWS2LS_Register.h"

#include "main/remote/NDWorldServerInfo.h"
#include "main/local/NDLoginServer.h"


NDS2SRegisterCallBack::NDS2SRegisterCallBack(void)
{
	NDRegisterCallBackMACRO( sNDLoginServer.dataProcess(), CMDP_NDWS2LS_Register_Req, this )
	//NDRegisterCallBackMACRO( sNDLoginServer.dataProcess(), CMDP_NDL2A_Register_Res, this )
}

NDS2SRegisterCallBack::~NDS2SRegisterCallBack(void)
{
}

NDBool NDS2SRegisterCallBack::Process( NDIStream& rIStream, NDProtocolHeader& protocolHeader )
{
	NDBool bRet = NDFalse;

	switch (protocolHeader.m_nProtocolID)
	{
	case CMDP_NDWS2LS_Register_Req:
		{
			bRet = c2lRegisterReqDispose( rIStream, protocolHeader );
		}
		break;
	//case CMD_NDL2A_Register_Res:
	//	{
	//		bRet = l2aRegisterResDispose( rIStream, protocolHeader );
	//	}
	//	break;
	}

	return bRet;
}

NDBool NDS2SRegisterCallBack::c2lRegisterReqDispose( NDIStream& rIStream, NDProtocolHeader& protocolHeader )
{
	NDWS2LS_Register_Req registerReq;
	if ( NDFalse == registerReq.Deserialize(rIStream) )
	{
		NDTOTAL_LOG_ERROR( " [NDS2SRegisterCallBack::c2lRegisterReqDispose] NDC2L_Register_Req deserialize failed! " )
		return NDFalse;
	}

	NDWorldServerInfo* pWorld =  new NDWorldServerInfo;
	if ( NULL ==  pWorld )
	{
		NDTOTAL_LOG_ERROR( " [NDS2SRegisterCallBack::c2lRegisterReqDispose] new NDWorld failed! " )
		return NDFalse;
	}

	pWorld->setServerType( WORLD_SERVER );
	
	pWorld->setWorldID( registerReq.m_nWorldID );
	pWorld->setServerName( "NDWorldServer" );
	pWorld->setWorldName( registerReq.m_szWorldName );
	pWorld->setSessionID( protocolHeader.m_nSessionID );
	pWorld->setNetAddress( registerReq.m_netAddress );

	sNDLoginServer.worldManager()->addRemoteServer( pWorld );

	//NDServerManager::getSingleton().SetSessionProtocolType( protocolHeader.m_nSessionID, NDSessionProtocolType_CS2L );

	const NDSocketAddress& rNetAddress = registerReq.m_netAddress;

	ostringstream& oStr = sNDLoginServer.getostringstream();
	oStr		<< " " 
				<< pWorld->getServerName()		<< " ["
				<< rNetAddress.getIP()			<< ":"
				<< rNetAddress.getPort()		<< "] "
				<< "[WorldName:"
				<< pWorld->getWorldName()		<< "]"
				<< " connected! ";
	string oStrTemp( oStr.str() );
	NDTOTAL_LOG_INFO( oStrTemp.c_str() )
	oStr.clear();
	oStr.str("");


	NDLS2WS_Register_Res response;
	response.m_uiErrCode = eND_SRS_OK;
	return NDServerManager::getSingleton().sendToClient( response, protocolHeader.m_nSessionID );
}


//NDBool NDS2SRegisterCallBack::l2aRegisterResDispose( NDIStream& rIStream, NDProtocolHeader& protocolHeader )
//{
//	NDL2A_Register_Res response;
//	if ( NDFalse == response.Deserialize( rIStream ) )
//	{
//		NDTOTAL_LOG_ERROR( " [NDS2SRegisterCallBack::l2aRegisterResDispose] NDL2A_Register_Res deserialize failed! " )
//		return NDFalse;
//	}
//
//
//	const NDServerInfo* pAccountDBServerInfo = NDServerManager::getSingleton().getConnServerInfo( protocolHeader.m_nSessionID );
//	if ( NULL == pAccountDBServerInfo )
//	{
//		NDTOTAL_LOG_ERROR( " [NDS2SRegisterCallBack::l2aRegisterResDispose] pAccountDBServerInfo is NULL! " )
//		return NDFalse;
//	}
//
//	std::ostringstream oStr;
//	oStr		<< " register "
//				<< pAccountDBServerInfo->getServerName()	<< "("
//				<< pAccountDBServerInfo->getServerIP()		<< ":"
//				<< pAccountDBServerInfo->getServerPort()	<< ")"
//				<< " return response success! ";
//	NDTOTAL_LOG_INFO( oStr.str().c_str() )
//	oStr.clear();
//
//
//	return NDTrue;
//}


