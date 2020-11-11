#pragma once
#include "Serialize.h"

#if defined(DS203) || defined(DS213) || defined(LA104)
#define BufferSectorSize BIOS::FAT::SectorSize
#else
#define BufferSectorSize (int)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize)
#endif

class CBufferedReader : public CSerialize
{
	ui8* m_pData;
	int m_nOffset;
	int m_nSectorOffset;

public:
	bool Open( PSTR strName )
	{
		m_pData = (ui8*)BIOS::FAT::GetSharedBuffer();
		m_nOffset = 0;
		m_nSectorOffset = 0;

		if ( BIOS::FAT::Open( strName, BIOS::FAT::IoRead ) != BIOS::FAT::EOk )
			return false;

		return BIOS::FAT::Read( m_pData ) == BIOS::FAT::EOk;
	}
    
    ui8* GetBuffer()
    {
        return m_pData;
    }

	int GetFileSize()
	{
		return BIOS::FAT::GetFileSize();
	}

    void ReadLine(char* str, int maxLength, bool* pwrap = nullptr)
    {
        int i;
        if (pwrap)
            *pwrap = true;
        
        for ( i = 0; i < maxLength-1; )
        {
            str[i] = m_pData[m_nOffset++];
            if ( m_nOffset == BufferSectorSize )
            {
		m_nSectorOffset += BufferSectorSize;
                m_nOffset = 0; // TODO: nemame skutocny offset
                BIOS::FAT::EResult eResult = BIOS::FAT::Read( m_pData );
                _ASSERT( eResult == BIOS::FAT::EOk );
            }
            if ( str[i] == '\n' ) // \r \n
            {
                if (i > 0 && str[i-1] == '\r')
                    i--;
                if (pwrap)
                    *pwrap = false;
                break;
            }
            i++;
        }
        _ASSERT(i < maxLength);
        str[i] = 0;
    }
    
    CBufferedReader& operator >>( PSTR str )
	{
        // unsafe
        ReadLine(str, 128);
        return *this;
	}
	
	CBufferedReader& operator >>( ui32 &i )
	{
		CStream stream(i);
		return *this >> stream;
	}

	CBufferedReader& operator >>( ui8 &i )
	{
		CStream stream(i);
		return *this >> stream;
	}

	CBufferedReader& operator >>( int &i )
	{
		CStream stream(i);
		return *this >> stream;
	}

	CBufferedReader& operator >>( const ui8 &i )
	{
		// ugly! but MSVC requires const...
		CStream stream(*(ui8*)&i);
		return *this >> stream;
	}

	virtual CBufferedReader& operator >>( const CStream& stream_ )
	{
		// ugly conversion, GCC requires const
		CStream& stream = *(CStream*)&stream_;
		for (int i = 0; i < stream.GetLength(); i++ )
		{
			stream[i] = m_pData[m_nOffset++];
			if ( m_nOffset == BufferSectorSize )
			{
				m_nOffset = 0;
				m_nSectorOffset += BufferSectorSize;
				BIOS::FAT::EResult eResult = BIOS::FAT::Read( m_pData );
				_ASSERT( eResult == BIOS::FAT::EOk );
			}
		}
		return *this;
	}

	bool Seek(ui32 lOffset)
	{
	        m_nOffset = lOffset % BufferSectorSize;
		lOffset -= m_nOffset;

		if ((int)lOffset == m_nSectorOffset + BufferSectorSize)
		{
                    m_nSectorOffset += BufferSectorSize;
	            BIOS::FAT::EResult eResult = BIOS::FAT::Read( m_pData );
	            return ( eResult == BIOS::FAT::EOk );			
		} 
	        if ((int)lOffset != m_nSectorOffset)
	        {
	            m_nSectorOffset = lOffset;
		    BIOS::FAT::Seek( lOffset );
	            BIOS::FAT::EResult eResult = BIOS::FAT::Read( m_pData );
	            return ( eResult == BIOS::FAT::EOk );
	        }
                return true;
	}

    int GetFileOffset()
    {
        return m_nSectorOffset + m_nOffset;
    }
    
	void Close()
	{
		BIOS::FAT::Close();
	}
    
    bool Eof()
    {
        return GetFileOffset() >= GetFileSize();
    }
};

class CBufferedWriter : public CSerialize
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

	virtual CBufferedWriter& operator <<( PSTR str )
	{
		CStream stream(str);
		*this << stream;
		return *this;
	}

	virtual CBufferedWriter& operator <<( PCSTR str )
	{
		CStream stream((char*)str);
		*this << stream;
		return *this;
	}

	virtual CBufferedWriter& operator <<( ui32 dwData )
	{
		*this << CStream(dwData);
		return *this;
	}

	virtual CBufferedWriter& operator <<( int dwData )
	{
		*this << CStream(dwData);
		return *this;
	}

	virtual CBufferedWriter& operator <<( ui16 wData )
	{
		*this << CStream(wData);
		return *this;
	}

	virtual CBufferedWriter& operator <<( ui8 dwData )
	{
		*this << CStream(dwData);
		return *this;
	}

	virtual CBufferedWriter& operator <<( const CStream& stream_ )
	{
		// ugly conversion, GCC requires const
		CStream& stream = *(CStream*)&stream_;
		for (int i = 0; i < stream.GetLength(); i++ )
		{
			m_pData[m_nOffset++] = stream[i];
			if ( m_nOffset == BufferSectorSize )
			{
				m_nOffset = 0;
				BIOS::FAT::EResult eResult = BIOS::FAT::Write( m_pData );
				_ASSERT( eResult == BIOS::FAT::EOk );
#ifdef _WIN32
				memset( m_pData, 0x20, BufferSectorSize );
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
