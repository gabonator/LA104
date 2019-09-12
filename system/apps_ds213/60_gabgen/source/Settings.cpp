#include "Settings.h"    

CSettings* CSettings::m_pInstance = NULL;

/*static*/ const char* const CSettings::AnalogChannel::ppszTextEnabled[] = 
		{"No", "Yes"};
/*static*/ const char* const CSettings::AnalogChannel::ppszTextCoupling[] = 
		{"AC", "DC", "GND"};
/*static*/ const char* const CSettings::AnalogChannel::ppszTextResolution[] =
        {"50mV", "100mV", "200mV", "500mV", "1V", "2V", "5V", "10V"};
/*static*/ const char* const CSettings::AnalogChannel::ppszTextResolution10[] =
        {"500mV", "1V", "2V", "5V", "10V", "20V", "50V", "100V"};
/*static*/ const char* const CSettings::AnalogChannel::ppszTextResolution100[] =
        {"5V", "10V", "20V", "50V", "100V", "200V", "500V", "1000V"};
/*static*/ const char* const CSettings::AnalogChannel::ppszTextResolution1000[] =
        {"50V", "100V", "200V", "500V", "1000V", "2000V", "5000V", "10000V"};
/*static*/ const char* const* CSettings::AnalogChannel::ppszTextResolutionByProbe[4] = 
        {CSettings::AnalogChannel::ppszTextResolution, CSettings::AnalogChannel::ppszTextResolution10, CSettings::AnalogChannel::ppszTextResolution100, CSettings::AnalogChannel::ppszTextResolution1000};
/*static*/ const float CSettings::AnalogChannel::pfValueResolution[] =
		{0.050f, 0.100f, 0.200f, 0.500f, 1.0f, 2.0f, 5.0f, 10.0f};
/*static*/ const char* const CSettings::AnalogChannel::ppszTextProbe[] =
		{"1 x", "10 x", "100 x", "1000 x"};
/*static*/ const float CSettings::AnalogChannel::pfValueProbe[] =
		{1.0f, 10.0f, 100.0f, 1000.0f};
/*static*/ const char* const CSettings::DigitalChannel::ppszTextPolarity[] =
		{"Positive", "Negative"};
/*static*/ const char* const CSettings::TimeBase::ppszTextResolution[] =
		{/*"100ns",*/ "200ns", "500ns", 
		"1us", "2us", "5us",
		"10us", "20us", "50us", "100us", "200us", "500us",
		"1ms", "2ms", "5ms",
		"10ms", "20ms", "50ms", "100ms", "200ms", "500ms", 
		"1s" };
		
/*static*/ const int CSettings::TimeBase::pfValueResolutionCorrection[] = // florian
		{/*246,*/ 492, 614,
		819, 983, 1024,
		1024, 1024, 1024, 1024, 1024, 1024,
		1024, 1024, 1024,
		1024, 1024, 1024, 1024, 1024, 1024,
		1024};
		
/*static*/ const float CSettings::TimeBase::pfValueResolution[] =
		{/*100e-9f,*/ 200e-9f, 500e-9f,
		/* 97.65e-9f, 417e-9f, */
		1e-6f, 2e-6f, 5e-6f, 
		/*8e-7f, 1.934e-6f, 5e-6f,*/
		10e-6f, 20e-6f, 50e-6f, 100e-6f, 200e-6f, 500e-6f,
		1e-3f, 2e-3f, 5e-3f,
		10e-3f, 20e-3f, 50e-3f, 100e-3f, 200e-3f, 500e-3f,
		1.0f};
		
/*static*/ const char* const CSettings::Generator::ppszTextWave[] =
		{"DC", "Sin HQ", "Sin LQ", "Triangle", "Sawtooth", "Square", "Cardiac", "Volatile"};
/*static*/ const char* const CSettings::Trigger::ppszTextSync[] =
		{"Auto", "Norm", "Single", "Scan", "None"};
/*static*/ const char* const CSettings::Trigger::ppszTextSource[] =
		{"CH1", "CH2", "CH3", "CH4", "Math"};
/*static*/ const char* const CSettings::Trigger::ppszTextType[] =
		{"Falling", "Rising", "<Vt", ">Vt", "<TL", ">TL", "<TH", ">TH" };

/*static*/ const char* const CSettings::Marker::ppszTextMode[] =
		{"Off", "On", "Auto"};
/*static*/ const char* const CSettings::Marker::ppszTextSource[] =
		{ "CH1", "CH2" };
/*static*/ const char* const CSettings::Marker::ppszTextDisplay[] =
		{ "Raw", "Units" };
/*static*/ const char* const CSettings::Marker::ppszTextFind[] =
		{ "Minimum", "Average", "Maximum" };

/*static*/ const char* const CSettings::Measure::ppszTextEnabled[] =
		{ "Off", "On" };
/*static*/ const char* const CSettings::Measure::ppszTextSource[] =
		{ "CH1", "CH2", "Math" };
/*static*/ const char* const CSettings::Measure::ppszTextType[] =
		{ "Minimum", "Maximum", "Average", "RectAvg", "RMS", "Vpp", "Freq", "Period", "PWM %", "Delta+M", "Angle+M", "Time H", "Time L", 
		"Rising", "Falling", "FormFact", "Sigma", "Variance", "Baud", "P(W)+M", "P(kW)+M", "Q(VAr)+M", "Q(kVA)+M", "S(VA)+M", 
		"S(kVA)+M" };
/*static*/ const char* const CSettings::Measure::ppszTextSuffix[] =
		{ "V", "V", "V", "V", "V", "V", "kHz", "ms", "", "ms", "deg", "ms", "ms", "ms", "ms", "", "", "", "", "W", "kW", "VAr", 
		"kVAr", "VA", "kVA" };

/*static*/ const char* const CSettings::Measure::ppszTextRange[] =
		{ "View", "Selection", "All" };

/*static*/ const char* const CSettings::MathOperator::ppszTextType[] = 
		{"Off", "A", "B", "C", "A+B+C", "A-B+C", "B-A+C", "(A>B)+C", "(A<B)+C", "min(A,B)", "max(A,B)", "Fir(A)+C", "F(A)/B+C", "A*B*C" };
/*static*/ const char* const CSettings::MathOperand::ppszTextType[] = 
		{"CH1raw", "CH1", "CH2raw", "CH2", "Const", "Fx"};

/*static*/ const char* const CSettings::Display::ppszTextAxes[]
		 = {"T-Y", "X-Y", "Y-X"};
/*static*/ const char* const CSettings::Display::ppszTextDraw[]
		 = {"Dots", "Lines", "Fill"};
/*static*/ const char* const CSettings::Display::ppszTextAverage[]
		 = {"Off", "CH1", "CH2"};
/*static*/ const char* const CSettings::Display::ppszTextPersist[]
		 = {"No", "Yes"};
/*static*/ const char* const CSettings::Display::ppszTextGrid[]
		 = {"None", "Dots", "Lines"};
/*static*/ const char* const CSettings::Display::ppszTextAxis[]
		 = {"None", "Single", "Double"};
//
///*static*/ const char* const CSettings::Spectrum::ppszTextWindow[]
//		= {"Rect", "Hann"};
///*static*/ const char* const CSettings::Spectrum::ppszTextDisplay[]
//		= {"FFT", "FFT&Time", "Spectrog"};
///*static*/ const char* const CSettings::Spectrum::ppszTextScale[]
//		= {"Linear", "Log."};
///*static*/ const char* const CSettings::Spectrum::ppszTextSource[]
//		= {"Off", "CH1", "CH2"};
///*static*/ const char* const CSettings::Spectrum::ppszTextMode[]
//		= {"Manual", "Find max"};

/*static*/ const char* const CSettings::CRuntime::ppszTextBeepOnOff[]
		= {"On", "Off"};
/*static*/ const char* const CSettings::CRuntime::ppszTextShortcut[]
		= {"None", "Start/Stop Acquisition", "Toolbox", "Wave manager", "Screenshot"};



void CSettings::Reset()
{
	CH1.pszName = "CH1";
	CH1.pszFullName = "Analog input: CH1";
	CH1.Coupling = AnalogChannel::_DC;
	CH1.Resolution = AnalogChannel::_200mV;
	CH1.Probe = AnalogChannel::_1X;
	CH1.u16Color = RGB565(ffff00);
	CH1.u16Position = 55;
	CH1.Enabled = AnalogChannel::_YES;
	
	CH2.pszName = "CH2";
	CH2.pszFullName = "Analog input: CH2";
	CH2.Coupling = AnalogChannel::_AC;
	CH2.Resolution = AnalogChannel::_50mV;
	CH2.Probe = AnalogChannel::_10X;
	CH2.u16Color = RGB565(00ffff);
	CH2.u16Position = 100;
	CH2.Enabled = AnalogChannel::_YES;
	
	CH3.pszName = "CH3";
	CH3.pszFullName = "Digital input: CH3";
	CH3.u16Color = RGB565(ff00ff);
	CH3.u16Position = 100;
	CH3.Polarity = DigitalChannel::_POS;
	CH3.Enabled = DigitalChannel::_NO;
	
	CH4.pszName = "CH4";
	CH4.pszFullName = "Digital input: CH4";
	CH4.u16Color = RGB565(00ff00);
	CH4.u16Position = 200;
	CH4.Polarity = DigitalChannel::_NEG;
	CH4.Enabled = DigitalChannel::_NO;
	
	Time.Resolution = TimeBase::_500us;
	Time.Range = TimeBase::_4;
	Time.InvalidFirst = 30;
	// first samples are some noise, cut them out, length matching one div
	Time.Shift = Time.InvalidFirst;
	
	Trig.Sync = Trigger::_Auto;
	Trig.Type = Trigger::_EdgeLH;
	Trig.Source = Trigger::_CH1;
	Trig.State = Trigger::_Run;
	Trig.nLevel = 128;
	Trig.nTime = 30*5;
	Trig.nHoldOff = 0;
	Trig.nPosition = 150;
	Trig.nLastChange = 0;
	
	MarkT1.Mode = Marker::_On;
	MarkT1.Source = Marker::_CH1;
	MarkT1.Display = Marker::_Physical;
	MarkT1.nValue = 100;
	MarkT1.u16Color = RGB565(707070);
	MarkT1.Type = Marker::_Time;
	MarkT1.strName = "X1";
	MarkT1.strFullName = "Cursor X1";
	
	MarkT2.Mode = Marker::_On;
	MarkT2.Source = Marker::_CH1;
	MarkT2.Display = Marker::_Physical;
	MarkT2.nValue = 200;
	MarkT2.u16Color = RGB565(d0d0d0);
	MarkT2.Type = Marker::_Time;
	MarkT2.strName = "X2";
	MarkT2.strFullName = "Cursor X2";
	
	MarkY1.Mode = Marker::_Auto;
	MarkY1.Source = Marker::_CH1;
	MarkY1.Display = Marker::_Physical;
	MarkY1.nValue = 200;
	MarkY1.u16Color = RGB565(707070);
	MarkY1.Type = Marker::_Voltage;
	MarkY1.strName = "Y1";
	MarkY1.strFullName = "Cursor Y1";
	
	MarkY2.Mode = Marker::_Auto;
	MarkY2.Source = Marker::_CH1;
	MarkY2.Display = Marker::_Physical;
	MarkY2.nValue = 200;
	MarkY2.u16Color = RGB565(d0d0d0);
	MarkY2.Type = Marker::_Voltage;
	MarkY2.strName = "Y2";
	MarkY2.strFullName = "Cursor Y2";
	
	Meas[0].Enabled = Measure::_On;
	Meas[0].Source = Measure::_CH1;
	Meas[0].Type = Measure::_Rms;
	Meas[0].Range = Measure::_View;
	Meas[0].fValue = 0;
	
	Meas[1].Enabled = Measure::_On;
	Meas[1].Source = Measure::_CH1;
	Meas[1].Type = Measure::_Vpp;
	Meas[1].Range = Measure::_View;
	Meas[1].fValue = 0;
	
	Meas[2].Enabled = Measure::_Off;
	Meas[2].Source = Measure::_CH1;
	Meas[2].Type = Measure::_Vpp;
	Meas[2].Range = Measure::_View;
	Meas[2].fValue = 0;
	
	Meas[3].Enabled = Measure::_On;
	Meas[3].Source = Measure::_CH2;
	Meas[3].Type = Measure::_Rms;
	Meas[3].Range = Measure::_View;
	Meas[3].fValue = 0;
	
	Meas[4].Enabled = Measure::_On;
	Meas[4].Source = Measure::_CH2;
	Meas[4].Type = Measure::_Vpp;
	Meas[4].Range = Measure::_View;
	Meas[4].fValue = 0;
	
	Meas[5].Enabled = Measure::_Off;
	Meas[5].Source = Measure::_CH2;
	Meas[5].Type = Measure::_Vpp;
	Meas[5].Range = Measure::_View;
	Meas[5].fValue = 0;
	
	MathA.nConstant = 0;
	MathA.nScale = 100;
	MathA.Type = MathOperand::_CH1Corrected;
	MathA.strName = "A:";
	//MathA.uiColor = RGB565(808080);
	
	MathB.nConstant = 0;
	MathB.nScale = 100;
	MathB.Type = MathOperand::_Ch2Corrected;
	MathB.strName = "B:";
	//MathB.uiColor = RGB565(808080);
	
	MathC.nConstant = 0;
	MathC.nScale = 100;
	MathC.Type = MathOperand::_Constant;
	MathC.strName = "C:";
	//MathC.uiColor = RGB565(808080);
	
	Math.Type = MathOperator::_AplusBplusC;
	Math.uiColor = RGB565(ff80ff);
	Math.Position = 127;
	Math.Resolution = CSettings::AnalogChannel::_1V;
	
	Disp.Axes = Display::_TY;
	Disp.Draw = Display::_Lines;
	Disp.Average = Display::_AvgNo;
	Disp.Persist = Display::_PerNo;
	Disp.Grid = Display::_GridDots;
	Disp.Axis= Display::_AxisSingle;
	
	
	Runtime.m_nMenuItem = -1;
	Runtime.m_nUptime = 0;
	Runtime.m_nBacklight = 50;
	Runtime.m_nVolume = 12;
	Runtime.m_Beep = CRuntime::_On;
	// cant use CWndToolBar::Find, MainWnd has been not initialized! (CMainWnd::m_pInstance = NULL)
	Runtime.m_nShortcutCircle = CRuntime::Toolbox;
	Runtime.m_nShortcutTriangle = CRuntime::None;//CToolbar::Find( );CRuntime::_StartStop;
	Runtime.m_nShortcutS1 = CRuntime::None; //CWndToolBar::Find( "Generator" );
	Runtime.m_nShortcutS2 = CRuntime::None; //CWndToolBar::Find( "Meter" );
	Runtime.m_nStandby = 5;
	
	Runtime.m_bUartTest = true;
	Runtime.m_bUartEcho = false;
	Runtime.m_bUartSdk = true;

	ResetCalibration();



	Gen.Wave = Generator::_SinHq;
	//Gen.nPsc = 180-1;
	//Gen.nArr = 592; //2000-1;
	Gen.nPsc = 180-1;
	Gen.nArr = 24;
	Gen.nCcr = (Gen.nArr+1)/2;
	Gen.nScale = 0x10000;
	Gen.nOffset = 0x8000;
	Gen.nDuty = 50;
}

void CSettings::ResetCalibration()
{
	// valid on range 128..4096
	//DacCalib.m_arrCurveIn[0] = 0.1765f;	DacCalib.m_arrCurveOut[0] = 256;
	//DacCalib.m_arrCurveIn[1] = 1.412f;	DacCalib.m_arrCurveOut[1] = 2048;
//	DacCalib.m_arrCurveIn[0] = 0.5f;	DacCalib.m_arrCurveOut[0] = 725;
//	DacCalib.m_arrCurveIn[1] = 1.5f;	DacCalib.m_arrCurveOut[1] = 2176;

	const static si16 defaultQin[] = {0, 256};
	const static si32 defaultQout[] = {0, -(256<<11)};
	const static si16 defaultKin[] = {0, 0, 0, 0, 0, 256};
	const static si32 defaultKout[] = {1<<11, 1<<11, 1<<11, 1<<11, 1<<11, 1<<11};

	for ( int i = 0; i <= AnalogChannel::_ResolutionMax; i++ )
	{
		memcpy( CH1Calib.CalData[i].m_arrCurveQin, defaultQin, sizeof(defaultQin) );
		memcpy( CH1Calib.CalData[i].m_arrCurveQout, defaultQout, sizeof(defaultQout) );
		memcpy( CH1Calib.CalData[i].m_arrCurveKin, defaultKin, sizeof(defaultKin) );
		memcpy( CH1Calib.CalData[i].m_arrCurveKout, defaultKout, sizeof(defaultKout) );

		memcpy( CH2Calib.CalData[i].m_arrCurveQin, defaultQin, sizeof(defaultQin) );
		memcpy( CH2Calib.CalData[i].m_arrCurveQout, defaultQout, sizeof(defaultQout) );
		memcpy( CH2Calib.CalData[i].m_arrCurveKin, defaultKin, sizeof(defaultKin) );
		memcpy( CH2Calib.CalData[i].m_arrCurveKout, defaultKout, sizeof(defaultKout) );
	}

	#define CONCAT2(x, y) x ## y
	#define CONCAT(x, y) CONCAT2(x, y)
	#define _COPY(type, target, ...) \
		const static type CONCAT(tmp,__LINE__)[] = __VA_ARGS__; \
		memcpy( target, CONCAT(tmp,__LINE__), sizeof(CONCAT(tmp,__LINE__)) );

//	_COPY( si16, CH1Calib.CalData[AnalogChannel::_200mV].m_arrCurveQin, {-20, 280} );
//	_COPY( si32, CH1Calib.CalData[AnalogChannel::_200mV].m_arrCurveQout, {9380*5, -154816*5} );
//	_COPY( si16, CH1Calib.CalData[AnalogChannel::_200mV].m_arrCurveKin, {-20, 15, 75, 90, 245, 280} );
//	_COPY( si32, CH1Calib.CalData[AnalogChannel::_200mV].m_arrCurveKout, {581*5, 580*5, 581*5, 582*5, 584*5, 580*5} );
}
