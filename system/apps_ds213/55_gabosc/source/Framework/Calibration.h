
	// input_storage_class, output_storage_class, input_variabe, output_variable, internal_calculations, length
	template <class Ttabin, class Ttabout, class Tvarin, class Tvarout, class Tcalc, int N_>
	class CalibrationCurve
	{
	public:
		Ttabin inp[N_];
		Ttabout out[N_];

		CalibrationCurve()
		{
			for ( int i=0; i<(int)COUNT(inp); i++ )
			{
				Tcalc t = (Tcalc)255*i/COUNT(inp);
				inp[i] = (Ttabin)t;
				out[i] = (Ttabout)t;
			}
		}

		CalibrationCurve(const Ttabin* tin, const Ttabout* tout)
		{
			for ( int i=0; i<(int)COUNT(inp); i++ )
			{
				inp[i] = tin[i];
				out[i] = tout[i];
			}
		}

		Tvarout Get( Tvarin i )
		{
			int b = 0, e = COUNT(inp)-1;
			while ( e-b > 1 )	
			{
				int m = (b+e) >> 1;
				if ( i >= inp[m] )
					b = m;
				else
					e = m;
			}
#ifdef _WIN32
			_ASSERT( b != e );
#endif
			Tcalc nTmp = (Tcalc)(i - inp[b]);
			nTmp *= out[e] - out[b];
			nTmp /= inp[e] - inp[b];
			nTmp += out[b];
			return (Tvarout)nTmp;
		}		

		static Tvarout Get( Ttabin* inp, Ttabout* out, Tvarin i )
		{
			int b = 0, e = N_-1;
			while ( e-b > 1 )	
			{
				int m = (b+e) >> 1;
				if ( i >= inp[m] )
					b = m;
				else
					e = m;
			}
#ifdef _WIN32
			_ASSERT( b != e );
			_ASSERT( inp[e] != inp[b] );
#endif
			Tcalc nTmp = (Tcalc)(i - inp[b]);
			nTmp *= out[e] - out[b];
			nTmp /= inp[e] - inp[b];
			nTmp += out[b];
			return (Tvarout)nTmp;
		}		
	};

	class LinCalibCurve
	{	
		public:
		enum {
			eQPoints = 2,
			eKPoints = 6
		};

		si16 m_arrCurveQin[eQPoints];
		si32 m_arrCurveQout[eQPoints];
		si16 m_arrCurveKin[eKPoints];
		si32 m_arrCurveKout[eKPoints];
	};

	class LinApprox
	{
		public:
		enum {
			ePoints = 2
		};

		float m_arrCurveIn[ePoints];
		si16 m_arrCurveOut[ePoints];
		typedef CalibrationCurve<float, si16, float, int, float, ePoints> Interpolator;

		int Get( float fVoltage )
		{
			return Interpolator::Get( m_arrCurveIn, m_arrCurveOut, fVoltage );
		}
	};

	class Calibrator
	{
	public:	
		LinCalibCurve CalData[AnalogChannel::_ResolutionMax+1];	
		typedef CalibrationCurve<si16, si32, int, int, int, LinCalibCurve::eKPoints> InterpolatorK;
		typedef CalibrationCurve<si16, si32, int, int, int, LinCalibCurve::eQPoints> InterpolatorQ;

		struct FastCalc 
		{
			float fMultiplier;
			int K, Q, Zero;
		};
#if 1
		// potrebujem vediet rozlisenie a vertikalnu polohu
		void Prepare(AnalogChannel* pSource, FastCalc& fast)
		{
			const static float arrMultipliers[AnalogChannel::_ResolutionMax+1] = 
				{50e-3f, 100e-3f, 200e-3f, 500e-3f, 1.0f, 2.0f, 5.0f, 10.0f};

			LinCalibCurve& pCurCurve = CalData[ pSource->Resolution ];
			si16& nVert = pSource->u16Position;

			fast.K = InterpolatorK::Get( pCurCurve.m_arrCurveKin, pCurCurve.m_arrCurveKout, nVert );
			fast.Q = InterpolatorQ::Get( pCurCurve.m_arrCurveQin, pCurCurve.m_arrCurveQout, nVert );
			fast.Zero = fast.Q / fast.K;
			fast.fMultiplier = arrMultipliers[pSource->Resolution];
		}

		float GetMultiplier(int nRes)
		{
			const static float arrMultipliers[AnalogChannel::_ResolutionMax+1] = 
				{50e-3f, 100e-3f, 200e-3f, 500e-3f, 1.0f, 2.0f, 5.0f, 10.0f};
			return (arrMultipliers[nRes]);
		}

		static void Prepare(int nRes, int nVert, LinCalibCurve& pCurCurve, FastCalc& fast)
		{
			const static float arrMultipliers[AnalogChannel::_ResolutionMax+1] = 
				{50e-3f, 100e-3f, 200e-3f, 500e-3f, 1.0f, 2.0f, 5.0f, 10.0f};

			fast.K = InterpolatorK::Get( pCurCurve.m_arrCurveKin, pCurCurve.m_arrCurveKout, nVert );
			fast.Q = InterpolatorQ::Get( pCurCurve.m_arrCurveQin, pCurCurve.m_arrCurveQout, nVert );
			fast.Zero = fast.Q / fast.K;
			fast.fMultiplier = arrMultipliers[nRes];
		}

		int Correct( FastCalc& fast, int nAdc )
		{
			//return -fast.Zero + ( ((fast.Q + nAdc * fast.K)*5) >> (16-5) ); // *32/200mV 5=1/200mV		
			// K, Q by mali byt v rozmere pixelov (0..255) reprezentujuce vysku od 0..8divov a nie vo voltoch!
			return -fast.Zero + ( (fast.Q + nAdc * fast.K) >> 11 );
		}

		float Voltage(FastCalc& fast, float fAdc)
		{	
			//return (fast.Q + fAdc * fast.K)/65536.0f; 
			// toto by malo vratit hodnotu 0..256 zodpovedajucu 0 az 8 divom
			// pre ziskanie skutocneho napatia teda vydelime osmimi a prenasobime konstantou
			return (fast.Q + fAdc * fast.K) * fast.fMultiplier / 65536.0f;
		}

		float Correct(FastCalc& fast, float fAdc)
		{
			float fZero = (float)fast.Q / fast.K;
  		//return -fZero + ( ((fast.Q + fAdc * fast.K)*5.0f) / 2048.0f ); // *32/200mV 5=1/200mV   2048 = 65536 {fixed 16.16} / 32 {1 div}
			return -fZero + ( (fast.Q + fAdc * fast.K) / 65536.0f / 32.0f );   

// *32/0.2
		}
#endif
		int GetZero(FastCalc& fast)
		{
			return -fast.Zero;
		}

#if 0
		void Prepare(AnalogChannel* pSource, FastCalc& fast)
		{
			const static float arrMultipliers[AnalogChannel::_ResolutionMax+1] = 
				{50e-3f, 100e-3f, 200e-3f, 500e-3f, 1.0f, 2.0f, 5.0f, 10.0f};

			LinCalibCurve& pCurCurve = CalData[ pSource->Resolution ];
			si16& nVert = pSource->u16Position;

			fast.K = InterpolatorK::Get( pCurCurve.m_arrCurveKin, pCurCurve.m_arrCurveKout, nVert );
			fast.Q = InterpolatorQ::Get( pCurCurve.m_arrCurveQin, pCurCurve.m_arrCurveQout, nVert );
			fast.Zero = fast.Q / 2048;
			fast.fMultiplier = arrMultipliers[pSource->Resolution];
		}
		static void Prepare(int nRes, int nVert, LinCalibCurve& pCurCurve, FastCalc& fast)
		{
			const static float arrMultipliers[AnalogChannel::_ResolutionMax+1] = 
				{50e-3f, 100e-3f, 200e-3f, 500e-3f, 1.0f, 2.0f, 5.0f, 10.0f};

			fast.K = InterpolatorK::Get( pCurCurve.m_arrCurveKin, pCurCurve.m_arrCurveKout, nVert );
			fast.Q = InterpolatorQ::Get( pCurCurve.m_arrCurveQin, pCurCurve.m_arrCurveQout, nVert );
			fast.Zero = fast.Q / 2048;
			fast.fMultiplier = arrMultipliers[nRes];
		}
		int Correct( FastCalc& fast, int nAdc )
		{
			return -fast.Zero + ( ( (fast.Zero + nAdc) * fast.K) >> 11 );
		}
		float Voltage(FastCalc& fast, float fAdc)
		{	
			return ( (fast.Q / 2048.0f + fAdc) * fast.K) * fast.fMultiplier / 65536.0f;
		}
		float Correct(FastCalc& fast, float fAdc)
		{
			float fZero = (float)fast.Q / 2048.0f;
			return -fZero + ( ((fZero + fAdc) * fast.K) / 65536.0f / 32.0f );   
		}
#endif

	};
