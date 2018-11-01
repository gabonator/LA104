#pragma once
#include "Serialize.h"

class CBufferedReader2 : public CSerialize
{
	ui8* m_pData;
	int m_nOffset;

public:
	bool Open( PSTR strName )
	{
		m_pData = (ui8*)BIOS::FAT::GetSharedBuffer();
		m_nOffset = 0;

		if ( BIOS::FAT::Open( strName, BIOS::FAT::IoRead ) != BIOS::FAT::EOk )
			return false;

		BIOS::DBG::Print("reading...\n");

		return BIOS::FAT::Read( m_pData ) == BIOS::FAT::EOk;
	}
	ui32 GetFileSize()
	{
		return BIOS::FAT::GetFileSize();
	}
	CBufferedReader2& operator >>( PSTR str )
	{
		// unsafe!
		int i;
		int nLimit = 64;
		for ( i = 0; i < nLimit-1; i++ )
		{
			str[i] = m_pData[m_nOffset++];
			if ( m_nOffset == BIOS::FAT::SectorSize )
			{
				m_nOffset = 0;
				BIOS::FAT::EResult eResult = BIOS::FAT::Read( m_pData );
				_ASSERT( eResult == BIOS::FAT::EOk );
			}
			if ( str[i] == '\n' )
				break;
		}
		if ( str[i] == '\r' )
			i--;
		str[i] = 0;
		return *this;
	}
	
	CBufferedReader2& operator >>( ui32 &i )
	{
		CStream stream(i);
		return *this >> stream;
	}

	CBufferedReader2& operator >>( ui8 &i )
	{
		CStream stream(i);
		return *this >> stream;
	}

	CBufferedReader2& operator >>( int &i )
	{
		CStream stream(i);
		return *this >> stream;
	}

	CBufferedReader2& operator >>( const ui8 &i )
	{
		// ugly! but MSVC requires const...
		CStream stream(*(ui8*)&i);
		return *this >> stream;
	}

	virtual CBufferedReader2& operator >>( const CStream& stream_ )
	{
		// ugly conversion, GCC requires const
		CStream& stream = *(CStream*)&stream_;
		for (int i = 0; i < stream.GetLength(); i++ )
		{
			stream[i] = m_pData[m_nOffset++];
			if ( m_nOffset == BIOS::FAT::SectorSize )
			{
				m_nOffset = 0;
				BIOS::FAT::EResult eResult = BIOS::FAT::Read( m_pData );
				_ASSERT( eResult == BIOS::FAT::EOk );
			}
		}
		return *this;
	}

	void Seek(ui32 lOffset)
	{
		m_nOffset = lOffset % BIOS::FAT::SectorSize;
		lOffset -= m_nOffset;
		BIOS::FAT::Seek( lOffset );
		BIOS::FAT::EResult eResult = BIOS::FAT::Read( m_pData );
		_ASSERT( eResult == BIOS::FAT::EOk );
	}

	void Close()
	{
		BIOS::FAT::Close();
	}
};

class CBufferedWriter2 : public CSerialize
{
	ui8* m_pData;
	int m_nOffset;
	int m_nSize;

public:
	bool Open( PSTR strName )
	{
		m_pData = (ui8*)BIOS::FAT::GetSharedBuffer();
		m_nOffset = 0;
		m_nSize = 0;

		if ( BIOS::FAT::Open( strName, BIOS::FAT::IoWrite ) != BIOS::FAT::EOk )
		{
			_ASSERT(0);
			return false;
		}

#ifdef _WIN32
		memset(m_pData, 0, BIOS::FAT::SectorSize);
#endif
		return true;
	}

	virtual CBufferedWriter2& operator <<( PSTR str )
	{
		CStream stream(str);
		*this << stream;
		return *this;
	}

	virtual CBufferedWriter2& operator <<( PCSTR str )
	{
		CStream stream((char*)str);
		*this << stream;
		return *this;
	}

	virtual CBufferedWriter2& operator <<( ui32 dwData )
	{
		*this << CStream(dwData);
		return *this;
	}

	virtual CBufferedWriter2& operator <<( int dwData )
	{
		*this << CStream(dwData);
		return *this;
	}

	virtual CBufferedWriter2& operator <<( ui16 wData )
	{
		*this << CStream(wData);
		return *this;
	}

	virtual CBufferedWriter2& operator <<( ui8 dwData )
	{
		*this << CStream(dwData);
		return *this;
	}

	virtual CBufferedWriter2& operator <<( const CStream& stream_ )
	{
		// ugly conversion, GCC requires const
		CStream& stream = *(CStream*)&stream_;
		for (int i = 0; i < stream.GetLength(); i++ )
		{
			m_pData[m_nOffset++] = stream[i];
			if ( m_nOffset == BIOS::FAT::SectorSize )
			{
				m_nOffset = 0;
				BIOS::FAT::EResult eResult = BIOS::FAT::Write( m_pData );
				_ASSERT( eResult == BIOS::FAT::EOk );
#ifdef _WIN32
				memset( m_pData, 0x20, BIOS::FAT::SectorSize );
#endif
			}
		}
		m_nSize += stream.GetLength();
		return *this;
	}

	void Close()
	{
		if ( m_nOffset > 0 )
			BIOS::FAT::Write( m_pData );
		BIOS::FAT::Close( m_nSize );
	}
};
