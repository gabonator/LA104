#define CH_A_OFFSET       0     // Aͨ
#define CH_B_OFFSET       1     // Bͨ
#define BACKLIGHT         2     // 
#define BEEP_VOLUME       3     // 
#define BETTERY_DT        4     // 
#define ADC_MODE          5     // ADC
#define FIFO_CLR          6     // FIFOָ
  #define R_PTR           0       // FIFO
  #define W_PTR           1       // FIFOд
#define T_BASE_PSC        7     // ʱ
#define T_BASE_ARR        8     // ʱ
#define CH_A_COUPLE       9     // Aͨ
  #define DC              0
  #define AC              1
#define CH_A_RANGE       10     // Aͨ
#define CH_B_COUPLE      11     // Bͨ
//#define DC              0
//#define AC              1
#define CH_B_RANGE       12     // Bͨ
#define ANALOG_ARR       13     // ģ
#define ANALOG_PTR       14     // ģ
#define ANALOG_CNT       15     // ÿ
#define DIGTAL_PSC       16     // 
#define DIGTAL_ARR       17     // 
#define DIGTAL_CCR       18     // 
#define KEY_IF_RST       19     // 
#define STANDBY          20     // 
#define FPGA_RST         31     // FPGA 

#define TRIGG_MODE       32+0  // 
#define V_THRESHOLD      32+1  // 
#define T_THRESHOLD      32+2  // 
#define ADC_CTRL         32+4  // ADC 
#define A_POSITION       32+5  // CH_A
#define B_POSITION       32+6  // CH_B
#define REG_ADDR         32+7  // 

#define UNCONDITION       0x20        // 
#define DN            0
#define EN            1
#define SEPARATE      0    // ADC
#define INTERLACE     1    // ADC

#define FPGA_OK                11   // FPGA 

#define FIFO_DIGIT             0    // 16bits FIFO digital data
#define FIFO_EMPTY             1    // FIFO empty flag: 1 = empty
#define FIFO_START             2    // FIFO start flag: 1 = start
#define FIFO_FULL              3    // FIFO full flag: 1 = Full

typedef uint8_t u8;
typedef uint32_t u32;

//extern "C" void __Set(u8, u32);
//extern "C" u32 __Get(u8, u32);
//extern "C" u32 __Read_FIFO();

extern "C"
{
extern void __Set(u8, u32);
extern u32 _Read_FIFO();
extern u32 __Get(u8, u32);
extern u32 __Read_FIFO();
}

int _binary_search(ui32* A, ui32 key, int imin, int imax)
{
  // continue searching while [imin,imax] is not empty
  while (imax >= imin)
    {
      /* calculate the midpoint for roughly equal partition */
      int imid = (imin + imax)/2;
 
      // determine which subarray to search
      if      (A[imid] <  key)
        // change min index to search upper subarray
        imin = imid + 1;
      else if (A[imid] > key )
        // change max index to search lower subarray
        imax = imid - 1;
      else
        // key found at index imid
        return imid;
    }
  // key not found
  return -1;
}


namespace BIOS
{
  namespace ADC
  {
    bool gEnabled{false};
    bool gInit{false};
    int gBeginOffset{0};
    int32_t gStarted{0};
    float gTimePerDiv{1.0f};

    BIOS::ADC::EState GetState() 
    {
      if (!gEnabled)
        return EState::Offline;

      if (__Get(FIFO_FULL, 0) & 1)
        return EState::Full;

      if (__Get(FIFO_EMPTY, 0) & 1)
        return EState::Busy;

      return EState::Online;
    }

    void Restart(int beginOffset) 
    {
	__Set(FIFO_CLR, W_PTR);
	gStarted = BIOS::SYS::GetTick();
	gBeginOffset = beginOffset;
    }

    BIOS::ADC::TSample::SampleType Get() 
    {
      while (gBeginOffset > 0)
      {
        gBeginOffset--;
        // TODO: optimize!
        __asm (
          "LDR     R1,  = 0x40011000\n"  //     ; GPIO  PORT_C   
          "MOVW    R2,  #0x20\n"         // ; H_L pin: GPIO_PORT_B_Bit5 
          "LDR     R3,  =0x64000000\n"   //  ; FIFO Port address
          "STR     R2,  [R1, #0x14]\n"   // ; 0 -> H_L
          "LDRH    R0,  [R3, #0]\n"      // ; FIFO Data -> R0
          "STR     R2,  [R1, #0x10]\n"   // ; 1 -> H_L
          "LDRH    R1,  [R3, #0]\n"      // ; FIFO Data -> R1  
        );
      }

      __asm (
        "LDR     R1,  = 0x40011000\n"  //     ; GPIO  PORT_C   
        "MOVW    R2,  #0x20\n"         // ; H_L pin: GPIO_PORT_B_Bit5 
        "LDR     R3,  =0x64000000\n"   //  ; FIFO Port address
        "STR     R2,  [R1, #0x14]\n"   // ; 0 -> H_L
        "LDRH    R0,  [R3, #0]\n"      // ; FIFO Data -> R0
        "LSL     R0,  R0,  #16\n"      //

        "STR     R2,  [R1, #0x10]\n"   // ; 1 -> H_L
        "LDRH    R1,  [R3, #0]\n"      // ; FIFO Data -> R1  
        "ADD     R0,  R0, R1\n"
        "BX LR\n"
      );

      return 0; 
    }

    int GetPointer() 
    { 
	int nTime = BIOS::SYS::GetTick() - gStarted;
	float fDivs = nTime / 1000.0f / gTimePerDiv;
	int nSamples = fDivs * 30.0f;
	if ( nSamples > BIOS::ADC::NumSamples )
		return BIOS::ADC::NumSamples;
	return nSamples;
    }

    bool Enable(bool bEnable) 
    {
      if (!gInit)
      {
        if (__Get(FPGA_OK, 0) == 0)
        {
          BIOS::DBG::Print("FPGA configuration error\n");
          _ASSERT(0);
          return false;
        }
        __Set(STANDBY, DN);
        gInit = true;
      }

      gEnabled = bEnable;
      __Set(ADC_CTRL, bEnable ? EN : DN );
      if (bEnable)
      {     
        __Set(ADC_MODE, SEPARATE);
      }
      return true;
    }

    bool Enabled()
    { 
      return gEnabled; 
    }

    void ConfigureInput(BIOS::ADC::EInput input, BIOS::ADC::ECouple couple, BIOS::ADC::EResolution res, int offset) 
    {
      switch (input)
      {
        case BIOS::ADC::EInput::CH1:
//          __Set(ADC_MODE, SEPARATE);

          __Set(CH_A_COUPLE, (int)couple);
          __Set(CH_A_RANGE,  (int)res);
          __Set(CH_A_OFFSET, offset/4);
          break;

        case BIOS::ADC::EInput::CH2:
//          __Set(ADC_MODE, SEPARATE);

          __Set(CH_B_COUPLE, (int)couple);
          __Set(CH_B_RANGE,  (int)res);
          __Set(CH_B_OFFSET, offset/4);
          break;

        default:
          _ASSERT(0);
      }

    }

    void ConfigureTimebase(float timePerDiv) 
    {
	const static ui16 arrPsc[] = 
		{ 1-1, 1-1, 1-1, 1-1, 1-1, 1-1, 1-1, 1-1, 1-1, 1-1, 1-1, 10-1, 10-1, 10-1, 100-1, 100-1, 
		100-1, 1000-1, 1000-1, 1000-1, 1000-1, 1000-1 };

	const static ui16 arrArr[] = 
		{ 1-1, 1-1, 2-1, 3-1, 5-1, 12-1, 24-1, 48-1, 120-1, 240-1, 480-1, 120-1, 240-1, 480-1,
		120-1, 240-1, 480-1, 120-1, 240-1, 480-1, 1200-1, 1000-1};

	// TODO: rewrite T intervals into frequency, get rid of float operations

	const static float arrTime[] =
		{100e-9f, 200e-9f, 500e-9f,
		1e-6f, 2e-6f, 5e-6f,
		10e-6f, 20e-6f, 50e-6f, 100e-6f, 200e-6f, 500e-6f,
		1e-3f, 2e-3f, 5e-3f,
		10e-3f, 20e-3f, 50e-3f, 100e-3f, 200e-3f, 500e-3f,
		1.0f};

/*
	const static uint32_t arrTimeInv[] =
		{(uint32_t)1/100e-9f, (uint32_t)1/200e-9f, (uint32_t)1/500e-9f,
		(uint32_t)1/1e-6f, (uint32_t)1/2e-6f, (uint32_t)1/5e-6f,
		(uint32_t)1/10e-6f, (uint32_t)1/20e-6f, (uint32_t)1/50e-6f, (uint32_t)1/100e-6f, (uint32_t)1/200e-6f, (uint32_t)1/500e-6f,
		(uint32_t)1/1e-3f, (uint32_t)1/2e-3f, (uint32_t)1/5e-3f,
		1/10e-3f, 1/20e-3f, 1/50e-3f, 1/100e-3f, 1/200e-3f, 1/500e-3f,
		1/1.0f};
*/

	// positive floats can be treated as dwords when compared
	void* pArray = (void*)arrTime; // prevent gcc error "strict alignment rules"
	void* pValue = (void*)&timePerDiv;
	int nI = _binary_search( (ui32*)pArray, *(ui32*)pValue, 0, (int)COUNT(arrTime)-1 );

	_ASSERT( nI >= 0 || nI < (int)COUNT(arrPsc) );
	if ( nI == -1 )
	{
		_ASSERT(0);
		return;
	}

	gTimePerDiv = timePerDiv;

	__Set(T_BASE_PSC, arrPsc[nI]);
	__Set(T_BASE_ARR, arrArr[nI]);
    }

    void ConfigureTrigger(int time, int value, BIOS::ADC::ETriggerType type, BIOS::ADC::EInput source)
    {
      if ( type == ETriggerType::None )
      {
        __Set(TRIGG_MODE, UNCONDITION);
        return;
      }
      __Set(T_THRESHOLD, time);  
      __Set(V_THRESHOLD, value/4);  
      __Set(TRIGG_MODE,  ((int)source << 3) | (int)type);
    }

    // Configure buffer length!!!
  }
}
