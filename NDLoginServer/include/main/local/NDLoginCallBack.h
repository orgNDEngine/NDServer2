#ifndef __LOGINSERVER_ND_LOGIN_CALL_BACK_H__
#define __LOGINSERVER_ND_LOGIN_CALL_BACK_H__

#include "NDPreCompiler.h"

//#include "Message/NDS2SRegisterCallBack.h"
//#include "Message/NDC2LCallBack.h"
//#include "Message/NDL2ACallBack.h"
//#include "Message/NDClient2LCallBack.h"

class NDLoginCallBack : public NDProtocolCallBack
{
public:
	NDLoginCallBack(void);
	~NDLoginCallBack(void);

	NDBool Process( NDIStream& rIStream, NDProtocolHeader& protocolHeader );

private:
	//NDBool disconnectNotifyDispose( NDIStream& rIStream, NDProtocolHeader& protocolHeader );
	//NDBool timerNotifyDispose( NDIStream& rIStream, NDProtocolHeader& protocolHeader );

private:
	//NDS2SRegisterCallBack		m_NDS2SRegisterCallBack;
	//NDC2LCallBack				m_NDC2LCallBack;
	//NDL2ACallBack				m_NDL2ACallBack;
	//NDClient2LCallBack			m_NDClient2LCallBack;
};

#endif