#ifndef __SERVERSHARE_ND_SERVER_SHARE_H__
#define __SERVERSHARE_ND_SERVER_SHARE_H__

#include <iosfwd>

//NDShareBase工程头文件;
#include "NDTypes.h"
#include "NDCLibFun.h"
#include "NDShareBaseMacros.h"
#include "NDShareBaseEnums.h"
#include "NDShareBaseGlobal.h"


//file operation header;
#include "file/NDConsoleLog.h"
#include "file/NDCLogFile.h"

//network header;
#include "net/process/NDDataProcess.h"


//function header;
#include "function/NDConsoleCMDManager.h"
#include "function/NDRefCounted.h"

//用using避免使用完全限定名;
using NDShareBase::NDProtocolHeader;
using NDShareBase::NDProtocolCallBack;
using NDShareBase::NDConsoleCMDManager;
using NDShareBase::NDRefCounted;
using NDShareBase::NDConsoleColor;
using NDShareBase::NDCLogManager;
using NDShareBase::g_pLogManager;


//self 工程头文件;
#include "local/NDXmlConfigServerCommon.h"
#include "local/NDServerInfo.h"
#include "local/NDLocalServer.h"
#include "local/NDServerManager.h"

#include "remote/NDRemoteServerInfo.h"
#include "remote/NDRemoteServerManager.h"

#endif
