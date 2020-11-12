namespace gpio
{
#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
#define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)

enum {BASEP1= GPIOB_BASE, BASEP2=GPIOB_BASE, P1=10, P2=11};
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


template<uint32_t nPortBase, int nPin, int nState>
void SetGpioState()
{
  constexpr int nPinPos = nPin & 7;
  constexpr uint32_t dwMask = ~(0xf << (nPinPos*4));
  constexpr uint32_t dwBits = nState << (nPinPos*4);

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

template<uint32_t nPortBase, int nPin, bool value>
void SetGpioLevel()
{
  if (value)
  {
    uint32_t* pBSRR = &((uint32_t*)nPortBase)[4];
    *pBSRR = 1<<nPin;
  } else
  {
    uint32_t* pBRR = &((uint32_t*)nPortBase)[5];
    *pBRR = 1<<nPin;
  }
}

}

namespace i2c 
{
// https://github.com/todbot/SoftI2CMaster/blob/master/SoftI2CMaster.cpp

using namespace gpio;

void init() 
{ 
  // exit i2c mode
  BIOS::GPIO::PinMode(BIOS::GPIO::EPin::P1, BIOS::GPIO::EMode::Input);
  BIOS::GPIO::PinMode(BIOS::GPIO::EPin::P2, BIOS::GPIO::EMode::Input);

  // configure pins
  SetGpioState<BASEP1, P1, StateOutput50Mhz | StateOutputPushPull>();
  SetGpioState<BASEP2, P2, StateOutput50Mhz | StateOutputPushPull>();
  SetGpioLevel<BASEP1, P1, true>();
  SetGpioLevel<BASEP2, P2, true>();
}

void nop() { __asm__("nop"); } // 115 FPS
void sda_hi() { SetGpioLevel<BASEP2, P2, true>(); }
void scl_hi() { SetGpioLevel<BASEP1, P1, true>(); nop(); }
void sda_lo() { SetGpioLevel<BASEP2, P2, false>(); }
void scl_lo() { SetGpioLevel<BASEP1, P1, false>(); nop(); }

void writebit( int c )
{
    if (c)
    {
        sda_hi();
        scl_hi();
        scl_lo();
    } else 
    {
        sda_lo();
        scl_hi();
        scl_lo();
    }
}

void readbit(void)
{
    sda_hi();
    scl_hi();
    scl_lo();
}

void start()
{
    sda_hi();
    scl_hi();
    sda_lo();
    scl_lo();
}

void write( int c )
{
    writebit( c & 128 );
    c <<= 1;
    writebit( c & 128 );
    c <<= 1;
    writebit( c & 128 );
    c <<= 1;
    writebit( c & 128 );
    c <<= 1;
    writebit( c & 128 );
    c <<= 1;
    writebit( c & 128 );
    c <<= 1;
    writebit( c & 128 );
    c <<= 1;
    writebit( c & 128 );
    readbit();
}

void stop()
{
    scl_hi();
    sda_hi();
}

void start(int address)
{
  start();
  write(address<<1);
}

}