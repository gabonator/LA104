//TODO: cleanup
#include "Utils.h"
//#include "../bios/Bios.h"

char tmp[16];
/*static */ const char hex[16] = {'0', '1', '2', '3', '4',
			'5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

/*static*/ int CUtils::atoi(char *str)
{
	int nValue = 0;
	while ( *str >= '0' && *str <= '9' )
	{
		nValue *= 10;
		nValue += *str++ - '0';
	}
	return nValue;
}

int hexval(char ch)
{
	if ( ch >= '0' && ch <= '9' )
		return ch - '0';
	if ( ch >= 'A' && ch <= 'F' )
		return ch - 'A' + 10;
	if ( ch >= 'a' && ch <= 'f' )
		return ch - 'a' + 10;
	return -1;
}
/*
bool ishex(char c)
{
	if ( ch >= '0' && ch <= '9' )
		return true;
	if ( ch >= 'A' && ch <= 'F' )
		return true;
	if ( ch >= 'a' && ch <= 'f' )
		return true;
	return false;
}*/

/*static*/ ui32 CUtils::htoi(char *str)
{
	ui32 nValue = 0;
	int nDigit = 0;
	while ( (nDigit = hexval(*str++)) != -1 )
	{
		nValue <<= 4;
		nValue |= nDigit;
	}
	return nValue;
}

/*static*/ char* CUtils::itoa2(ui8 n)
{
	for (int i=0; i<8; i++)
		if (n & (128>>i) )
			tmp[i] = '1';
		else
			tmp[i] = '0';
	tmp[8] = 0;
	return tmp;
}

/*static*/ char* CUtils::itoa(si16 n)
{
	int i = 0;
	ui8 bSignum = 0;
	if (n<0)
	{
		n = -n;
		bSignum = 1;
	} else
	if (n==0)
	{
		tmp[i++] = '0';
	}
	while (n > 0)
	{
		tmp[i++] = '0'+(n%10);
		n /= 10;
	}
	if (bSignum)
		tmp[i++] = '-';
	tmp[i] = 0;
	for (int j=0; j<i/2;j++)
	{
		char t = tmp[j];
		tmp[j] = tmp[i-j-1];
		tmp[i-j-1] = t;
	}
	return tmp;
}
/*static*/ char CUtils::tohex(ui8 n)
{
	_ASSERT( n >= 0 && n < 16 );
	return hex[n];
}
/*static*/ char* CUtils::clrhex(ui16 c)
{
	tmp[0] = tohex((c&0x1f)>>1);
	tmp[1] = tohex(((c>>5)&0x3f)>>2);
	tmp[2] = tohex((c>>11)>>1);
	tmp[3] = 0;
	return tmp;
}
#if 0
/*static*/ char* CUtils::FormatVoltage( float fV, int nChars )
{
	sprintf(tmp, "%f", fV );
	_ASSERT( strlen(tmp) < 15 );
	int nLen = strlen(tmp);
	while ( nLen > nChars-1 )
		tmp[--nLen] = 0;
	tmp[nLen++] = ' ';
	tmp[nLen++] = 'V';
	tmp[nLen] = 0;
	return tmp;
}
	
/*static*/ char* CUtils::FormatFrequency( float fF, int nChars )
{
	char* strUnits = (char*)" Hz";
	if (fF >= 1000)
	{
		strUnits = (char*)" kHz";
		fF *= 0.001f;
	}
	if (fF >= 1000)
	{
		strUnits = (char*)" MHz";
		fF *= 0.001f;
	}
	sprintf( tmp, "%f", fF );
	int nLen = strlen(tmp);
	int nLenUnits = strlen(strUnits);
	while ( nLen + nLenUnits > nChars )
		tmp[--nLen] = 0;
	if ( tmp[nLen-1] == '.' )
		tmp[--nLen] = 0;
	strcat( tmp, strUnits );
	return tmp;
}

/*static*/ char* CUtils::FormatTime( float fT, int nChars )
{
	char* strUnits = (char*)" s";

	if (fT < 0.001f)
	{
		strUnits = (char*)" \xe6s";
		fT *= 1000.0f;
	} else
	if (fT < 1.0f)
	{
		strUnits = (char*)" ms";
		fT *= 1000.0f;
	} 

	sprintf( tmp, "%f", fT );
	int nLen = strlen(tmp);
	int nLenUnits = strlen(strUnits);
	while ( nLen + nLenUnits > nChars )
		tmp[--nLen] = 0;
	if ( tmp[nLen-1] == '.' )
		tmp[--nLen] = 0;
	strcat( tmp, strUnits );
	return tmp;
}

/*static*/ char* CUtils::ftoa(float f)
{
	sprintf( tmp, "%f", f );
	return tmp;
}

/*static*/ char* CUtils::FormatFloat5( float f )
{
	tmp[0] = ( f < 0.0f ) ? '-' : ' ';
	if ( f < 0 )
		f = -f;

//	_ASSERT( f < 10.0f );
	f += 0.0005f;
	sprintf( tmp+1, "%d", (int)f );
	f -= (int)f;
	f *= 1000.0f;
	sprintf( tmp+strlen(tmp), ".%03d", (int)f );
	tmp[6] = 0; 
	return tmp;
}
	
/*static*/ char* CUtils::MidiNote(int n)
{
	const static char notes[] = "C-" "C#" "D-" "D#" "E-" "F-" "F#" "G-" "G#" "A-" "A#" "B-";
	if ( n < 12 || n >= 78 )
	{
		tmp[0] = '?';
		tmp[1] = 0;
		return tmp;
	}
	int nOctave = (n / 12)-1;
	int nNote = n % 12;
	tmp[0] = notes[nNote*2];
	tmp[1] = notes[nNote*2+1];
	tmp[2] = '0' + nOctave;
	tmp[3] = 0;
	return tmp;
}
#endif
ui16 CUtils::InterpolateColor( ui16 clrA, ui16 clrB, int nLevel )
{
	int br = Get565R(clrB)*nLevel;
	int bg = Get565G(clrB)*nLevel;
	int bb = Get565B(clrB)*nLevel;
	nLevel = 255-nLevel;
	int ar = Get565R(clrA)*nLevel;
	int ag = Get565G(clrA)*nLevel;
	int ab = Get565B(clrA)*nLevel;
	ar = (ar+br) / 256;
	ag = (ag+bg) / 256;
	ab = (ab+bb) / 256;
	return RGB565RGB(ar, ag, ab);
}

int CUtils::Sqrt(int a)
{
	int ret=0;
	int s;
	int ret_sq=-a-1;
	for(s=30; s>=0; s-=2){
		int b;
		ret+= ret;
		b=ret_sq + ((2*ret+1)<<s);
		if(b<0){
			ret_sq=b;
			ret++;
		}
	}
	return ret;
}
