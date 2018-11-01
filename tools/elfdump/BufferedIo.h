#ifndef __TOOLBOX_BUFFEREDIO_H__
#define __TOOLBOX_BUFFEREDIO_H__

#include "Serialize.h"
/*
class CBufferedWriter : public CSerialize
{
	ui8* m_pData;
	int m_nOffset;
	int m_nSize;
	FILEINFO m_FileInfo;

public:
	void Open( PSTR strName )
	{
		m_pData = (ui8*)BIOS::DSK::GetSharedBuffer();
		m_nOffset = 0;
		m_nSize = 0;

		if ( !BIOS::DSK::Open( &m_FileInfo, strName, BIOS::DSK::IoWrite ) )
		{
			_ASSERT(0);
			return;
		}
#ifdef _WIN32
		memset(m_pData, 0, FILEINFO::SectorSize);
#endif
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
			if ( m_nOffset == FILEINFO::SectorSize )
			{
				m_nOffset = 0;
				BIOS::DSK::Write( &m_FileInfo, m_pData );
#ifdef _WIN32
				memset( m_pData, 0x20, FILEINFO::SectorSize );
#endif
			}
		}
		m_nSize += stream.GetLength();
		return *this;
	}

	void Close()
	{
		if ( m_nOffset > 0 )
			BIOS::DSK::Write( &m_FileInfo, m_pData );
		BIOS::DSK::Close( &m_FileInfo, m_nSize );
	}
};
*/
class CBufferedReader : public CSerialize
{
    FILE* m_f;

public:
	bool Open( PSTR strName )
	{

        m_f = fopen(strName, "rb");
		return true;
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
            stream[i] = fgetc(m_f);
		}
		return *this;
	}

	void Close()
    {
        fclose(m_f);
	}
};

class CBufferedReader2 : public CSerialize
{
	int m_nOffset;
    FILE* m_f;
    
public:
	bool Open( PSTR strName )
	{
        m_f = fopen(strName, "rb");
        return m_f != NULL;
	}
    /*
	ui32 GetFileSize()
	{
		return BIOS::FAT::GetFileSize();
	}*/
	
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
            stream[i] = fgetc(m_f);
		}
		return *this;
	}

	void Seek(ui32 lOffset)
	{
        fseek(m_f, lOffset, SEEK_SET);
	}

	void Close()
	{
        fclose(m_f);
	}
};
#endif
