namespace Gpio {
#define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)
#define GPIOD_BASE            (APB2PERIPH_BASE + 0x1400)
#define GPIOE_BASE            (APB2PERIPH_BASE + 0x1800)

enum {P1=10, P1MOSI = 15,
      P2=11, P2MISO = 14,
      P3=8,  P3SCK2 = 13,
      P4=9,  P4CS = 12,
      BASEB = GPIOB_BASE, 
      BASED = GPIOD_BASE,
      //PINMISO = P1,
      //PINSCK = P2,
      //PINMOSI = P3,
      //PINCS = P4
     };

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
    //uint32_t* pBSRR = &((uint32_t*)nPortBase)[4];
    //*pBSRR = (value ? 1 : 0x10000) << nPin;
    uint32_t* pODR = &((uint32_t*)nPortBase)[3];
  if (value)
  {
      *pODR |= 1<<nPin;
    //uint32_t* pBSRR = &((uint32_t*)nPortBase)[4];
    //*pBSRR = 1<<nPin;
  } else
  {
      *pODR &= ~(1<<nPin);
    //uint32_t* pBRR = &((uint32_t*)nPortBase)[5];
    //*pBRR = 1<<nPin;
  }

                  //__asm__("nop"); __asm__("nop");

}

bool GetLevel(uint32_t nPortBase, int nPin)
{
 //                   __asm__("nop"); __asm__("nop");

  return (((uint32_t*)nPortBase)[2] >> nPin) & 1;
}

}
