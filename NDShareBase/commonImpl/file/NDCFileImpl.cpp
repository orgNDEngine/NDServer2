#include "NDCFileImpl.h"

#include <string.h>
#include <stdlib.h>

_NDSHAREBASE_BEGIN

NDCFileImpl::NDCFileImpl(void) : m_pFile( NULL ), m_pFileName( NULL ), m_nReadStart( 0 )
{
}

NDCFileImpl::~NDCFileImpl(void)
{
	if ( NULL != m_pFile ) 
	{
		close();
	}
}

NDBool NDCFileImpl::isopen() const
{
	if ( NULL == m_pFile )	return NDFalse;
	
	return NDTrue;
}

NDBool NDCFileImpl::open( const char* pFileName, const char* pOpenFlag )
{
	if ( NULL != m_pFile ) return NDFalse;
	//if ( 0 != (::fopen_s(&m_pFile, pFileName, pOpenFlag )) )
	//{
	//	return NDFalse;
	//}
	if ( NULL == (m_pFile = fopen( pFileName, pOpenFlag )) )
	{
		return NDFalse;
	}
	
	
	if ( NULL != m_pFileName)
	{
		delete m_pFileName;
		m_pFileName = NULL;
	}

	NDUint32 nSize = (NDUint32)strlen(pFileName);
	m_pFileName = new char[nSize + 1];
	m_pFileName[nSize] = '\0';
	memcpy(m_pFileName, pFileName, nSize);

	return NDTrue;
}

NDUint32 NDCFileImpl::read( void* pReadBuf, NDUint32 nSize )
{
	if ( NULL == m_pFile ) return 0;
	if ( 0 == ::fseek(m_pFile, m_nReadStart, SEEK_SET) )
	{
		if ( 1 == ::fread( pReadBuf, nSize, 1, m_pFile ) )
		{
			m_nReadStart += (long)nSize;
			return nSize;
		}
	}
	return 0;
}

NDUint32 NDCFileImpl::write( const void* pWriteBuf, NDUint32 nSize )
{
	if ( NULL == m_pFile ) return 0;
	if ( 0 == ::fseek(m_pFile, 0, SEEK_END) )
	{
		if ( 1 == ::fwrite( pWriteBuf, nSize, 1, m_pFile ) )
		{
			//::fflush( m_pFile );	//possible effect  efficiency
			return nSize;
		}
	}
	return 0;
}

NDUint32 NDCFileImpl::size()
{
	if ( NULL == m_pFile ) return 0;
	if ( 0 == ::fseek(m_pFile, 0, SEEK_END) )
	{
		NDUint32 nSize = ::ftell(m_pFile);
		::fseek(m_pFile, 0, SEEK_SET);
		return nSize;
	}
	return 0;
}

void NDCFileImpl::close()
{
	if ( NULL == m_pFile ) return ;

	::fflush( m_pFile );
	::fclose( m_pFile );

	m_pFile = NULL;
	if ( NULL != m_pFileName)
	{
		delete m_pFileName;
		m_pFileName = NULL;
	}
}

NDInt32	NDCFileImpl::flush()
{
	if ( NULL == m_pFile ) return 1;
	return ::fflush( m_pFile );
}

_NDSHAREBASE_END

