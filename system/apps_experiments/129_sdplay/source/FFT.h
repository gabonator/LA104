#ifndef __FFT3_H__
#define __FFT3_H__
#include <stdint.h>
//#include <Source/HwLayer/Types.h>
typedef uint16_t ui16;
typedef uint32_t ui32;
class CFftTables
{
protected:
	static const ui16 arrHannWindow[512];
	static const short Sinewave512[512-512/4+1];
};

template <int N>
class CFft : CFftTables
{
public:
	void Forward(short* pafReal, short* pafImag);
	int Hann(int i);
	int Sqrt(int x);

private:
	int _logbits();
	int _cos(int i);
	int _sin(int i);
};

// Template implementation

template <int N>
int CFft<N>::Hann(int i)
{	
	i = i*1024/N;
	if (i < 512)
		return arrHannWindow[i];
  return arrHannWindow[1024-i-1];
}

template <int N>
int CFft<N>::Sqrt(int n)
{
    unsigned int c = 0x8000;
    unsigned int g = 0x8000;

    for(;;) {
        if(g*g > (ui32)n)
            g ^= c;
        c >>= 1;
        if(c == 0)
            return g;
        g |= c;
    }
	return 0;
}

template <int N>
void CFft<N>::Forward(short* fr, short* fi)
{
	long int mr = 0, nn, i, j, l, k, istep, n;
	short qr, qi, tr, ti, wr, wi, m;

	n = N;
	nn = n - 1;

	/* decimation in time - re-order data */
	for (m=1; m<=nn; ++m)
	{
		l = n;
		do
		{
			l >>= 1;
		} while (mr+l > nn);
		
		mr = (mr & (l-1)) + l;
		if (mr <= m) continue;
		
		tr = fr[m];
		fr[m] = fr[mr];
		fr[mr] = tr;
		ti = fi[m];
		fi[m] = fi[mr];
		fi[mr] = ti;
	}

	l = 1;
	k = _logbits()-1;
	
	while (l < n)
	{
		/*
		  fixed scaling, for proper normalization --
		  there will be log2(n) passes, so this results
		  in an overall factor of 1/n, distributed to
		  maximize arithmetic accuracy.

		  It may not be obvious, but the shift will be
		  performed on each data point exactly once,
		  during this pass.
		*/
		
		// Variables for multiplication code
		long int c;
		short b;
		
		istep = l << 1;
		for (m=0; m<l; ++m)
		{
			j = m << k;
			/* 0 <= j < N_WAVE/2 */
			wr = _cos(j);	//Sinewave512[j+N/4];
			wi = _sin(j);	//-Sinewave512[j];

			wr >>= 1;
			wi >>= 1;
			
			for (i=m; i<n; i+=istep)
			{
				j = i + l;
				
				// Multiplications unrolled to prevent overhead
				// for procedural calls (the stm32 has an ALU with
				// H/W divide and single cycle multiply):
				
				// tr = FIX_MPY(wr,fr[j]) - FIX_MPY(wi,fi[j]);
				c = ((long int)wr * (long int)fr[j]);
				c = c >> 14;
				b = c & 0x01;
				tr = (short)((c >> 1) + b);
				
				c = ((long int)wi * (long int)fi[j]);
				c = c >> 14;
				b = c & 0x01;
				tr = tr - (short)(((c >> 1) + b));
				
				// ti = FIX_MPY(wr,fi[j]) + FIX_MPY(wi,fr[j]);
				c = ((long int)wr * (long int)fi[j]);
				c = c >> 14;
				b = c & 0x01;
				ti = (short)((c >> 1) + b);
				
				c = ((long int)wi * (long int)fr[j]);
				c = c >> 14;
				b = c & 0x01;
				ti = ti + (short)((c >> 1) + b);
				
				qr = fr[i];
				qi = fi[i];
				qr >>= 1;
				qi >>= 1;

				fr[j] = qr - tr;
				fi[j] = qi - ti;
				fr[i] = qr + tr;
				fi[i] = qi + ti;
			}
		}
		
		--k;
		l = istep;
	}
}


#endif
