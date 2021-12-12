
	
	CSerialize& CSerialize::operator <<( CStream& stream )
	{
		_ASSERT(0);
		//stream << CStream(this, sizeof(*this));
		return *this;
	}

	CSerialize& CSerialize::operator >>( CStream& stream )
	{
		_ASSERT(0);
		//stream >> CStream(this, sizeof(*this));
		return *this;
	}

class CBufferedReader : public CSerialize
{
enum { BufferSectorSize = 4096};

FILE *f{nullptr};

public:
	bool Open( PSTR strName )
	{
f = fopen(strName, "rb");
		return !!f;
	}
/*    
    ui8* GetBuffer()
    {
        return m_pData;
    }
*/	
	CBufferedReader& operator >>( char* text )
	{
	        fgets(text, 256, f);
		return *this;
	}

	CBufferedReader& operator >>( uint32_t &i )
	{
		fread(&i, sizeof(i), 1, f);
		return *this;
	}

	CBufferedReader& operator >>( uint8_t &i )
	{
		fread(&i, sizeof(i), 1, f);
		return *this;
	}

	CBufferedReader& operator >>( int &i )
	{
		fread(&i, sizeof(i), 1, f);
		return *this;
	}

	virtual CBufferedReader& operator >>( const CStream& stream )
	{
	  fread(&stream[0], stream.GetLength(), 1, f);
		return *this;
	}

	bool Seek(uint32_t lOffset)
	{
fseek(f, lOffset, SEEK_SET);
                return true;
	}

	void Close()
	{
          fclose(f);
	}
    
};

class CBufferedWriter : public CSerialize
{
FILE *f{nullptr};
public:
	bool Open( PSTR strName )
{
  f = fopen(strName, "wb");
  return !!f;
}
	
	CBufferedWriter& operator <<( uint32_t i )
	{
	  fwrite(&i, sizeof(i), 1, f);
		return *this;
	}
	CBufferedWriter& operator <<( uint16_t i )
	{
	  fwrite(&i, sizeof(i), 1, f);
		return *this;
	}
	virtual CBufferedWriter& operator <<( const CStream& stream )
	{
	  fwrite(&stream[0], stream.GetLength(), 1, f);
          return *this;
        }

bool Close()
{
  fclose(f);
 return true;
}
};
