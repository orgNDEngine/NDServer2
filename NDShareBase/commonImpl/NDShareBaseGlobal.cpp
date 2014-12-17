#include "NDShareBaseGlobal.h"


#include <sys/timeb.h>


#ifndef  WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <execinfo.h>
#endif

#include "zlib.h"


#include "NDCLibFun.h"
#include "function/NDStatWatcher.h"

 
_NDSHAREBASE_BEGIN

#if  !defined(WIN32) 

typedef struct _SYSTEMTIME {
	NDUint16	wYear;
	NDUint16	wMonth;
	NDUint16 	wDayOfWeek;
	NDUint16 	wDay;
	NDUint16 	wHour;
	NDUint16 	wMinute;
	NDUint16 	wSecond;
	NDUint16 	wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

void GetLocalTime( LPSYSTEMTIME lpSystemTime )
{
	struct timeval tv;
	gettimeofday( &tv, NULL );
	
	tm tm_now;
	localtime_r( &tv.tv_sec, &tm_now );
	
	lpSystemTime->wYear			= tm_now.tm_year+1900;						//年份;
	lpSystemTime->wMonth		= tm_now.tm_mon + 1;						//月 tm[0-11] sys[1-12];
	lpSystemTime->wDay			= tm_now.tm_mday;							//日;
	lpSystemTime->wDayOfWeek	= (tm_now.tm_wday + 1)%7;					// tm一星期的日数,从星期一算起,范围为0-6 sys从星期日算起;
	lpSystemTime->wHour			= tm_now.tm_hour;							//小时;
	lpSystemTime->wMinute		= tm_now.tm_min;							//分钟;
	lpSystemTime->wSecond		= tm_now.tm_sec;							//秒;	
	lpSystemTime->wMilliseconds	= tv.tv_usec/1000;							//毫秒;
}

#endif


NDUint64 NDShareBaseGlobal::m_nRealWorldMSTimeInStartBoot	= 0;
NDUint64 NDShareBaseGlobal::m_nUTCMSTimeInStartBoot			= 0;
NDUint64 NDShareBaseGlobal::m_nServerGUIDBase				= 0;

const NDUint8	NDShareBaseGlobal::m_szToupper[CTYPE_NUM_CHARS] = {
	0x00,	0x01,   0x02,   0x03,   0x04,   0x05,   0x06,   0x07,
	0x08,   0x09,   0x0a,   0x0b,   0x0c,   0x0d,   0x0e,   0x0f,
	0x10,   0x11,   0x12,   0x13,   0x14,   0x15,   0x16,   0x17,
	0x18,   0x19,   0x1a,   0x1b,   0x1c,   0x1d,   0x1e,   0x1f,
	0x20,   0x21,   0x22,   0x23,   0x24,   0x25,   0x26,   0x27,
	0x28,   0x29,   0x2a,   0x2b,   0x2c,   0x2d,   0x2e,   0x2f,
	0x30,   0x31,   0x32,   0x33,   0x34,   0x35,   0x36,   0x37,
	0x38,   0x39,   0x3a,   0x3b,   0x3c,   0x3d,   0x3e,   0x3f,
	0x40,   0x41,   0x42,   0x43,   0x44,   0x45,   0x46,   0x47,
	0x48,   0x49,   0x4a,   0x4b,   0x4c,   0x4d,   0x4e,   0x4f,
	0x50,   0x51,   0x52,   0x53,   0x54,   0x55,   0x56,   0x57,
	0x58,   0x59,   0x5a,   0x5b,   0x5c,   0x5d,   0x5e,   0x5f,
	0x60,   'A',    'B',    'C',    'D',    'E',    'F',    'G',
	'H',    'I',    'J',    'K',    'L',    'M',    'N',    'O',
	'P',    'Q',    'R',    'S',    'T',    'U',    'V',    'W',
	'X',    'Y',    'Z',    0x7b,   0x7c,   0x7d,   0x7e,   0x7f,
	0x80,   0x81,   0x82,   0x83,   0x84,   0x85,   0x86,   0x87,
	0x88,   0x89,   0x8a,   0x8b,   0x8c,   0x8d,   0x8e,   0x8f,
	0x90,   0x91,   0x92,   0x93,   0x94,   0x95,   0x96,   0x97,
	0x98,   0x99,   0x9a,   0x9b,   0x9c,   0x9d,   0x9e,   0x9f,
	0xa0,   0xa1,   0xa2,   0xa3,   0xa4,   0xa5,   0xa6,   0xa7,
	0xa8,   0xa9,   0xaa,   0xab,   0xac,   0xad,   0xae,   0xaf,
	0xb0,   0xb1,   0xb2,   0xb3,   0xb4,   0xb5,   0xb6,   0xb7,
	0xb8,   0xb9,   0xba,   0xbb,   0xbc,   0xbd,   0xbe,   0xbf,
	0xc0,   0xc1,   0xc2,   0xc3,   0xc4,   0xc5,   0xc6,   0xc7,
	0xc8,   0xc9,   0xca,   0xcb,   0xcc,   0xcd,   0xce,   0xcf,
	0xd0,   0xd1,   0xd2,   0xd3,   0xd4,   0xd5,   0xd6,   0xd7,
	0xd8,   0xd9,   0xda,   0xdb,   0xdc,   0xdd,   0xde,   0xdf,
	0xe0,   0xe1,   0xe2,   0xe3,   0xe4,   0xe5,   0xe6,   0xe7,
	0xe8,   0xe9,   0xea,   0xeb,   0xec,   0xed,   0xee,   0xef,
	0xf0,   0xf1,   0xf2,   0xf3,   0xf4,   0xf5,   0xf6,   0xf7,
	0xf8,   0xf9,   0xfa,   0xfb,   0xfc,   0xfd,   0xfe,   0xff
};

const NDUint8	NDShareBaseGlobal::m_szTolower[CTYPE_NUM_CHARS] = {
	0x00,   0x01,   0x02,   0x03,   0x04,   0x05,   0x06,   0x07,
	0x08,   0x09,   0x0a,   0x0b,   0x0c,   0x0d,   0x0e,   0x0f,
	0x10,   0x11,   0x12,   0x13,   0x14,   0x15,   0x16,   0x17,
	0x18,   0x19,   0x1a,   0x1b,   0x1c,   0x1d,   0x1e,   0x1f,
	0x20,   0x21,   0x22,   0x23,   0x24,   0x25,   0x26,   0x27,
	0x28,   0x29,   0x2a,   0x2b,   0x2c,   0x2d,   0x2e,   0x2f,
	0x30,   0x31,   0x32,   0x33,   0x34,   0x35,   0x36,   0x37,
	0x38,   0x39,   0x3a,   0x3b,   0x3c,   0x3d,   0x3e,   0x3f,
	0x40,   'a',    'b',    'c',    'd',    'e',    'f',    'g',
	'h',    'i',    'j',    'k',    'l',    'm',    'n',    'o',
	'p',    'q',    'r',    's',    't',    'u',    'v',    'w',
	'x',    'y',    'z',    0x5b,   0x5c,   0x5d,   0x5e,   0x5f,
	0x60,   0x61,   0x62,   0x63,   0x64,   0x65,   0x66,   0x67,
	0x68,   0x69,   0x6a,   0x6b,   0x6c,   0x6d,   0x6e,   0x6f,
	0x70,   0x71,   0x72,   0x73,   0x74,   0x75,   0x76,   0x77,
	0x78,   0x79,   0x7a,   0x7b,   0x7c,   0x7d,   0x7e,   0x7f,
	0x80,   0x81,   0x82,   0x83,   0x84,   0x85,   0x86,   0x87,
	0x88,   0x89,   0x8a,   0x8b,   0x8c,   0x8d,   0x8e,   0x8f,
	0x90,   0x91,   0x92,   0x93,   0x94,   0x95,   0x96,   0x97,
	0x98,   0x99,   0x9a,   0x9b,   0x9c,   0x9d,   0x9e,   0x9f,
	0xa0,   0xa1,   0xa2,   0xa3,   0xa4,   0xa5,   0xa6,   0xa7,
	0xa8,   0xa9,   0xaa,   0xab,   0xac,   0xad,   0xae,   0xaf,
	0xb0,   0xb1,   0xb2,   0xb3,   0xb4,   0xb5,   0xb6,   0xb7,
	0xb8,   0xb9,   0xba,   0xbb,   0xbc,   0xbd,   0xbe,   0xbf,
	0xc0,   0xc1,   0xc2,   0xc3,   0xc4,   0xc5,   0xc6,   0xc7,
	0xc8,   0xc9,   0xca,   0xcb,   0xcc,   0xcd,   0xce,   0xcf,
	0xd0,   0xd1,   0xd2,   0xd3,   0xd4,   0xd5,   0xd6,   0xd7,
	0xd8,   0xd9,   0xda,   0xdb,   0xdc,   0xdd,   0xde,   0xdf,
	0xe0,   0xe1,   0xe2,   0xe3,   0xe4,   0xe5,   0xe6,   0xe7,
	0xe8,   0xe9,   0xea,   0xeb,   0xec,   0xed,   0xee,   0xef,
	0xf0,   0xf1,   0xf2,   0xf3,   0xf4,   0xf5,   0xf6,   0xf7,
	0xf8,   0xf9,   0xfa,   0xfb,   0xfc,   0xfd,   0xfe,   0xff
};


NDUint8 NDShareBaseGlobal::m_szMoudlePath[MAX_PATH_LEN] = {0};




#ifdef WIN32

LARGE_INTEGER NDShareBaseGlobal::m_start;
LARGE_INTEGER NDShareBaseGlobal::m_FrequencyCounter;

#else

struct timeval NDShareBaseGlobal::m_start;

#endif



const char* pStatContentArray[] = { ST_PROTIME };
StatData* NDShareBaseGlobal::g_spStatFunc = new StatData( pStatContentArray, 1 );
map<NDUint32, string> NDShareBaseGlobal::g_sStatFuncNameMap;



const NDUint8 nGroupMove	= 56;
const NDUint8 nServerMove	= 48;
const NDUint8 nCountMove	= 32;


///////////////////////////////////////////////////////////////////////////

NDBool NDShareBaseGlobal::nd_init()
{
	//init time function;
	nd_initForGetMSTime();

	return NDTrue;
}

NDBool NDShareBaseGlobal::nd_isCPULittleEndian()
{
	union
	{
		unsigned int	a;
		unsigned char	b;
	}c;

	c.a = 1;

	return ( c.b == 1 );
}


NDBool NDShareBaseGlobal::nd_isValidPointer( const void* x )
{
#ifdef WIN32
	return ( x != (void*)0xcccccccc ) && ( x != (void*)0xdeadbeef) && ( x != (void*)0xfeeefeee ) && ( x != NULL );
#else
	return ( x != NULL );
#endif
}

NDUint32 NDShareBaseGlobal::nd_getdayofweek( NDUint32 nYear, NDUint32 nMonth, NDUint32 nDay )
{
	if ( (nMonth == 1) || (nMonth ==2) )
	{
		nMonth += 12;
		--nYear;
	}
	return (nYear + nYear/4 - nYear/100 + nYear/400 + (2*nMonth) + (3*(nMonth+1))/5 + nDay) % 7 + 1;
}


const char* NDShareBaseGlobal::nd_getMoudlePath() 
{
#ifdef	WIN32
	if ( m_szMoudlePath[0] == '\0' )
	{
		if ( 0 == ::GetModuleFileNameA( NULL, (char*)m_szMoudlePath, MAX_PATH_LEN ) )
		{
			return (const char*)m_szMoudlePath;
		}

		char* pFind = strrchr( (char*)m_szMoudlePath, '\\' );
		if ( NULL == pFind )
			pFind = strrchr( (char*)m_szMoudlePath, '/' );
		if ( NULL == pFind )
			return (const char*)m_szMoudlePath;

		*pFind = '\0';
	}
#else
#endif
	return (const char*)m_szMoudlePath;
}

string NDShareBaseGlobal::nd_getLocalDayTimeStr() 
{
	//纯C实现方式;
	//char szBuf[32] = {0};

	//time_t time_ = ::time( NULL );
	//tm* localTime= ::localtime( &time_ );	//tm * localTime = ::gmtime(&time_);
	//
	////tm  timeInfo  = { 0 };
	////tm* localTime = &timeInfo;
	////::localtime_s( localTime, &time_ );
	////_snprintf_s( szBuf, 31, "%-4d-%02d-%02d", localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday );

	//ND_SNPRINTF( szBuf, 31, "%-4d-%02d-%02d", localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday );
	//return string(szBuf);


	char szBuf[32] = {0};
	SYSTEMTIME sysTime = {0};
	GetLocalTime(&sysTime);

	ND_SNPRINTF( szBuf, 31, "%-4d-%02d-%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay );
	return string(szBuf);
}

string NDShareBaseGlobal::nd_getLocalSecondTimeStr() 
{
	//纯C实现方式;
	//char szBuf[32] = {0};

	//time_t time_ = ::time( NULL );
	//tm* localTime= ::localtime( &time_ );	

	//ND_SNPRINTF( szBuf, 31, "%-4d-%02d-%02d %02d:%02d:%02d", localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday,
	//													   localTime->tm_hour, localTime->tm_min, localTime->tm_sec );
	//return string(szBuf);


	char szBuf[32] = {0};
	SYSTEMTIME sysTime = {0};
	GetLocalTime(&sysTime);

	ND_SNPRINTF( szBuf, 31, "%-4d-%02d-%02d %02d:%02d:%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay,
															 sysTime.wHour, sysTime.wMinute, sysTime.wSecond );
	return string(szBuf);
}

string NDShareBaseGlobal::nd_getLocalSecondTimeStr2() 
{
	//纯C实现方式;
	//char szBuf[32] = {0};

	//time_t time_ = ::time( NULL );
	//tm* localTime= ::localtime( &time_ );	

	//ND_SNPRINTF( szBuf, 31, "%-4d-%02d-%02d %02d-%02d-%02d", localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday,
	//														localTime->tm_hour, localTime->tm_min, localTime->tm_sec );
	//return string(szBuf);

	char szBuf[32] = {0};
	SYSTEMTIME sysTime = {0};
	GetLocalTime(&sysTime);

	ND_SNPRINTF( szBuf, 31, "%-4d-%02d-%02d %02d-%02d-%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay,
															 sysTime.wHour, sysTime.wMinute, sysTime.wSecond );
	return string(szBuf);
}


void NDShareBaseGlobal::nd_initForGetMSTime()
{
#ifdef WIN32
	if ( QueryPerformanceFrequency( &m_FrequencyCounter ) )
	{
		QueryPerformanceCounter( &m_start );
	}

#else

	gettimeofday(&m_start, NULL);

#endif

	struct timeb	tb;
	ftime( &tb );

	//秒;
	m_nRealWorldMSTimeInStartBoot = tb.time - tb.timezone * 60 + ( tb.dstflag ? 3600 : 0 );
	//毫秒;
	m_nRealWorldMSTimeInStartBoot = ( m_nRealWorldMSTimeInStartBoot * 1000 ) + tb.millitm;

	//UTC毫秒;
	m_nUTCMSTimeInStartBoot		  = ( tb.time * 1000 )	+ tb.millitm;
}


NDTime NDShareBaseGlobal::nd_getMSTimeSinceStartBoot()
{
#ifdef WIN32
	LARGE_INTEGER	now;
	QueryPerformanceCounter( &now );
	return (NDTime)( ( now.QuadPart - m_start.QuadPart ) * 1000 / m_FrequencyCounter.QuadPart );
#else
	struct timeval now;
	gettimeofday(&now, NULL);
	return ( (NDTime)( (now.tv_sec - m_start.tv_sec) * 1000 ) + ((now.tv_usec - m_start.tv_usec) / 1000) );	
#endif	
}


NDTime NDShareBaseGlobal::nd_getCurMSTimeOfLoacl()
{
#ifdef WIN32
	LARGE_INTEGER	now;
	QueryPerformanceCounter( &now );
	return  ( ( now.QuadPart - m_start.QuadPart ) * 1000 / m_FrequencyCounter.QuadPart ) + m_nRealWorldMSTimeInStartBoot;
#else
	struct timeval now;
	gettimeofday(&now, NULL);
	return ( (NDTime)( (now.tv_sec - m_start.tv_sec) * 1000 ) + ((now.tv_usec - m_start.tv_usec) / 1000) + m_nRealWorldMSTimeInStartBoot );	
#endif	
}


NDTime NDShareBaseGlobal::nd_getCurMSTimeOfUTC()
{
#ifdef WIN32
	LARGE_INTEGER	now;
	QueryPerformanceCounter( &now );
	return  ( ( now.QuadPart - m_start.QuadPart ) * 1000 / m_FrequencyCounter.QuadPart ) + m_nUTCMSTimeInStartBoot;
#else
	struct timeval now;
	gettimeofday(&now, NULL);
	return ( (NDTime)( (now.tv_sec - m_start.tv_sec) * 1000 ) + ((now.tv_usec - m_start.tv_usec) / 1000) + m_nUTCMSTimeInStartBoot );	
#endif	
}


NDTime NDShareBaseGlobal::nd_getCurSecondTimeOfUTC()
{
	return (NDTime)( ::time(NULL) );
}


NDTime NDShareBaseGlobal::nd_getDiffMilliSecondTime( NDTime nEndSecondTime, NDTime nStartSecondTime )
{
	return NDTime( ::difftime( (time_t)nEndSecondTime, (time_t)nStartSecondTime) * 1000 );
}

string NDShareBaseGlobal::nd_transferSecondTimeToStr( NDTime nSecondTime )
{
	char szBuf[32] = {0};

	time_t time_= (time_t)nSecondTime;

#ifdef WIN32
	tm* ptime	= ::localtime( &time_ );
#else

	tm tm_now	= {0};
	localtime_r( &time_, &tm_now );
	tm* ptime	= &tm_now;

#endif

	ND_SNPRINTF( szBuf, 31, "%-4d-%02d-%02d %02d:%02d:%02d", ptime->tm_year + 1900, ptime->tm_mon + 1, ptime->tm_mday,
															 ptime->tm_hour, ptime->tm_min, ptime->tm_sec );
	return string(szBuf);
}

NDTime NDShareBaseGlobal::nd_transferSecondTimeStrToNDTime( const string& strTime )
{
	const char* ptime = strTime.c_str();

	tm tmTime;
	ND_SSCANF( ptime, "%4d-%02d-%02d %02d:%02d:%02d", &tmTime.tm_year, &tmTime.tm_mon, &tmTime.tm_mday,
													   &tmTime.tm_hour, &tmTime.tm_min, &tmTime.tm_sec );
	tmTime.tm_year -= 1900;
	tmTime.tm_mon  -= 1;

	return (NDTime)(::mktime( &tmTime ));
}



NDBool NDShareBaseGlobal::nd_isValidStrParam( const char* pParam, NDUint32& refLen )
{
	if ( NULL == pParam || '\0' == pParam[0] )
	{
		return NDFalse;
	}

	refLen = (NDUint32)strlen( pParam );

	return NDTrue;
}

NDUint32 NDShareBaseGlobal::nd_safeStrCopy( const char* pSrc, char* pDest, NDUint32 nSrcLen, NDUint32 nDestLen )
{
	NDUint32 nRet = nSrcLen;
	if ( nSrcLen >= nDestLen )
	{
		nRet = ( nDestLen - 1 );
	}

	memcpy(pDest, pSrc, nRet);
	pDest[nRet] = '\0';

	return nRet;
}


NDUint32 NDShareBaseGlobal::nd_strlcpy( const char* pSrc, char* pDest, NDUint32 nDestLen )
{
	char* d			= pDest;
	const char* s	= pSrc;
	NDUint32	n	= nDestLen;

	/* Copy as many bytes as will fit */
	if ( ( n != 0 ) && ( --n != 0 ) )
	{
		do 
		{
			if ( (*d++ == *s++) == '\0' )
				break;
		} while ( --n != 0 );
	}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if ( n == 0 )
	{
		if ( nDestLen != 0 )
			*d = '\0';				/* NUL-terminate dst */
		while (*s++)
			;
	}

	return (NDUint32)( s - pSrc - 1 );	/* count does not include NUL */
}


NDInt32	NDShareBaseGlobal::nd_toupper( NDInt32 c )
{
	if ( (NDUint32)c > 255 )
	{
		return (c);
	}

	return m_szToupper[c];
}


NDInt32	NDShareBaseGlobal::nd_tolower( NDInt32 c )
{
	if ( (NDUint32)c > 255 )
	{
		return (c);
	}

	return m_szTolower[c];	
}


void NDShareBaseGlobal::nd_toupper( char* pParam, NDUint32 nLen )
{
	for ( NDUint32 i = 0; i < nLen; ++ i)
	{
		pParam[i] = (char)nd_toupper( pParam[i] );
	}
}

void NDShareBaseGlobal::nd_toupper( string& strParam )
{
	NDUint32 nLen = (NDUint32)strParam.length();
	for ( NDUint32 i = 0; i < nLen; ++ i)
	{
		strParam[i] = (char)nd_toupper( strParam[i] );
	}
}

void NDShareBaseGlobal::nd_tolower( char* pParam, NDUint32 nLen )
{
	for ( NDUint32 i = 0; i < nLen; ++ i)
	{
		pParam[i] = (char)nd_tolower( pParam[i] );
	}
}

void NDShareBaseGlobal::nd_tolower( string& strParam )
{
	NDUint32 nLen = (NDUint32)strParam.length();
	for ( NDUint32 i = 0; i < nLen; ++ i)
	{
		strParam[i] = (char)nd_tolower( strParam[i] );
	}
}


void NDShareBaseGlobal::nd_strtrim( string& refStr, char szFlag )
{
	string::size_type nFlagPos = 0;
	string::size_type nCharPos = 0;

	if ( refStr.npos != ( nCharPos = refStr.find_first_not_of( szFlag ) ) )
	{
		while ( refStr.npos != ( nFlagPos = refStr.find_first_of( szFlag ) ) )
		{
			if ( nFlagPos > nCharPos )
			{
				refStr = refStr.substr( 0, nFlagPos );
			} 
			else
			{
				refStr = refStr.substr( nCharPos );
				if ( refStr.npos == ( nCharPos = refStr.find_first_not_of( szFlag ) ) )
				{
					return;
				}
			}
		}
	}
}


void NDShareBaseGlobal::nd_strsplit( const string& refStr, const string& strKey, vector<string>& refStrVec )
{
	string::size_type nStrKey = strKey.length();
	string::size_type nPrePos = 0;
	string::size_type nCurPos = 0;

	while ( string::npos != ( nCurPos = refStr.find( strKey, nCurPos ) ) )
	{
		if ( nCurPos != nPrePos )
		{
			refStrVec.push_back( refStr.substr( nPrePos, nCurPos - nPrePos ) );

			nCurPos += nStrKey;
			nPrePos	 = nCurPos;
		}
	}

	refStrVec.push_back( refStr.substr( nPrePos ) );
}


NDBool NDShareBaseGlobal::nd_compress( char* pDestBuf, NDUint32* nDestLen, const char* pSrcBuf, NDUint32 nSrcLen )
{
	NDInt32 nRet = compress( (Bytef*)pDestBuf, (uLongf*)nDestLen, (const Bytef*)pSrcBuf, (uLong)nSrcLen );
	if ( Z_OK == nRet )
	{
		return NDTrue;
	}

	return NDFalse;
}

NDBool NDShareBaseGlobal::nd_uncompress( char* pDestBuf, NDUint32* nDestLen, const char* pSrcBuf, NDUint32 nSrcLen )
{
	NDInt32 nRet = uncompress( (Bytef*)pDestBuf, (uLongf*)nDestLen, (const Bytef*)pSrcBuf, (uLong)nSrcLen );
	if ( Z_OK == nRet )
	{
		return NDTrue;
	}

	return NDFalse;
}

NDUint32 NDShareBaseGlobal::nd_crc32( NDUint32 nCRC, const char* pBuf, NDUint32 nBufLen )
{
	return (NDUint32)crc32( (uLong)nCRC, (const Bytef*)pBuf, (uInt)nBufLen );
}


NDUint32 NDShareBaseGlobal::nd_bkdr_hash( const char* pBuf )
{
	if ( NULL == pBuf || '\0' == pBuf[0])
	{
		return 0;
	}

	char* pBufTemp = (char*)pBuf;

	NDUint32	nSeed = 131;	//1313 13131;
	NDUint32	nHash = 0;

	while ( *pBufTemp )
	{
		nHash = ( ( nHash * nSeed ) + (*pBufTemp++) );
	}

	return ( nHash & 0x7FFFFFFF );
}

#ifdef WIN32

//在__excep后面的()中是一个表达式，值可以是:;
//EXCEPTION_CONTINUE_EXECUTION (–1)	异常被忽略,控制流将在异常出现的点之后,继续恢复运行.;
//EXCEPTION_CONTINUE_SEARCH		(0)		异常不被识别,也即当前的这个__except模块不是这个异常错误所对应的正确的异常处理模块.系统将继续到上一层的try-except域中继续查找一个恰当的__except模块.;
//EXCEPTION_EXECUTE_HANDLER		(1)		异常已经被识别,也即当前的这个异常错误,系统已经找到了并能够确认,这个__except模块就是正确的异常处理模块.控制流将进入到__except模块中.;

NDInt32 NDShareBaseGlobal::nd_generateDump( const char* szDumpFilePath, void* pExtend )
{
	if ( NULL == szDumpFilePath || '\0' == szDumpFilePath[0] || NULL == pExtend )
	{
		return EXCEPTION_EXECUTE_HANDLER;
	}

	struct _EXCEPTION_POINTERS* pExcpInfo = (struct _EXCEPTION_POINTERS*)pExtend; 
	//open the file
	HANDLE hFile = ::CreateFileA( szDumpFilePath,
								GENERIC_READ|GENERIC_WRITE,
								FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE,
								NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if ( INVALID_HANDLE_VALUE != hFile )
	{
		// get the process information
		HANDLE hProc	= ::GetCurrentProcess();
		DWORD nProcID	= ::GetProcessId(hProc);

		// if we have SEH info, package it up
		MINIDUMP_EXCEPTION_INFORMATION sehInfo = {0};
		MINIDUMP_EXCEPTION_INFORMATION *sehPtr = NULL;
		if( NULL!= pExcpInfo)
		{
			sehInfo.ThreadId = ::GetCurrentThreadId();
			sehInfo.ExceptionPointers	= pExcpInfo;
			sehInfo.ClientPointers		= FALSE;
			sehPtr = &sehInfo;
		}

		// generate the crash dump
		BOOL result = ::MiniDumpWriteDump(	hProc, nProcID, hFile,
			MiniDumpNormal, sehPtr, NULL, NULL );

		if(!result)
		{
			//HRESULT nError = (HRESULT)GetLastError(); // already an HRESULT;
			//DWORD nError = GetLastError(); // already an HRESULT;
		}

		// close the file
		::CloseHandle( hFile );
	}
	else
	{

	}

	return EXCEPTION_EXECUTE_HANDLER;
}

#else

NDInt32 NDShareBaseGlobal::nd_generateDump( const char* szDumpFilePath, void* pExtend )
{
	if ( NULL == szDumpFilePath || '\0' == szDumpFilePath[0] )
	{
		return 0;
	}

	void *array[32];
	int nSize = backtrace(array, 32);
	
	int fd = open( szDumpFilePath, O_CREAT | O_WRONLY );
	backtrace_symbols_fd( array, nSize, fd );
	close(fd);

	return 0;
}

#endif

void NDShareBaseGlobal::nd_init_guid( NDUint8 nServerGroup, NDUint8 nServerID )
{
	NDUint64 nTemp = nServerGroup;
	//设置服务器组号;
	m_nServerGUIDBase += ( nTemp << nGroupMove );
	//设置服务器号;
	nTemp = nServerID;
	m_nServerGUIDBase += ( nTemp << nServerMove );
}

//	
//服务器组号|服务器号|流水号|时间;
//    8         8		16	  32
NDUint64 NDShareBaseGlobal::nd_get_guid()
{
	NDUint64 nRet = m_nServerGUIDBase;

	//设置流水号;
	static NDUint16 nCounter = 0;
	if ( nCounter >= ND_UINT16_MAX )
	{
		nCounter = 0;
	}
	else
	{
		++nCounter;
	}

	NDUint64 nTemp = nCounter;
	nRet += ( nTemp << nCountMove );

	//设置时间;
	NDUint32 nTime = (NDUint32)time(NULL);
	nRet += nTime;

	return nRet;
}


void NDShareBaseGlobal::nd_save_maptype_log( const char* szFilePathName )
{
	g_spStatFunc->SaveMapTypeLog( szFilePathName, g_sStatFuncNameMap );
}

void NDShareBaseGlobal::nd_sleep( NDTime nSleepMSTime )
{
	// On Windows, a "time slice" is measured in quanta of 3-5 ms (http://support.microsoft.com/kb/259025)
	// Sleep(0) yields the remainder of the time slice, which could be a long time.
	// A 1 ms minimum time experimentally kept the "Empty GApp" at nearly no CPU load at 100 fps,
	// yet nailed the frame timing perfectly.
	static NDTime minRealSleepTime = 3;	//3ms (milliseconds)

	// all time is milliseconds;
	NDTime nowTime			= nd_getCurMSTimeOfUTC();
	NDTime wakeupTime		= nowTime + nSleepMSTime;
	NDTime remainingTime	= nSleepMSTime;
	NDTime sleepTime		= 0;

	while (remainingTime > 0) {

		if (remainingTime > minRealSleepTime * 2.5) {
			// Safe to use Sleep with a time... sleep for half the remaining time;
			sleepTime = (NDTime)max(remainingTime * 0.5, 0.5);
		} else if (remainingTime > minRealSleepTime) {
			// Safe to use Sleep with a zero time;
			// causes the program to yield only
			// the current time slice, and then return.
			sleepTime = 0;
		} else {
			// Not safe to use Sleep; busy wait;
			sleepTime = (NDTime)-1;
		}

		if ( sleepTime >= 0 && ( sleepTime != (NDTime)-1 ) ) {
#ifdef WIN32
			// Translate to milliseconds;
			Sleep( (int)sleepTime );
#else
			// Translate to microseconds;
			usleep((int)(sleepTime * 1000));
#endif
		}

		nowTime	= nd_getCurMSTimeOfUTC();
		if ( nowTime >= wakeupTime )
		{
			remainingTime = 0;
		}
		else
		{
			remainingTime = wakeupTime - nowTime;
		}
	}	
}

void NDShareBaseGlobal::nd_sleep2( NDTime nSleepMSTime )
{
#ifdef WIN32
	// Translate to milliseconds;
	Sleep( (int)nSleepMSTime );
#else
	// Translate to microseconds;
	usleep((int)(nSleepMSTime * 1000));
#endif
}


_NDSHAREBASE_END


