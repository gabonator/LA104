#include "platform.h"
#include "DS213HwDriver.h"
#include "fpga.h"

extern HwDrvDef  Hw;

namespace BIOS
{
  namespace ADC
  {
    bool bInit = false;
    bool bEnabled = false;
    bool bRestart = true;
    bool bEmpty = false;
    int nReadOffset = 0;

    bool Enabled()
    {
      return bEnabled;
    }

    BIOS::ADC::EState GetState() 
    {
      if (!bEnabled)
        return BIOS::ADC::EState::Offline;

      constexpr int CH_A = 0;
      constexpr int CH_B = 1;

      u32 Status = FPGA_CtrlRW(CH_A, SMPL_ST) | FPGA_CtrlRW(CH_B, SMPL_ST);

//      if ((Status & TRIGG) && (Status & SCRNF) )
//        return BIOS::ADC::EState::TriggeredFull; // TODO: verify full?

      if (Status & TRIGG)
        return BIOS::ADC::EState::Triggered; // TODO: verify full?

      if (Status & SCRNF) // Screen full?
        return BIOS::ADC::EState::Online; // ready to transfer

      return BIOS::ADC::EState::Busy;
    }

    void Restart(int offset) 
    {
      nReadOffset = offset;

      FPGA_DataWr(A_C_CH, SMPL_MODE, SEPARATE); 
      FPGA_DataWr(B_D_CH, SMPL_MODE, SEPARATE);
      FPGA_DataWr(A_C_CH, SMPL_PSMP, 150);
      FPGA_DataWr(B_D_CH, SMPL_PSMP, 150);
      *Hw.pOut_Clr = 1, *Hw.pOut_Clr = 0;        // FPGA 
      bRestart = true;
    }

    BIOS::ADC::TSample::SampleType Get() 
    {
      if (bRestart)
      {
        bRestart = false;
        bEmpty = false;
        FPGA_DataWr(A_C_CH, SMPL_RPTR, nReadOffset);
        FPGA_DataWr(B_D_CH, SMPL_RPTR, nReadOffset);
      }

      int sampleAC = FPGA_SmplRd(A_C_CH);
      int sampleBD = FPGA_SmplRd(B_D_CH);
      if ((sampleAC | sampleBD) & EMPTY)
        bEmpty = true;                              

      if (bEmpty)
        return ADC::NoData;

      uint32_t result = (sampleAC >> 8) | (sampleBD & 0xff00);

      if (sampleAC & 1)
        result |= 0x010000;
      if (sampleBD & 1)
        result |= 0x020000;
      return result;
    }

    int GetPointer() 
    { 
      _ASSERT(0);
      return 0;
    }

    void _Copy()
    {
      FPGA_DataWr(A_C_CH, SMPL_MODE, SEPARATE); 
      FPGA_DataWr(B_D_CH, SMPL_MODE, SEPARATE);
      FPGA_DataWr(A_C_CH, SMPL_PSMP, 150);
      FPGA_DataWr(B_D_CH, SMPL_PSMP, 150);
      *Hw.pOut_Clr = 1, *Hw.pOut_Clr = 0;        // FPGA 


      FPGA_DataWr(A_C_CH, SMPL_RPTR, 0);
      FPGA_DataWr(B_D_CH, SMPL_RPTR, 0);

      FPGA_SmplRd(A_C_CH);
      FPGA_SmplRd(B_D_CH);

      FPGA_DataWr(A_C_CH, SMPL_MODE, SEPARATE); 
      FPGA_DataWr(B_D_CH, SMPL_MODE, SEPARATE);
      FPGA_DataWr(A_C_CH, SMPL_PSMP, 150);
      FPGA_DataWr(B_D_CH, SMPL_PSMP, 150);
      *Hw.pOut_Clr = 1, *Hw.pOut_Clr = 0;        // FPGA 
    }

    bool Enable(bool bEnable) 
    {
      if (!bInit)
      {
        pFunc1 __BiosInit = (pFunc1)0x08000101; // Point to DFU BiosInit
        __BiosInit((uint32_t)&Hw);
        bInit = true;
      }

      bEnabled = bEnable;
      _ASSERT(bEnable);

      *Hw.pOut_nPD = 1;                                // 
      Hw.pDevInit(SGNL_IN);                            // 
      Hw.pDevInit(SGNLOUT);                            // 
      Hw.pDevInit(SO_ANLG);                            // 
      Hw.pDevInit(SO_DGTL);                            // 

      FPGA_CtrlRW(A_C_CH, CHIP_RST);                   // FPGA #0
      FPGA_CtrlRW(B_D_CH, CHIP_RST);                   // FPGA #1

      FPGA_DataWr(A_C_CH, SMPL_MODE, SEPARATE);  // 
      FPGA_DataWr(B_D_CH, SMPL_MODE, SEPARATE);
      FPGA_DataWr(A_C_CH, SMPL_PSMP, 150);
      FPGA_DataWr(B_D_CH, SMPL_PSMP, 150);

      // TODO: Strange startup sequence
      FPGA_ByteWr(A_C_CH, TRIG_VOLT, 128);
      FPGA_ByteWr(B_D_CH, TRIG_VOLT, 128);

      FPGA_ByteWr(A_C_CH, TRIG_KIND, TRIG_NONE);
      FPGA_ByteWr(B_D_CH, TRIG_KIND, TRIG_NONE);

      *Hw.pOut_Clr = 1, *Hw.pOut_Clr = 0;
      for (int i=0; i<400; i++) { FPGA_SmplRd(A_C_CH); FPGA_SmplRd(B_D_CH); }

      *Hw.pOut_Clr = 1, *Hw.pOut_Clr = 0;
      for (int i=0; i<400; i++) { FPGA_SmplRd(A_C_CH); FPGA_SmplRd(B_D_CH); }

      *Hw.pOut_Clr = 1, *Hw.pOut_Clr = 0;
      for (int i=0; i<400; i++) { FPGA_SmplRd(A_C_CH); FPGA_SmplRd(B_D_CH); }

      return true;
    }

    void ConfigureInput(BIOS::ADC::EInput input, BIOS::ADC::ECouple couple, BIOS::ADC::EResolution res, int offset) 
    {
//    _50mV, _100mV, _200mV, _500mV, _1V, _2V, _5V, _10V
//    {"10mV","20mV","50mV","0.1V","0.2V","0.5V"," 1V "," 2V "," 5V ","!10V!"};
      static const uint8_t RANGE_SW[][10]= 
        {{ 1,    1,    1,    1,    1,    0,    0,    0,    0,    0,}, 
         { 0,    0,    1,    1,    0,    0,    0,    0,    1,    0,}, 
         { 0,    1,    1,    1,    0,    0,    1,    1,    1,    0,}, 
         { 1,    1,    1,    0,    0,    1,    1,    0,    0,    0,}};

      if (input == BIOS::ADC::EInput::CH1)
      {
        int Range = (int)res+2;
        *Hw.pCh_A_Posn = offset; // 0..1024
        *Hw.pOut_A_Coupl = couple == BIOS::ADC::ECouple::DC;
        *Hw.pOut_A_Level  = RANGE_SW[0][Range];
        *Hw.pOut_A_Slct0  = RANGE_SW[1][Range];
        *Hw.pOut_A_Slct1  = RANGE_SW[2][Range];
        *Hw.pOut_A_Slct2  = RANGE_SW[3][Range];
        *Hw.pOut_A_Slct_B = 0;
      }
      if (input == BIOS::ADC::EInput::CH2)
      {
        int Range = (int)res+2;
        *Hw.pCh_B_Posn = offset;
        *Hw.pOut_B_Coupl = couple == BIOS::ADC::ECouple::DC;
        *Hw.pOut_B_Level  = RANGE_SW[0][Range];
        *Hw.pOut_B_Slct0  = RANGE_SW[1][Range];
        *Hw.pOut_B_Slct1  = RANGE_SW[2][Range];
        *Hw.pOut_B_Slct2  = RANGE_SW[3][Range];
        *Hw.pOut_B_Slct_A = 0;
      }
    }

    void ConfigureTimebase(float timePerDiv) 
    {
	const static float arrTime[] = {
          1.0f, 500e-3f, 200e-3f, 100e-3f, 50e-3f, 20e-3f,
          10e-3f, 5e-3f, 2e-3f, 1e-3f, 500e-6f, 200e-6f,
          100e-6f, 50e-6f, 20e-6f, 10e-6f, 5e-6f, 2e-6f,
          1e-6f, 500e-9f, 200e-9f, 100e-9f};
        const static int BASE_PSC[]  = {  
          2000-1,   2000-1,    800-1,    400-1,    200-1,     80-1, 
            40-1,     20-1,     10-1,      5-1,      2-1,      1-1,
             1-1,      1-1,      1-1,      1-1,      1-1,      1-1,
             1-1,      1-1,      1-1,      1-1,                   };
        const static int BASE_ARR[]  = {  
          1667-1,    833-1,    833-1,    833-1,    833-1,    833-1, 
           833-1,    833-1,    667-1,    667-1,    833-1,    667-1,
           333-1,    167-1,     67-1,     34-1,     17-1,      7-1,
             4-1,      2-1,      2-1,      1-1,                   };

      for (int i=0; i<COUNT(arrTime); i++)
      {
        float base = arrTime[i];
        if (base * 0.9f <= timePerDiv && timePerDiv <= base * 1.1f)
        {
          FPGA_DataWr(A_C_CH, SMPL_PSCL, BASE_PSC[i]);
          FPGA_DataWr(B_D_CH, SMPL_PSCL, BASE_PSC[i]);
          FPGA_DataWr(A_C_CH, SMPL_TDIV, BASE_ARR[i]);
          FPGA_DataWr(B_D_CH, SMPL_TDIV, BASE_ARR[i]);
          return;
        }
      }
      _ASSERT(0);
    }

    void ConfigureTrigger(int time, int value, BIOS::ADC::ETriggerType type, BIOS::ADC::EInput source)
    {
      int trigType = TRIG_ANY;
      switch (type)
      {
        case BIOS::ADC::ETriggerType::EdgeHL: trigType = (int)TRIG_AiDN; break;
        case BIOS::ADC::ETriggerType::EdgeLH: trigType = (int)TRIG_AiUP; break;
        case BIOS::ADC::ETriggerType::LevelLow: trigType = (int)TRIG_AiLT; break;
        case BIOS::ADC::ETriggerType::LevelHigh: trigType = (int)TRIG_AiGT; break;
        case BIOS::ADC::ETriggerType::LowerDTLow: trigType = (int)TRIG_AnLT; break;
        case BIOS::ADC::ETriggerType::GreaterDTLow: trigType = (int)TRIG_AnGT; break;
        case BIOS::ADC::ETriggerType::LowerDTHigh: trigType = (int)TRIG_ApLT; break;
        case BIOS::ADC::ETriggerType::GreaterDTHigh: trigType = (int)TRIG_ApGT; break;
        case BIOS::ADC::ETriggerType::None: trigType = (int)TRIG_ANY; break;
      }
 
      if (source == BIOS::ADC::EInput::CH1)
      {
        FPGA_ByteWr(A_C_CH, TRIG_KIND, trigType);
        FPGA_ByteWr(B_D_CH, TRIG_KIND, TRIG_NONE);
        FPGA_ByteWr(A_C_CH, TRIG_VOLT, value/4);
        FPGA_ByteWr(A_C_CH, TRIG_WDTH, time);
      }
      if (source == BIOS::ADC::EInput::CH2)
      {
        FPGA_ByteWr(A_C_CH, TRIG_KIND, TRIG_NONE);
        FPGA_ByteWr(B_D_CH, TRIG_KIND, trigType);
        FPGA_ByteWr(B_D_CH, TRIG_VOLT, value/4);
        FPGA_ByteWr(B_D_CH, TRIG_WDTH, time);
      }
    }
  }
}
