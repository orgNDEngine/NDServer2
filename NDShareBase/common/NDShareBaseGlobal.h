#ifndef __SHARE_BASE_ND_GLOBAL_H__
#define __SHARE_BASE_ND_GLOBAL_H__

#include <vector>
using std::vector;
#include <map>
using std::map;

#include <time.h>

#include "NDTypes.h"
#include "NDShareBaseMacros.h"


#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )

#ifndef	WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <excpt.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp")

#pragma warning(disable:4312)
#pragma warning(disable:4127)

#endif




_NDSHAREBASE_BEGIN

/* Define this if you're using a big-endian machine */
#ifdef USING_BIG_ENDIAN
#include <machine/byte_order.h>
#define nd_bswap_16(x) NXSwapShort(x)
#define nd_bswap_32(x) NXSwapInt(x)
#define nd_bswap_64(x) NXSwapLongLong(x)
#endif

#ifdef USING_BIG_ENDIAN
#define GUID_HIPART(x)		(*((NDUint32*)&(x)))
#define GUID_LOPART(x)		(*(((NDUint32*)&(x))+1))
#define NDUINT32_HIPART(x)	(*((NDUint16*)&(x)))
#define NDUINT32_LOPART(x)	(*(((NDUint16*)&(x))+1))
#else
#define GUID_HIPART(x)		(*(((NDUint32*)&(x))+1))
#define GUID_LOPART(x)		(*((NDUint32*)&(x)))
#define NDUINT32_HIPART(x)	(*(((NDUint16*)&(x))+1))
#define NDUINT32_LOPART(x)	(*((NDUint16*)&(x)))
#endif


class StatData;
//most function isn't thread safe;
struct NDShareBaseGlobal
{
public:
	/************************************************************************
	** function DESCRIPTION:
	**  call init function for NDShareBaseGlobal;
	************************************************************************/
	static NDBool		nd_init();

	/************************************************************************
	** function DESCRIPTION:
	**	return true--CPU is little endian, false -- CPU is big endian.
	************************************************************************/
	static NDBool		nd_isCPULittleEndian();

		/************************************************************************
	** function DESCRIPTION:
	**	return true-- x is right pointer, false -- x is error pointer.
	************************************************************************/
	static NDBool		nd_isValidPointer( const void* x );

	/************************************************************************
	** function DESCRIPTION:
	**	return day of week(number from 1 to 7).
	**	sample:
	**	invoke nd_getdayofweek( 2011, 6, 9) get the number 4.
	************************************************************************/
	static NDUint32		nd_getdayofweek( NDUint32 nYear, NDUint32 nMonth, NDUint32 nDay );

	/************************************************************************
	** function DESCRIPTION:
	**	return current module path string.
	**	sample: 
		if  run "E:\vs.net\NDServer\DebugBin\debug.exe"
		invoke nd_getMoudlePath get "E:\vs.net\NDServer\DebugBin"
	************************************************************************/
	static const char*	nd_getMoudlePath();

	/************************************************************************
	** function DESCRIPTION:
	**  return local day time string.
	**  sample: "2010-4-16"
	************************************************************************/
	static string		nd_getLocalDayTimeStr();

	/************************************************************************
	** function DESCRIPTION:
	**  return local day second time string.
	**  sample: "2010-4-16 10:18:30"
	************************************************************************/
	static string		nd_getLocalSecondTimeStr();

	/************************************************************************
	** function DESCRIPTION:
	**  return local day second time string.
	**  sample: "2010-4-16 10-18-30"
	************************************************************************/
	static string		nd_getLocalSecondTimeStr2();

	/************************************************************************
	** function DESCRIPTION:
	**  The returned NDUint32 is a count of system ticks 
	**  (or milliseconds) since boot time.(误差小于1ms must call nd_initForGetMSTime ).
	************************************************************************/
	static NDTime		nd_getMSTimeSinceStartBoot();

	/************************************************************************
	** function DESCRIPTION:
	**  The returned NDTime is milliseconds time from standard start time
	**	(1970-1-1) to current time( the real world time ). ( must call nd_initForGetMSTime );
	************************************************************************/
	static NDTime		nd_getCurMSTimeOfLoacl();

	/************************************************************************
	** function DESCRIPTION:
	**  The returned NDTime is milliseconds time from standard start time
	**	(1970-1-1) to current time( UTC ms, ret/1000=time(NULL); ). ( must call nd_initForGetMSTime );
	************************************************************************/
	static NDTime		nd_getCurMSTimeOfUTC();

	/************************************************************************
	** function DESCRIPTION:
	**  The returned NDTime is seconds time from standard start time
	**	(1970-1-1) to current time. (just return time(NULL); );
	************************************************************************/
	static NDTime		nd_getCurSecondTimeOfUTC();

	/************************************************************************
	** function DESCRIPTION:
	**  The returned NDTime is Milliseconds time from nEndSecondTime
	**	to nStartSecondTime. (just return difftime( nEndSecondTime, nStartSecondTime )*1000; );
	************************************************************************/
	static NDTime		nd_getDiffMilliSecondTime( NDTime nEndSecondTime, NDTime nStartSecondTime );

	/************************************************************************
	** function DESCRIPTION:
	**  return second time string( transfer nTime to time string ).
	**  sample: "2010-4-16 10:18:30"
	************************************************************************/
	static string		nd_transferSecondTimeToStr( NDTime nSecondTime );


	/************************************************************************
	** function DESCRIPTION:
	**  return second time ( transfer time string to NDUint32 value ).
	************************************************************************/
	static NDTime		nd_transferSecondTimeStrToNDTime( const string& strTime );


	/************************************************************************
	** function DESCRIPTION:
	**  NDTime nSleepMSTime: milliseconds, sleep milliseconds.
	************************************************************************/
	static void			nd_sleep( NDTime nSleepMSTime );

	/************************************************************************
	** function DESCRIPTION:
	**  NDTime nSleepMSTime: milliseconds, sleep milliseconds.
	************************************************************************/
	static void			nd_sleep2( NDTime nSleepMSTime );

	/************************************************************************
	** function DESCRIPTION:
	**  judge const char* pParam is valid or not.
	**  return true--valid, false--invalid.
	************************************************************************/
	static NDBool		nd_isValidStrParam( const char* pParam, NDUint32& refLen );



	/************************************************************************
	** function DESCRIPTION:
	**  safe string copy
	**  return copy size.
	************************************************************************/
	static NDUint32		nd_safeStrCopy( const char* pSrc, char* pDest, NDUint32 nSrcLen, NDUint32 nDestLen );


	/************************************************************************
	** function DESCRIPTION:
	**  safe string copy
	**	Copy pSrc to string pDest of size nDestLen.  At most nDestLen-1 characters
	**	will be copied.  Always NUL terminates (unless nDestLen == 0).
	**  Returns strlen(pSrc); if retval >= nDestLen, truncation occurred.
	**  return copy size.
	************************************************************************/
	static NDUint32		nd_strlcpy( const char* pSrc, char* pDest, NDUint32 nDestLen );



	/************************************************************************
	** function DESCRIPTION:
	**  transfer c pParam to upper .
	**  return NDInt32.
	** 功能：将字符(c)转换为大写英文字母字符;
	************************************************************************/
	static NDInt32		nd_toupper( NDInt32 c );

	/************************************************************************
	** function DESCRIPTION:
	**  transfer c strParam to lower .
	**  return void.
	** 功能：将字符(c)转换为小写英文字母字符;
	************************************************************************/
	static NDInt32		nd_tolower( NDInt32 c );

	/************************************************************************
	** function DESCRIPTION:
	**  transfer char* pParam to upper .
	**  return void.
	** 功能：将字符串(pParam)转换为大写英文字母字符串;
	************************************************************************/
	static void			nd_toupper( char* pParam, NDUint32 nLen );

	/************************************************************************
	** function DESCRIPTION:
	**  transfer string& strParam to upper .
	**  return void.
	** 功能：将string(strParam)转换为大写英文字母字符串组成的string;
	************************************************************************/
	static void			nd_toupper( string& strParam );


	/************************************************************************
	** function DESCRIPTION:
	**  transfer char* pParam to lower .
	**  return void.
	** 功能：将字符串(pParam)转换为小写英文字母字符串;
	************************************************************************/
	static void			nd_tolower( char* pParam, NDUint32 nLen );

	/************************************************************************
	** function DESCRIPTION:
	**  transfer string& strParam to lower .
	**  return void.
	** 功能：将string(strParam)转换为小写英文字母字符串组成的string;
	************************************************************************/
	static void			nd_tolower( string& strParam );

	/************************************************************************
	** function DESCRIPTION:
	**  remove the begin and end of string& refStr szFlag character.
	**  return void.
	** 功能：将string(refStr)前后面的特殊字符szFlag去掉;
	************************************************************************/
	static void			nd_strtrim( string& refStr, char szFlag );

	/************************************************************************
	** function DESCRIPTION:
	**  split const string& refStr to vector<string>& refStrVec depend on const string& strKey.
	**  return void.
	** 功能：将string(refStr)按照string(strkey)分开塞入vector<string>& refStrVec;
	************************************************************************/
	static void			nd_strsplit( const string& refStr, const string& strKey, vector<string>& refStrVec );

	/************************************************************************
	** function DESCRIPTION:
	**  const char* pSrcBuf compress to  char* pDestBuf .
	**  return NDTrue--success NDFalse--failed.
	************************************************************************/
	static NDBool		nd_compress( char* pDestBuf, NDUint32* nDestLen, const char* pSrcBuf, NDUint32 nSrcLen );

	/************************************************************************
	** function DESCRIPTION:
	**  const char* pSrcBuf uncompress to  char* pDestBuf .
	**  return NDTrue--success NDFalse--failed.
	************************************************************************/
	static NDBool		nd_uncompress( char* pDestBuf, NDUint32* nDestLen, const char* pSrcBuf, NDUint32 nSrcLen );

	/************************************************************************
	** function DESCRIPTION:
	**  calculate CRC code.
	**  return CRC len.
	************************************************************************/
	static NDUint32		nd_crc32( NDUint32 nCRC, const char* pBuf, NDUint32 nBufLen );


	/************************************************************************
	** function DESCRIPTION:
	**  this is BKDR HASH function.
	**  transform calculate string code to only NDUint32 code.
	**  return CRC len.
	************************************************************************/
	static NDUint32		nd_bkdr_hash( const char* pBuf );


	/************************************************************************
	** function DESCRIPTION:
	**  this is init guid function.
	**  nServerGroup is server group number.
	**  nServer is server number;
	**  return void.
	************************************************************************/
	static void			nd_init_guid( NDUint8 nServerGroup, NDUint8 nServerID );

	/************************************************************************
	** function DESCRIPTION:
	**  this is get guid function.
	**  call nd_get_guid before must call nd_init_guid init function.
	**  nServer is server number;
	**  return NDUint64.
	************************************************************************/
	static NDUint64		nd_get_guid();

	/************************************************************************
	** function DESCRIPTION:
	**  write exception information to szDumpFile
	************************************************************************/
	static NDInt32		nd_generateDump( const char* szDumpFilePath, void* pExtend );


#ifdef USING_BIG_ENDIAN

	static void			nd_swap16(NDUint16* p);
	static void			nd_swap32(NDUint32* p);
	static void			nd_swap64(NDUint64* p);
	static NDFloat32	nd_swapfloat( NDFloat32 p );
	static NDFloat64	nd_swapdouble( NDFloat64 p );
	static void			nd_swapfloat( NDFloat32 * p );
	static void			nd_swapdouble(NDFloat64 * p);
	static NDUint16 	nd_swap16(NDUint16 p);
	static NDUint32 	nd_swap32(NDUint32 p);
	static NDUint64 	nd_swap64(NDUint64 p);
	static void 		nd_swap16(NDInt16* p);
	static void 		nd_swap32(NDInt32* p);
	static void 		nd_swap64(NDInt64* p);
	static NDInt16 		nd_swap16(NDInt16 p);
	static NDInt32 		nd_swap32(NDInt32 p);
	static NDInt64 		nd_swap64(NDInt64 p);
	
#endif


private:
	/************************************************************************
	** function DESCRIPTION:
	**  init param for GetMSTime;
	************************************************************************/
	static void			nd_initForGetMSTime();

private:
	static NDUint64					m_nRealWorldMSTimeInStartBoot;	//开启程序时的毫秒数(本地真实世界的时间);
	static NDUint64					m_nUTCMSTimeInStartBoot;		//开启程序时的毫秒数(UTC时间);
	static NDUint64					m_nServerGUIDBase;				//GUID的基数;

	static const NDUint8			m_szToupper[CTYPE_NUM_CHARS];
	static const NDUint8			m_szTolower[CTYPE_NUM_CHARS];
	static NDUint8					m_szMoudlePath[MAX_PATH_LEN];

#ifdef WIN32
	static LARGE_INTEGER			m_start;
	static LARGE_INTEGER			m_FrequencyCounter;
#else
	static struct timeval			m_start;
#endif

public:
	static	StatData*					g_spStatFunc;
	static	map<NDUint32, string>		g_sStatFuncNameMap;
	static	void						nd_save_maptype_log( const char* szFilePathName );

};


/* these can be optimized into assembly */
#ifdef USING_BIG_ENDIAN

/*ND_INLINE static void NDShareBaseGlobal::nd_swap16(NDUint16* p) { *p = ((*p >> 8) & 0xff) | (*p << 8); }
ND_INLINE static void NDShareBaseGlobal::nd_swap32(NDUint32* p) { *p = ((*p >> 24 & 0xff)) | ((*p >> 8) & 0xff00) | ((*p << 8) & 0xff0000) | (*p << 24); }
ND_INLINE static void NDShareBaseGlobal::nd_swap64(NDUint64* p) { *p = ((*p >> 56)) | ((*p >> 40) & 0x000000000000ff00ULL) | ((*p >> 24) & 0x0000000000ff0000ULL) | ((*p >> 8 ) & 0x00000000ff000000ULL) |
								((*p << 8 ) & 0x000000ff00000000ULL) | ((*p << 24) & 0x0000ff0000000000ULL) | ((*p << 40) & 0x00ff000000000000ULL) | ((*p << 56)); }*/

ND_INLINE static void NDShareBaseGlobal::nd_swap16(NDUint16* p) { *p = nd_bswap_16((NDUint16)*p); }
ND_INLINE static void NDShareBaseGlobal::nd_swap32(NDUint32* p) { *p = nd_bswap_32((NDUint32)*p); }
ND_INLINE static void NDShareBaseGlobal::nd_swap64(NDUint64* p) { *p = nd_bswap_64((NDUint64)*p);; }

ND_INLINE static NDFloat32 NDShareBaseGlobal::nd_swapfloat( NDFloat32 p )
{
	union { NDFloat32 asfloat; NDUint8 asbytes[4]; } u1, u2;
	u1.asfloat = p;
	/* swap! */
	u2.asbytes[0] = u1.asbytes[3];
	u2.asbytes[1] = u1.asbytes[2];
	u2.asbytes[2] = u1.asbytes[1];
	u2.asbytes[3] = u1.asbytes[0];
    
	return u2.asfloat;
}

ND_INLINE static NDFloat64 NDShareBaseGlobal::nd_swapdouble( NDFloat64 p )
{
	union { NDFloat64 asfloat; NDUint8 asbytes[8]; } u1, u2;
	u1.asfloat = p;
	/* swap! */
	u2.asbytes[0] = u1.asbytes[7];
	u2.asbytes[1] = u1.asbytes[6];
	u2.asbytes[2] = u1.asbytes[5];
	u2.asbytes[3] = u1.asbytes[4];
	u2.asbytes[4] = u1.asbytes[3];
	u2.asbytes[5] = u1.asbytes[2];
	u2.asbytes[6] = u1.asbytes[1];
	u2.asbytes[7] = u1.asbytes[0];

	return u2.asfloat;
}

ND_INLINE static void NDShareBaseGlobal::nd_swapfloat( NDFloat32 * p )
{
	union { NDFloat32 asfloat; NDUint8 asbytes[4]; } u1, u2;
	u1.asfloat = *p;
	/* swap! */
	u2.asbytes[0] = u1.asbytes[3];
	u2.asbytes[1] = u1.asbytes[2];
	u2.asbytes[2] = u1.asbytes[1];
	u2.asbytes[3] = u1.asbytes[0];
	*p = u2.asfloat;
}

ND_INLINE static void NDShareBaseGlobal::nd_swapdouble(NDFloat64 * p)
{
	union { NDFloat64 asfloat; NDUint8 asbytes[8]; } u1, u2;
	u1.asfloat = *p;
	/* swap! */
	u2.asbytes[0] = u1.asbytes[7];
	u2.asbytes[1] = u1.asbytes[6];
	u2.asbytes[2] = u1.asbytes[5];
	u2.asbytes[3] = u1.asbytes[4];
	u2.asbytes[4] = u1.asbytes[3];
	u2.asbytes[5] = u1.asbytes[2];
	u2.asbytes[6] = u1.asbytes[1];
	u2.asbytes[7] = u1.asbytes[0];
	*p = u2.asfloat;
}

/*ND_INLINE static NDUint16 NDShareBaseGlobal::nd_swap16(NDUint16 p) { return ((p >> 8) & 0xff) | (p << 8); }
ND_INLINE static NDUint32 NDShareBaseGlobal::nd_swap32(NDUint32 p) { return ((p >> 24) & 0xff) | ((p >> 8) & 0xff00) | ((p << 8) & 0xff0000) | (p << 24); }
ND_INLINE static NDUint64 NDShareBaseGlobal::nd_swap64(NDUint64 p)  { p = (((p >> 56) & 0xff)) | ((p >> 40) & 0x000000000000ff00ULL) | ((p >> 24) & 0x0000000000ff0000ULL) | ((p >> 8 ) & 0x00000000ff000000ULL) |
								((p << 8 ) & 0x000000ff00000000ULL) | ((p << 24) & 0x0000ff0000000000ULL) | ((p << 40) & 0x00ff000000000000ULL) | ((p << 56)); }

ND_INLINE static void NDShareBaseGlobal::nd_swap16(NDInt16* p) { *p = ((*p >> 8) & 0xff) | (*p << 8); }
ND_INLINE static void NDShareBaseGlobal::nd_swap32(NDInt32* p) { *p = ((*p >> 24) & 0xff) | ((*p >> 8) & 0xff00) | ((*p << 8) & 0xff0000) | (*p << 24); }
ND_INLINE static void NDShareBaseGlobal::nd_swap64(NDInt64* p) { *p = ((*p >> 56) & 0xff) | ((*p >> 40) & 0x000000000000ff00ULL) | ((*p >> 24) & 0x0000000000ff0000ULL) | ((*p >> 8 ) & 0x00000000ff000000ULL) |
								((*p << 8 ) & 0x000000ff00000000ULL) | ((*p << 24) & 0x0000ff0000000000ULL) | ((*p << 40) & 0x00ff000000000000ULL) | ((*p << 56)); }

ND_INLINE static NDInt16 NDShareBaseGlobal::nd_swap16(NDInt16 p) { return ((p >> 8) & 0xff) | (p << 8); }
ND_INLINE static NDInt32 NDShareBaseGlobal::nd_swap32(NDInt32 p) { return ((p >> 24) & 0xff) | ((p >> 8) & 0xff00) | ((p << 8) & 0xff0000) | (p << 24); }
ND_INLINE static NDInt64 NDShareBaseGlobal::nd_swap64(NDInt64 p)  { return ((((p >> 56) & 0xff)) | ((p >> 40) & 0x000000000000ff00ULL) | ((p >> 24) & 0x0000000000ff0000ULL) | ((p >> 8 ) & 0x00000000ff000000ULL) |
								((p << 8 ) & 0x000000ff00000000ULL) | ((p << 24) & 0x0000ff0000000000ULL) | ((p << 40) & 0x00ff000000000000ULL) | ((p << 56))); }*/

ND_INLINE static NDUint16 NDShareBaseGlobal::nd_swap16(NDUint16 p) { return nd_bswap_16((NDUint16)p); }
ND_INLINE static NDUint32 NDShareBaseGlobal::nd_swap32(NDUint32 p) { return nd_bswap_32((NDUint32)p); }
ND_INLINE static NDUint64 NDShareBaseGlobal::nd_swap64(NDUint64 p) { return nd_bswap_64((NDUint64)p); }

ND_INLINE static void NDShareBaseGlobal::nd_swap16(NDInt16* p) { *p = nd_bswap_16((NDUint16)*p); }
ND_INLINE static void NDShareBaseGlobal::nd_swap32(NDInt32* p) { *p = nd_bswap_32((NDUint32)*p); }
ND_INLINE static void NDShareBaseGlobal::nd_swap64(NDInt64* p) { *p = nd_bswap_64((NDUint64)*p); }

ND_INLINE static NDInt16 NDShareBaseGlobal::nd_swap16(NDInt16 p) { return nd_bswap_16((NDInt16)p); }
ND_INLINE static NDInt32 NDShareBaseGlobal::nd_swap32(NDInt32 p) { return nd_bswap_32((NDUint32)p); }
ND_INLINE static NDInt64 NDShareBaseGlobal::nd_swap64(NDInt64 p) { return nd_bswap_64((NDUint64)p); }

#endif




#ifndef ND_EXCEPTION_PROTECTED
#define ND_EXCEPTION_PROTECTED							

#ifdef WIN32

#define ND_EXCEPTION_PROTECTED_BEGIN()									_try {
#define ND_EXCEPTION_PROTECTED_CATCH( szDumpPath, NDPTHREADFUN )		} __except( NDShareBaseGlobal::nd_generateDump( szDumpPath, GetExceptionInformation() ) ) { NDPTHREADFUN(NULL); }

#else
#define ND_EXCEPTION_PROTECTED_BEGIN()									try {
#define ND_EXCEPTION_PROTECTED_CATCH( szDumpPath, NDPTHREADFUN )		} catch( NDShareBaseGlobal::nd_generateDump( szDumpPath, NULL ) ) { NDPTHREADFUN(NULL); }

#endif

#endif




_NDSHAREBASE_END
#endif

