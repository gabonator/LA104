#include "la104.h"

uint32_t (*__Bios)(uint8_t, uint32_t) = (uint32_t (*)(uint8_t, uint32_t))0x08000101;

enum
{
    SYSINFO, // ????????      Var: PRDT_SN  ???????  Rtn: (u16)???????
             //                   Var: PRODUCT  ??????  Rtn: ?????????
             //                   Var: PRDTNUM  ???????  Rtn: (u16)???????
             //                   Var: SCH_VER  ?????  Rtn: ????????
             //                   Var: MCU_TYP  MCU ???  Rtn: ?????????
             //                   Var: DFU_VER  DFU ??  Rtn: ????????
             //                   Var: OEM_NUM  OEM ????  Rtn: ??????????
             //                   Var: MANUFAC ???????? Rtn: ?????????
             //                   Var: LICENCE ??????? Rtn: SUCC/FAIL
             // ????????
};

enum
{
    PRDT_SN, //    8c33b088
    PRODUCT, //    LA104
    PRDTNUM, //    0104
    HDW_VER, //    1.5B
    MCU_TYP, //    STM32F103VC
    DFU_VER, //    V3.61D
    OEM_NUM, //    X
    MANUFAC, //    e-Design
    LICENCE, //    1
};

// implementation
char* CLA104::DfuVersion()
{
  return (char*)__Bios(SYSINFO, DFU_VER);
}

char* CLA104::HardwareVersion()
{
  return (char*)__Bios(SYSINFO, HDW_VER);
}

char* CLA104::SystemVersion()
{
  return (char*)__Bios(SYSINFO, PRODUCT);
}

char* CLA104::FpgaVersion()
{
  return nullptr;
}

char* CLA104::SerialNumber()
{                            
  static char serial[16];
  sprintf(serial, "%08x", __Bios(SYSINFO, PRDT_SN));
  return serial;
}

char* CLA104::DisplayType()
{
  return (char*)"ili9314";
}

char* CLA104::DiskType()
{
  return nullptr;
}
