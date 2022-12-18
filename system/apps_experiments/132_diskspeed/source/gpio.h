namespace Gpio
{
#define PERIPH_BASE           ((uint32_t)0x40000000)
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
#define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)

enum {P1=10, P1alt = 15,
      P2=11, P2alt = 14,
      P3=8,  P3alt = 13,
      P4=9,  P4alt = 12,
      BASEB = GPIOB_BASE};

enum {
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
};


void SetState(uint32_t nPortBase, int nPin, int nState)
{
   int nPinPos = nPin & 7;
   uint32_t dwMask = ~(0xf << (nPinPos*4));
   uint32_t dwBits = nState << (nPinPos*4);

  if (nPin < 8)
  {
    uint32_t* pCRL = (uint32_t*)nPortBase;
    *pCRL = (*pCRL & dwMask) | dwBits;
  } else
  {
    uint32_t* pCRH = (uint32_t*)(nPortBase + 4);
    *pCRH = (*pCRH & dwMask) | dwBits;
  }
}

void SetLevel(uint32_t nPortBase, int nPin, bool value)
{
/*
    uint32_t* pODR = &((uint32_t*)nPortBase)[3];
  if (value)
  {
      *pODR |= 1<<nPin;
  } else
  {
      *pODR &= ~(1<<nPin);
  }
*/
  (*(uint16_t*)0x42218184) = value;
}

bool GetLevel(uint32_t nPortBase, int nPin)
{
  return (((uint32_t*)nPortBase)[2] >> nPin) & 1;
}

#undef PERIPH_BASE
#undef APB2PERIPH_BASE
#undef GPIOB_BASE
}

