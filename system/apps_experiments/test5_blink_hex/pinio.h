// Imported code from DS203 gabonators firmware
#include <stdint.h>
#include <stdbool.h>

typedef uint32_t ui32;
typedef uint32_t u32;


enum {
  // nPort
  PortA = 0,
  PortB = 1,
  PortC = 2,
  PortD = 3,

  // nReg
  RegCrl = 0,
  RegCrh = 1,
  RegIdr = 2,
  RegOdr = 3,
  RegBsrr = 4,
  RegBrr = 5,
  RegLckr = 6,
  
  // nState
  StateInput = 0,           // ..00
  StateOutput10Mhz = 1,     // ..01
  StateOutput2Mhz = 2,      // ..10
  StateOutput50Mhz = 3,     // ..11

  StateInputAnalog = 0<<2,
  StateInputFloating = 1<<2,
  StateInputPull = 2<<2,

  StateOutputPushPull = 0<<2,
  StateOutputOpenDrain = 1<<2,
  StateOutputFunctionPushPull = 2<<2,
  StateOutputFunctionOpenDrain = 3<<2,
  
  // macros for easy use
  StateSimpleInput = StateInput | StateInputFloating,
  StateSimpleOutput = StateOutput2Mhz | StateOutputPushPull
};


#define PERIPH_BASE           ((u32)0x40000000)
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
#define GPIOA_BASE            (APB2PERIPH_BASE + 0x0800)
#define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)
#define GPIOC_BASE            (APB2PERIPH_BASE + 0x1000)
#define GPIOD_BASE            (APB2PERIPH_BASE + 0x1400)
#define GPIOE_BASE            (APB2PERIPH_BASE + 0x1800)
#define GPIOF_BASE            (APB2PERIPH_BASE + 0x1C00)
#define GPIOG_BASE            (APB2PERIPH_BASE + 0x2000)

const static ui32 arrGpioBase[] = {GPIOA_BASE, GPIOB_BASE, GPIOC_BASE, GPIOD_BASE, GPIOE_BASE, GPIOF_BASE, GPIOG_BASE};

ui32* GetRegister(int nPort, int nReg)
{
  ui32 dwAddr = arrGpioBase[nPort];

  dwAddr |= nReg*4; 
  return (ui32*)dwAddr;
}

void SetState(int nPort, int nPin, int nState)
{
  ui32 dwAddr;

  if ( nPin < 8 )
    dwAddr = arrGpioBase[nPort] + 0;	// CRL
  else
  {
    dwAddr = arrGpioBase[nPort] + 4;	// CRH
    nPin &= 7;
  }

  ui32 dwMask = ~(0xf << (nPin*4));
  ui32 dwBits = nState << (nPin*4);

  ui32* pCR = ((ui32*)dwAddr);
  *pCR = (*pCR & dwMask) | dwBits;
}

void SetPin(int nPort, int nPin, bool bValue)
{
  ui32 dwAddr = arrGpioBase[nPort];
  if ( bValue )
  {
    ui32* pBSRR = &((ui32*)dwAddr)[4];
    *pBSRR = 1<<nPin;
  } else
  {
    ui32* pBRR = &((ui32*)dwAddr)[5];
    *pBRR = 1<<nPin;
  }
}

bool GetPin(int nPort, int nPin)
{
  ui32 dwAddr = arrGpioBase[nPort];
  ui32 dwIDR = ((ui32*)dwAddr)[2];
  return (dwIDR & (1<<nPin)) ? 1 : 0;
}

void pinWrite(int nPort, int nPin, int nValue)
{
  SetState(nPort, nPin, StateSimpleOutput);
  SetPin(nPort, nPin, nValue);
}