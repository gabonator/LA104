#pragma once
#include <Framework.h>

#define Settings (*CSettings::m_pInstance)

class CSettings : public CSerialize
{
public:
	static CSettings* m_pInstance;

public:
class AnalogChannel : public CSerialize
    {
    public:
        static const char* const ppszTextEnabled[];
        static const char* const ppszTextCoupling[];
        static const char* const ppszTextResolution[];
        static const char* const ppszTextResolution10[];
        static const char* const ppszTextResolution100[];
        static const char* const ppszTextResolution1000[];
        static const char* const* ppszTextResolutionByProbe[4];
        static const char* const ppszTextProbe[];

		enum eEnabled {_NO = 0, _YES = 1, _EnabledMax = _YES}
			Enabled;
		enum eCoupling {_AC = 0, _DC, _GND, _CouplingMax = _GND} 
			Coupling;
		enum eResolution {_50mV, _100mV, _200mV, _500mV, _1V, _2V, _5V, _10V, _ResolutionMax = _10V} 
			Resolution;
		enum eProbe { _1X = 0, _10X, _100X, _1000X, _ProbeMax = _1000X }
			Probe;

		static const float pfValueProbe[];
		static const float pfValueResolution[];
		

		ui16 u16Color;
		si16 u16Position;
		const char* pszName;
		const char* pszFullName;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Enabled) << _E(Coupling) << _E(Resolution) << _E(Probe) << u16Color << u16Position;
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Enabled) >> _E(Coupling) >> _E(Resolution) >> _E(Probe) >> u16Color >> u16Position;
			return *this;
		}
	};
	class DigitalChannel : public CSerialize
	{
	public:
		static const char* const ppszTextPolarity[];

		enum {_NO = 0, _YES = 1, _EnabledMax = _YES}
			Enabled;
		ui16 u16Color;
		ui16 u16Position;
		enum {_POS=0, _NEG, _PolarityMax = _NEG}
			Polarity;

		const char* pszName;
		const char* pszFullName;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Enabled) << _E(Polarity) << u16Color << u16Position;
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Enabled) >> _E(Polarity) >> u16Color >> u16Position;
			return *this;
		}
	};
	class TimeBase : public CSerialize
	{
	public:
		static const char* const ppszTextResolution[];
		enum EResolution { 
			/*_100ns,*/ _200ns, _500ns, 
			_1us, _2us, _5us,
			_10us, _20us, _50us, _100us, _200us, _500us,
			_1ms, _2ms, _5ms,
			_10ms, _20ms, _50ms, _100ms, _200ms, _500ms, 
			_1s, _ResolutionMax = _500ms }
			Resolution;
		enum {
			_1, _2, _4, _FULL}
			Range;

		static const float pfValueResolution[];
		static const int pfValueResolutionCorrection[]; // florian

		si16 Shift;
		int InvalidFirst;
		
		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Resolution) << _E(Range) << Shift; 
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Resolution) >> _E(Range) >> Shift; 
			return *this;
		}
	};

	class Trigger : public CSerialize
	{
	public:
		static const char* const ppszTextSync[];
		static const char* const ppszTextSource[];
		static const char* const ppszTextType[];
		enum ESync { _Auto, _Norm, _Single, _Scan, _None, _SyncMax = _None }
			Sync;
		enum { _EdgeHL, _EdgeLH, _LevelLow, _LevelHigh, _LowerDTLow, _GreaterDTLow, _LowerDTHigh, _GreaterDTHigh, _TypeMax = _GreaterDTHigh }
			Type;
		enum { _CH1, _CH2, _CH3, _CH4, _Math, _SourceMax = _Math }
			Source;
		enum { _Run, _Stop, _Wait, _Unsync, _StateMax = _Unsync }
			State;
		si16 nLevel;
		si16 nTime;
		si16 nHoldOff;
		si16 nPosition;
		ui32 nLastChange;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Sync) << _E(Type) << _E(Source) << _E(State) << nLevel << nTime << nHoldOff;
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Sync) >> _E(Type) >> _E(Source) >> _E(State) >> nLevel >> nTime >> nPosition;
			return *this;
		}
	};
	class Generator : public CSerialize
	{
	public:
		static const char* const ppszTextWave[];
		enum { _Dc = 0, _SinHq, _SinLq, _Tri, _Saw, _Square, _Cardiac, _Volatile, _WaveMax = _Volatile }
			Wave;
		int nPsc;
		int nArr;
		int nCcr;
		int nScale; // 65536 - full
		int nOffset; // 0..65536

		int nDuty; // wanted duty cycle, real duty can be calculated from arr/ccr
		int nSamples;
		float nFrequency; 
		float fAmplitude; // 65536 -> full

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Wave) << nPsc << nArr << nScale;
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Wave) >> nPsc >> nArr >> nScale;
			return *this;
		}
	};
	class Marker : public CSerialize
	{
	public:
		static const char* const ppszTextMode[];
		static const char* const ppszTextSource[];
		static const char* const ppszTextDisplay[];
		static const char* const ppszTextFind[];

		enum { _Time, _Voltage }
			Type; // not necessary to serialize, cannot be changed by user
		enum { _Off, _On, _Auto, _ModeMaxTime = _On, _ModeMaxVoltage = _Auto }
			Mode;
		enum { _CH1, _CH2, _SourceMax = _CH2 }
			Source;
		enum { _Raw, _Physical, _DisplayMax = _Physical }
			Display;
		enum EFind { _MinFind, _AvgFind, _MaxFind, _FindMax = _MaxFind };

		int nValue;
		ui16 u16Color;

		PCSTR strName;
		PCSTR strFullName;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Mode) << _E(Source) << _E(Display) << nValue;
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Mode) >> _E(Source) >> _E(Display) >> nValue;
			return *this;
		}
	};
	class Measure : public CSerialize {
	public:
		static const char* const ppszTextEnabled[];
		static const char* const ppszTextSource[];
		static const char* const ppszTextType[];
		static const char* const ppszTextSuffix[];
		static const char* const ppszTextRange[];

		enum { _Off, _On, _MaxEnabled = _On }
			Enabled;
		enum ESource { _CH1, _CH2, _Math, _MaxSource = _Math }
			Source; 
		enum { _Min, _Max, _Avg, _RectAvg, _Rms, _Vpp, _Freq, _Period, _Pwm, _DeltaTime, _Angle, _TimeH, _TimeL, _TimeRise, _TimeFall, _FormFactor, 
			_Sigma, _Dispersion, _Baud, _P, _Pk, _Q, _Qk, _S, _Sk, _MaxType = _Sk }
			Type;
		enum ERange { _View, _Selection, _All, _MaxRange = _All }
			Range;
		
		float fValue;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Enabled) << _E(Source) << _E(Type) << _E(Range);
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Enabled) >> _E(Source) >> _E(Type) >> _E(Range);
			return *this;
		}
	};
	class MathOperand : public CSerialize
	{
	public:
		static const char* const ppszTextType[];
		// = {"CH1 raw", "CH1", "CH2 raw", "CH2", "Constant"}
		enum { _Ch1Raw, _CH1Corrected, _Ch2Raw, _Ch2Corrected, _Constant, _Fx, _TypeMax = _Fx }
			Type;
		int nScale;
		int nConstant;
		PCSTR strName;
	//	ui16 uiColor;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Type) << nConstant << nScale;
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Type) >> nConstant >> nScale;
			return *this;
		}
	};
	class MathOperator : public CSerialize
	{
	public:
		static const char* const ppszTextType[];
		// = {"Off", "A", "B", "A+B+C", "A+B-C", "B-A+C"}
		enum { _Off, _A, _B, _C, _AplusBplusC, _AminusBplusC, _BminusAplusC, 
			_AgreaterBplusC, _AlessBplusC, _minAB, _maxAB, _FirAplusC, _FirAdivBplusC,
			_AmulBmulC, _TypeMax = _AmulBmulC }
			Type;
		ui16 uiColor;
		AnalogChannel::eResolution Resolution;
		si16 Position;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Type) << uiColor << _E(Resolution) << Position;
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Type) >> uiColor >> _E(Resolution) >> Position;
			return *this;
		}
	};
	class Display : public CSerialize 
	{
	public:
		static const char* const ppszTextAxes[];
		// = {"T-Y", "X-Y"};
		static const char* const ppszTextDraw[];
		// = {"Points", "Lines", "Fill"};
		static const char* const ppszTextAverage[];
		// = {"No", "Yes"};
		static const char* const ppszTextPersist[];
		// = {"No", "Yes"};
		static const char* const ppszTextGrid[];
		// = {"None", "Dots", "Lines"};
		static const char* const ppszTextAxis[];
		// = {"None", "Single", "Double"};

		enum { _TY, _XY, _YX, _AxesMax = _YX }
			Axes;
		enum { _Points, _Lines, _Fill, _DrawMax = _Fill }
			Draw;
		enum { _AvgNo, _AvgCh1, _AvgCh2, _AverageMax = _AvgCh2 }
			Average;
		enum { _PerNo, _PerYes, _PersistMax = _PerYes }
			Persist;
		enum { _GridNone, _GridDots, _GridLines, _GridMax = _GridLines }
			Grid;
		enum { _AxisNone, _AxisSingle, _AxisDouble, _AxisMax = _AxisDouble }
			Axis;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << _E(Axes) << _E(Draw) << _E(Average) << _E(Persist) << _E(Grid) << _E(Axis);
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> _E(Axes) >> _E(Draw) >> _E(Average) >> _E(Persist) >> _E(Grid) >> _E(Axis);
			return *this;
		}
	};
//TODO: remove
	class CRuntime : public CSerialize
	{
	public:
		static const char* const ppszTextBeepOnOff[];
		// = {"On", "Off"};
		static const char* const ppszTextShortcut[];
		// = {"Start/Stop Acquisition", "Screenshot"};

		enum { _On, _Off, _BeepMax = _Off }
			m_Beep;

		enum EShortcut { None = -1, StartStop = -2, Toolbox = -3, WaveManager = -4, Screenshot = -5, _ShortcutMax = Screenshot };

		int m_nShortcutCircle;
		int m_nShortcutTriangle;
		int m_nShortcutS1;
		int m_nShortcutS2;

		int m_nMenuItem;
		int m_nUptime;
		int m_nBacklight;
		int m_nVolume;
		int m_nStandby;
		
		FLOAT m_fTimeRes;
		FLOAT m_fCH1Res;
		FLOAT m_fCH2Res;

		int m_bUartTest;
		int m_bUartEcho;	// not saved in settings
		int m_bUartSdk;

		virtual CSerialize& operator <<( CStream& stream )
		{
			stream << m_nMenuItem << m_nUptime << m_nBacklight << m_nVolume << m_nStandby
				<< _E(m_Beep) << m_nShortcutCircle << m_nShortcutTriangle << m_nShortcutS1 << m_nShortcutS2
				<< m_bUartTest;
			return *this;
		}
		virtual CSerialize& operator >>( CStream& stream )
		{
			stream >> m_nMenuItem >> m_nUptime >> m_nBacklight >> m_nVolume >> m_nStandby
				>> _E(m_Beep) >> m_nShortcutCircle >> m_nShortcutTriangle >> m_nShortcutS1 >> m_nShortcutS2
				>> m_bUartTest;
			return *this;
		}
	};


public:
	CRuntime Runtime;

	AnalogChannel CH1;
	AnalogChannel CH2;
	DigitalChannel CH3;
	DigitalChannel CH4;
	TimeBase Time;
	Trigger Trig;
	Generator Gen;
	Marker		MarkT1;
	Marker		MarkT2;
	Marker		MarkY1;
	Marker		MarkY2;
	Measure		Meas[6];

	MathOperand MathA;
	MathOperand MathB;
	MathOperand MathC;
	MathOperator Math;

	Display	Disp;

public:

	#include "Framework/Calibration.h"
	Calibrator	CH1Calib;
	Calibrator	CH2Calib;
	
	virtual CSerialize& operator <<( CStream& stream )
	{
/*
		ui32 dwId = _VERSION;
		ui32 dwEnd = ToDword('E', 'N', 'D', 27);
		stream << dwId << Runtime << CH1 << CH2 << CH3 << CH4 << Time << Trig << Gen
			<< MarkT1 << MarkT2 << MarkY1 << MarkY2
			<< Meas[0] << Meas[1] << Meas[2] << Meas[3] << Meas[4] << Meas[5]
			<< MathA << MathB << MathC << Math
			<< Spec
			<< dwEnd;
*/
		return *this;
	}

	virtual CSerialize& operator >>( CStream& stream )
	{
/*
		ui32 dwId = 0, dwEnd = 0;
		
		stream >> dwId;
		//_ASSERT( dwId == _VERSION );
		if ( dwId == _VERSION )
		{

			stream >> Runtime >> CH1 >> CH2 >> CH3 >> CH4 >> Time >> Trig >> Gen
				>> MarkT1 >> MarkT2 >> MarkY1 >> MarkY2
				>> Meas[0] >> Meas[1] >> Meas[2] >> Meas[3] >> Meas[4] >> Meas[5]
				>> MathA >> MathB >> MathC >> Math
				>> Spec
				>> dwEnd;
			// Dont forget to change version when the serialization order was changed!
			_ASSERT( dwEnd == ToDword('E', 'N', 'D', 27) );
			if ( dwEnd != ToDword('E', 'N', 'D', 27) )
			{
				Reset();
			}
		} else
			Reset();
*/
		return *this;
	}

	void Reset();
	void ResetCalibration();
	
	CSettings()
 	{
		m_pInstance = this;
		Reset();
	}
};
