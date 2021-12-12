#pragma once

#include "Types.h"
class CStream;

#define _E(x) ((NATIVEENUM*)(&x))

class CSerialize
{
public:
	virtual CSerialize& operator <<( CStream& stream );
	virtual CSerialize& operator >>( CStream& stream );
};

class CStream
{
	PVOID m_pBuffer;
	int m_nOffset;
	int m_nLength;

public:
	CStream( PVOID pBuffer, int nLength )
	{
		m_pBuffer = pBuffer;
		m_nLength = nLength;
		m_nOffset = 0;
	}
	CStream( PSTR strBuffer )
	{
		m_pBuffer = strBuffer;
		m_nLength = (int)strlen(strBuffer);
		m_nOffset = 0;
	}

	void Reset()
	{
		m_nOffset = 0;
	}
	
	CStream( NATIVEENUM* e ) : m_pBuffer( e ), m_nOffset( 0 ), m_nLength( sizeof(NATIVEENUM) ) {}
	CStream( uint32_t& n ) : m_pBuffer( &n ), m_nOffset( 0 ), m_nLength( sizeof(uint32_t) ) {}
	CStream( int32_t& n ) : m_pBuffer( &n ), m_nOffset( 0 ), m_nLength( sizeof(int32_t) ) {}
	CStream( uint16_t& n ) : m_pBuffer( &n ), m_nOffset( 0 ), m_nLength( sizeof(uint16_t) ) {}
	CStream( int16_t& n ) : m_pBuffer( &n ), m_nOffset( 0 ), m_nLength( sizeof(int16_t) ) {}
	CStream( uint8_t& n ) : m_pBuffer( &n ), m_nOffset( 0 ), m_nLength( sizeof(uint8_t) ) {}
	CStream( int8_t& n ) : m_pBuffer( &n ), m_nOffset( 0 ), m_nLength( sizeof(int8_t) ) {}
#if !defined(__APPLE__) && !defined(EMSCRIPTEN)
	CStream( int& n ) : m_pBuffer( &n ), m_nOffset( 0 ), m_nLength( sizeof(int) ) {}
#endif
	
	CStream& operator <<( const CStream& data )
	{
		_ASSERT( data.m_nLength < m_nLength - m_nOffset );
		memcpy( ((ui8*)m_pBuffer) + m_nOffset, data.m_pBuffer, data.m_nLength );
		m_nOffset += data.m_nLength;
		return *this;
	}
	CStream& operator >>( const CStream& data )
	{
		memcpy( data.m_pBuffer, ((ui8*)m_pBuffer) + m_nOffset, data.m_nLength );
		m_nOffset += data.m_nLength;
		return *this;
	}
	//
	CStream& operator <<( CSerialize& stream )
	{
		stream << *this;
		return *this;
	}

	CStream& operator >>( CSerialize& stream )
	{
		stream >> *this;
		return *this;
	}
	const int& GetLength() const
	{
		return m_nLength;
	}

	uint32_t GetChecksum()
	{
		uint32_t nAux = 0;
		uint8_t* pBuffer = (uint8_t*)m_pBuffer;
		for ( int i = 0; i < m_nLength; i++, pBuffer++ )
			nAux ^= (*pBuffer) << ((i&3)<<3);
		return nAux;
	}
	ui8& operator[](int nIndex) const
	{
		_ASSERT( nIndex >= 0 && nIndex < GetLength() );
		return ((ui8*)m_pBuffer)[nIndex];
	}
};
