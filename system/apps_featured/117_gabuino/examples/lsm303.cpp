#include <library.h>
#include <math.h>

typedef uint8_t byte;
#define PI 3.1415926535
#ifndef LSM303_h
#define LSM303_h

class LSM303
{
  public:
    template <typename T> struct vector
    {
      T x, y, z;
    };

    enum deviceType { device_DLH, device_DLM, device_DLHC, device_D, device_auto };
    enum sa0State { sa0_low, sa0_high, sa0_auto };

    // register addresses
    enum regAddr
    {
      TEMP_OUT_L        = 0x05, // D
      TEMP_OUT_H        = 0x06, // D

      STATUS_M          = 0x07, // D

      INT_CTRL_M        = 0x12, // D
      INT_SRC_M         = 0x13, // D
      INT_THS_L_M       = 0x14, // D
      INT_THS_H_M       = 0x15, // D

      OFFSET_X_L_M      = 0x16, // D
      OFFSET_X_H_M      = 0x17, // D
      OFFSET_Y_L_M      = 0x18, // D
      OFFSET_Y_H_M      = 0x19, // D
      OFFSET_Z_L_M      = 0x1A, // D
      OFFSET_Z_H_M      = 0x1B, // D
      REFERENCE_X       = 0x1C, // D
      REFERENCE_Y       = 0x1D, // D
      REFERENCE_Z       = 0x1E, // D

      CTRL0             = 0x1F, // D
      CTRL1             = 0x20, // D
      CTRL_REG1_A       = 0x20, // DLH, DLM, DLHC
      CTRL2             = 0x21, // D
      CTRL_REG2_A       = 0x21, // DLH, DLM, DLHC
      CTRL3             = 0x22, // D
      CTRL_REG3_A       = 0x22, // DLH, DLM, DLHC
      CTRL4             = 0x23, // D
      CTRL_REG4_A       = 0x23, // DLH, DLM, DLHC
      CTRL5             = 0x24, // D
      CTRL_REG5_A       = 0x24, // DLH, DLM, DLHC
      CTRL6             = 0x25, // D
      CTRL_REG6_A       = 0x25, // DLHC
      HP_FILTER_RESET_A = 0x25, // DLH, DLM
      CTRL7             = 0x26, // D
      REFERENCE_A       = 0x26, // DLH, DLM, DLHC
      STATUS_A          = 0x27, // D
      STATUS_REG_A      = 0x27, // DLH, DLM, DLHC

      OUT_X_L_A         = 0x28,
      OUT_X_H_A         = 0x29,
      OUT_Y_L_A         = 0x2A,
      OUT_Y_H_A         = 0x2B,
      OUT_Z_L_A         = 0x2C,
      OUT_Z_H_A         = 0x2D,

      FIFO_CTRL         = 0x2E, // D
      FIFO_CTRL_REG_A   = 0x2E, // DLHC
      FIFO_SRC          = 0x2F, // D
      FIFO_SRC_REG_A    = 0x2F, // DLHC

      IG_CFG1           = 0x30, // D
      INT1_CFG_A        = 0x30, // DLH, DLM, DLHC
      IG_SRC1           = 0x31, // D
      INT1_SRC_A        = 0x31, // DLH, DLM, DLHC
      IG_THS1           = 0x32, // D
      INT1_THS_A        = 0x32, // DLH, DLM, DLHC
      IG_DUR1           = 0x33, // D
      INT1_DURATION_A   = 0x33, // DLH, DLM, DLHC
      IG_CFG2           = 0x34, // D
      INT2_CFG_A        = 0x34, // DLH, DLM, DLHC
      IG_SRC2           = 0x35, // D
      INT2_SRC_A        = 0x35, // DLH, DLM, DLHC
      IG_THS2           = 0x36, // D
      INT2_THS_A        = 0x36, // DLH, DLM, DLHC
      IG_DUR2           = 0x37, // D
      INT2_DURATION_A   = 0x37, // DLH, DLM, DLHC

      CLICK_CFG         = 0x38, // D
      CLICK_CFG_A       = 0x38, // DLHC
      CLICK_SRC         = 0x39, // D
      CLICK_SRC_A       = 0x39, // DLHC
      CLICK_THS         = 0x3A, // D
      CLICK_THS_A       = 0x3A, // DLHC
      TIME_LIMIT        = 0x3B, // D
      TIME_LIMIT_A      = 0x3B, // DLHC
      TIME_LATENCY      = 0x3C, // D
      TIME_LATENCY_A    = 0x3C, // DLHC
      TIME_WINDOW       = 0x3D, // D
      TIME_WINDOW_A     = 0x3D, // DLHC

      Act_THS           = 0x3E, // D
      Act_DUR           = 0x3F, // D

      CRA_REG_M         = 0x00, // DLH, DLM, DLHC
      CRB_REG_M         = 0x01, // DLH, DLM, DLHC
      MR_REG_M          = 0x02, // DLH, DLM, DLHC

      SR_REG_M          = 0x09, // DLH, DLM, DLHC
      IRA_REG_M         = 0x0A, // DLH, DLM, DLHC
      IRB_REG_M         = 0x0B, // DLH, DLM, DLHC
      IRC_REG_M         = 0x0C, // DLH, DLM, DLHC

      WHO_AM_I          = 0x0F, // D
      WHO_AM_I_M        = 0x0F, // DLM

      TEMP_OUT_H_M      = 0x31, // DLHC
      TEMP_OUT_L_M      = 0x32, // DLHC


      // dummy addresses for registers in different locations on different devices;
      // the library translates these based on device type
      // value with sign flipped is used as index into translated_regs array

      OUT_X_H_M         = -1,
      OUT_X_L_M         = -2,
      OUT_Y_H_M         = -3,
      OUT_Y_L_M         = -4,
      OUT_Z_H_M         = -5,
      OUT_Z_L_M         = -6,
      // update dummy_reg_count if registers are added here!

      // device-specific register addresses

      DLH_OUT_X_H_M     = 0x03,
      DLH_OUT_X_L_M     = 0x04,
      DLH_OUT_Y_H_M     = 0x05,
      DLH_OUT_Y_L_M     = 0x06,
      DLH_OUT_Z_H_M     = 0x07,
      DLH_OUT_Z_L_M     = 0x08,

      DLM_OUT_X_H_M     = 0x03,
      DLM_OUT_X_L_M     = 0x04,
      DLM_OUT_Z_H_M     = 0x05,
      DLM_OUT_Z_L_M     = 0x06,
      DLM_OUT_Y_H_M     = 0x07,
      DLM_OUT_Y_L_M     = 0x08,

      DLHC_OUT_X_H_M    = 0x03,
      DLHC_OUT_X_L_M    = 0x04,
      DLHC_OUT_Z_H_M    = 0x05,
      DLHC_OUT_Z_L_M    = 0x06,
      DLHC_OUT_Y_H_M    = 0x07,
      DLHC_OUT_Y_L_M    = 0x08,

      D_OUT_X_L_M       = 0x08,
      D_OUT_X_H_M       = 0x09,
      D_OUT_Y_L_M       = 0x0A,
      D_OUT_Y_H_M       = 0x0B,
      D_OUT_Z_L_M       = 0x0C,
      D_OUT_Z_H_M       = 0x0D
    };

    vector<int16_t> a; // accelerometer readings
    vector<int16_t> m; // magnetometer readings
    vector<int16_t> m_max; // maximum magnetometer values, used for calibration
    vector<int16_t> m_min; // minimum magnetometer values, used for calibration

    byte last_status; // status of last I2C transmission

    LSM303(void);

    bool init(deviceType device = device_auto, sa0State sa0 = sa0_auto);
    deviceType getDeviceType(void) { return _device; }

    void enableDefault(void);

    void writeAccReg(byte reg, byte value);
    byte readAccReg(byte reg);
    void writeMagReg(byte reg, byte value);
    byte readMagReg(int reg);

    void writeReg(byte reg, byte value);
    byte readReg(int reg);

    void readAcc(void);
    void readMag(void);
    void read(void);

    void setTimeout(unsigned int timeout);
    unsigned int getTimeout(void);
    bool timeoutOccurred(void);

    float heading(void);
    template <typename T> float heading(vector<T> from);

    // vector functions
    template <typename Ta, typename Tb, typename To> static void vector_cross(const vector<Ta> *a, const vector<Tb> *b, vector<To> *out);
    template <typename Ta, typename Tb> static float vector_dot(const vector<Ta> *a, const vector<Tb> *b);
    static void vector_normalize(vector<float> *a);

  private:
    deviceType _device; // chip type (D, DLHC, DLM, or DLH)
    byte acc_address;
    byte mag_address;

    static const int dummy_reg_count = 6;
    regAddr translated_regs[dummy_reg_count + 1]; // index 0 not used

    unsigned int io_timeout;
    bool did_timeout;

    int testReg(byte address, regAddr reg);
};
#ifdef CALC
/*
Returns the angular difference in the horizontal plane between the
"from" vector and north, in degrees.
Description of heading algorithm:
Shift and scale the magnetic reading based on calibration data to find
the North vector. Use the acceleration readings to determine the Up
vector (gravity is measured as an upward acceleration). The cross
product of North and Up vectors is East. The vectors East and North
form a basis for the horizontal plane. The From vector is projected
into the horizontal plane and the angle between the projected vector
and horizontal north is returned.
*/
template <typename T> float LSM303::heading(vector<T> from)
{
    vector<int32_t> temp_m = {m.x, m.y, m.z};

    // subtract offset (average of min and max) from magnetometer readings
    temp_m.x -= ((int32_t)m_min.x + m_max.x) / 2;
    temp_m.y -= ((int32_t)m_min.y + m_max.y) / 2;
    temp_m.z -= ((int32_t)m_min.z + m_max.z) / 2;

    // compute E and N
    vector<float> E;
    vector<float> N;
    vector_cross(&temp_m, &a, &E);
    vector_normalize(&E);
    vector_cross(&a, &E, &N);
    vector_normalize(&N);

    // compute heading
    float heading = atan2(vector_dot(&E, &from), vector_dot(&N, &from)) * 180 / PI;
    if (heading < 0) heading += 360;
    return heading;
}

template <typename Ta, typename Tb, typename To> void LSM303::vector_cross(const vector<Ta> *a, const vector<Tb> *b, vector<To> *out)
{
  out->x = (a->y * b->z) - (a->z * b->y);
  out->y = (a->z * b->x) - (a->x * b->z);
  out->z = (a->x * b->y) - (a->y * b->x);
}

template <typename Ta, typename Tb> float LSM303::vector_dot(const vector<Ta> *a, const vector<Tb> *b)
{
  return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}
#endif
#endif



// Defines ////////////////////////////////////////////////////////////////

// The Arduino two-wire interface uses a 7-bit number for the address,
// and sets the last bit correctly based on reads and writes
#define D_SA0_HIGH_ADDRESS                0b0011101
#define D_SA0_LOW_ADDRESS                 0b0011110
#define DLHC_DLM_DLH_MAG_ADDRESS          0b0011110
#define DLHC_DLM_DLH_ACC_SA0_HIGH_ADDRESS 0b0011001
#define DLM_DLH_ACC_SA0_LOW_ADDRESS       0b0011000

#define TEST_REG_ERROR -1

#define D_WHO_ID    0x49
#define DLM_WHO_ID  0x3C

// Constructors ////////////////////////////////////////////////////////////////

LSM303::LSM303(void)
{
  /*
  These values lead to an assumed magnetometer bias of 0.
  Use the Calibrate example program to determine appropriate values
  for your particular unit. The Heading example demonstrates how to
  adjust these values in your own sketch.
  */
  m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
  m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};

  _device = device_auto;

  io_timeout = 0;  // 0 = no timeout
  did_timeout = false;
}

// Public Methods //////////////////////////////////////////////////////////////

// Did a timeout occur in readAcc(), readMag(), or read() since the last call to timeoutOccurred()?
bool LSM303::timeoutOccurred()
{
  bool tmp = did_timeout;
  did_timeout = false;
  return tmp;
}

void LSM303::setTimeout(unsigned int timeout)
{
  io_timeout = timeout;
}

unsigned int LSM303::getTimeout()
{
  return io_timeout;
}

bool LSM303::init(deviceType device, sa0State sa0)
{
  // perform auto-detection unless device type and SA0 state were both specified
  if (device == device_auto || sa0 == sa0_auto)
  {
    // check for LSM303D if device is unidentified or was specified to be this type
    if (device == device_auto || device == device_D)
    {
      // check SA0 high address unless SA0 was specified to be low
      if (sa0 != sa0_low && testReg(D_SA0_HIGH_ADDRESS, WHO_AM_I) == D_WHO_ID)
      {
        // device responds to address 0011101 with D ID; it's a D with SA0 high
        device = device_D;
        sa0 = sa0_high;
      }
      // check SA0 low address unless SA0 was specified to be high
      else if (sa0 != sa0_high && testReg(D_SA0_LOW_ADDRESS, WHO_AM_I) == D_WHO_ID)
      {
        // device responds to address 0011110 with D ID; it's a D with SA0 low
        device = device_D;
        sa0 = sa0_low;
      }
    }
    
    // check for LSM303DLHC, DLM, DLH if device is still unidentified or was specified to be one of these types
    if (device == device_auto || device == device_DLHC || device == device_DLM || device == device_DLH)
    {
      // check SA0 high address unless SA0 was specified to be low
      if (sa0 != sa0_low && testReg(DLHC_DLM_DLH_ACC_SA0_HIGH_ADDRESS, CTRL_REG1_A) != TEST_REG_ERROR)
      {
        // device responds to address 0011001; it's a DLHC, DLM with SA0 high, or DLH with SA0 high
        sa0 = sa0_high;
        if (device == device_auto)
        { 
          // use magnetometer WHO_AM_I register to determine device type
          //
          // DLHC seems to respond to WHO_AM_I request the same way as DLM, even though this
          // register isn't documented in its datasheet. Since the DLHC accelerometer address is the
          // same as the DLM with SA0 high, but Pololu DLM boards pull SA0 low by default, we'll
          // guess that a device whose accelerometer responds to the SA0 high address and whose
          // magnetometer gives the DLM ID is actually a DLHC.
          device = (testReg(DLHC_DLM_DLH_MAG_ADDRESS, WHO_AM_I_M) == DLM_WHO_ID) ? device_DLHC : device_DLH;
        }
      }
      // check SA0 low address unless SA0 was specified to be high
      else if (sa0 != sa0_high && testReg(DLM_DLH_ACC_SA0_LOW_ADDRESS, CTRL_REG1_A) != TEST_REG_ERROR)
      {
        // device responds to address 0011000; it's a DLM with SA0 low or DLH with SA0 low
        sa0 = sa0_low;
        if (device == device_auto)
        {
          // use magnetometer WHO_AM_I register to determine device type
          device = (testReg(DLHC_DLM_DLH_MAG_ADDRESS, WHO_AM_I_M) == DLM_WHO_ID) ? device_DLM : device_DLH;
        }
      }
    }
    
    // make sure device and SA0 were successfully detected; otherwise, indicate failure
    if (device == device_auto || sa0 == sa0_auto)
    {
      return false;
    }
  }
  _device = device;
  
  // set device addresses and translated register addresses
  switch (device)
  {
    case device_D:
      acc_address = mag_address = (sa0 == sa0_high) ? D_SA0_HIGH_ADDRESS : D_SA0_LOW_ADDRESS;
      translated_regs[-OUT_X_L_M] = D_OUT_X_L_M;
      translated_regs[-OUT_X_H_M] = D_OUT_X_H_M;
      translated_regs[-OUT_Y_L_M] = D_OUT_Y_L_M;
      translated_regs[-OUT_Y_H_M] = D_OUT_Y_H_M;
      translated_regs[-OUT_Z_L_M] = D_OUT_Z_L_M;
      translated_regs[-OUT_Z_H_M] = D_OUT_Z_H_M;
      break;

    case device_DLHC:
      acc_address = DLHC_DLM_DLH_ACC_SA0_HIGH_ADDRESS; // DLHC doesn't have configurable SA0 but uses same acc address as DLM/DLH with SA0 high
      mag_address = DLHC_DLM_DLH_MAG_ADDRESS;
      translated_regs[-OUT_X_H_M] = DLHC_OUT_X_H_M;
      translated_regs[-OUT_X_L_M] = DLHC_OUT_X_L_M;
      translated_regs[-OUT_Y_H_M] = DLHC_OUT_Y_H_M;
      translated_regs[-OUT_Y_L_M] = DLHC_OUT_Y_L_M;
      translated_regs[-OUT_Z_H_M] = DLHC_OUT_Z_H_M;
      translated_regs[-OUT_Z_L_M] = DLHC_OUT_Z_L_M;
      break;

    case device_DLM:
      acc_address = (sa0 == sa0_high) ? DLHC_DLM_DLH_ACC_SA0_HIGH_ADDRESS : DLM_DLH_ACC_SA0_LOW_ADDRESS;
      mag_address = DLHC_DLM_DLH_MAG_ADDRESS;
      translated_regs[-OUT_X_H_M] = DLM_OUT_X_H_M;
      translated_regs[-OUT_X_L_M] = DLM_OUT_X_L_M;
      translated_regs[-OUT_Y_H_M] = DLM_OUT_Y_H_M;
      translated_regs[-OUT_Y_L_M] = DLM_OUT_Y_L_M;
      translated_regs[-OUT_Z_H_M] = DLM_OUT_Z_H_M;
      translated_regs[-OUT_Z_L_M] = DLM_OUT_Z_L_M;
      break;

    case device_DLH:
      acc_address = (sa0 == sa0_high) ? DLHC_DLM_DLH_ACC_SA0_HIGH_ADDRESS : DLM_DLH_ACC_SA0_LOW_ADDRESS;
      mag_address = DLHC_DLM_DLH_MAG_ADDRESS;
      translated_regs[-OUT_X_H_M] = DLH_OUT_X_H_M;
      translated_regs[-OUT_X_L_M] = DLH_OUT_X_L_M;
      translated_regs[-OUT_Y_H_M] = DLH_OUT_Y_H_M;
      translated_regs[-OUT_Y_L_M] = DLH_OUT_Y_L_M;
      translated_regs[-OUT_Z_H_M] = DLH_OUT_Z_H_M;
      translated_regs[-OUT_Z_L_M] = DLH_OUT_Z_L_M;
      break;
  }
  
  return true;
}

/*
Enables the LSM303's accelerometer and magnetometer. Also:
- Sets sensor full scales (gain) to default power-on values, which are
  +/- 2 g for accelerometer and +/- 1.3 gauss for magnetometer
  (+/- 4 gauss on LSM303D).
- Selects 50 Hz ODR (output data rate) for accelerometer and 7.5 Hz
  ODR for magnetometer (6.25 Hz on LSM303D). (These are the ODR
  settings for which the electrical characteristics are specified in
  the datasheets.)
- Enables high resolution modes (if available).
Note that this function will also reset other settings controlled by
the registers it writes to.
*/
void LSM303::enableDefault(void)
{

  if (_device == device_D)
  {
    // Accelerometer

    // 0x00 = 0b00000000
    // AFS = 0 (+/- 2 g full scale)
    writeReg(CTRL2, 0x00);

    // 0x57 = 0b01010111
    // AODR = 0101 (50 Hz ODR); AZEN = AYEN = AXEN = 1 (all axes enabled)
    writeReg(CTRL1, 0x57);

    // Magnetometer

    // 0x64 = 0b01100100
    // M_RES = 11 (high resolution mode); M_ODR = 001 (6.25 Hz ODR)
    writeReg(CTRL5, 0x64);

    // 0x20 = 0b00100000
    // MFS = 01 (+/- 4 gauss full scale)
    writeReg(CTRL6, 0x20);

    // 0x00 = 0b00000000
    // MLP = 0 (low power mode off); MD = 00 (continuous-conversion mode)
    writeReg(CTRL7, 0x00);
  }
  else
  {
    // Accelerometer
    
    if (_device == device_DLHC)
    {
      // 0x08 = 0b00001000
      // FS = 00 (+/- 2 g full scale); HR = 1 (high resolution enable)
      writeAccReg(CTRL_REG4_A, 0x08);

      // 0x47 = 0b01000111
      // ODR = 0100 (50 Hz ODR); LPen = 0 (normal mode); Zen = Yen = Xen = 1 (all axes enabled)
      writeAccReg(CTRL_REG1_A, 0x47);
    }
    else // DLM, DLH
    {
      // 0x00 = 0b00000000
      // FS = 00 (+/- 2 g full scale)
      writeAccReg(CTRL_REG4_A, 0x00);

      // 0x27 = 0b00100111
      // PM = 001 (normal mode); DR = 00 (50 Hz ODR); Zen = Yen = Xen = 1 (all axes enabled)
      writeAccReg(CTRL_REG1_A, 0x27);
    }

    // Magnetometer

    // 0x0C = 0b00001100
    // DO = 011 (7.5 Hz ODR)
    writeMagReg(CRA_REG_M, 0x0C);

    // 0x20 = 0b00100000
    // GN = 001 (+/- 1.3 gauss full scale)
    writeMagReg(CRB_REG_M, 0x20);

    // 0x00 = 0b00000000
    // MD = 00 (continuous-conversion mode)
    writeMagReg(MR_REG_M, 0x00);
  }
}

// Writes an accelerometer register
void LSM303::writeAccReg(byte reg, byte value)
{
  BIOS::GPIO::I2C::BeginTransmission(acc_address);
  BIOS::GPIO::I2C::Write(reg);
  BIOS::GPIO::I2C::Write(value);
  last_status = BIOS::GPIO::I2C::EndTransmission();
}

// Reads an accelerometer register
byte LSM303::readAccReg(byte reg)
{
  byte value;

  BIOS::GPIO::I2C::BeginTransmission(acc_address);
  BIOS::GPIO::I2C::Write(reg);
  last_status = BIOS::GPIO::I2C::EndTransmission();
  BIOS::GPIO::I2C::RequestFrom(acc_address, (byte)1);
  value = BIOS::GPIO::I2C::Read();
  BIOS::GPIO::I2C::EndTransmission();

  return value;
}

// Writes a magnetometer register
void LSM303::writeMagReg(byte reg, byte value)
{
  BIOS::GPIO::I2C::BeginTransmission(mag_address);
  BIOS::GPIO::I2C::Write(reg);
  BIOS::GPIO::I2C::Write(value);
  last_status = BIOS::GPIO::I2C::EndTransmission();
}

// Reads a magnetometer register
byte LSM303::readMagReg(int reg)
{
  byte value;

  // if dummy register address (magnetometer Y/Z), look up actual translated address (based on device type)
  if (reg < 0)
  {
    reg = translated_regs[-reg];
  }

  BIOS::GPIO::I2C::BeginTransmission(mag_address);
  BIOS::GPIO::I2C::Write(reg);
  last_status = BIOS::GPIO::I2C::EndTransmission();
  BIOS::GPIO::I2C::RequestFrom(mag_address, (byte)1);
  value = BIOS::GPIO::I2C::Read();
  BIOS::GPIO::I2C::EndTransmission();

  return value;
}

void LSM303::writeReg(byte reg, byte value)
{
  // mag address == acc_address for LSM303D, so it doesn't really matter which one we use.
  if (_device == device_D || reg < CTRL_REG1_A)
  {
    writeMagReg(reg, value);
  }
  else
  {
    writeAccReg(reg, value);
  }
}

// Note that this function will not work for reading TEMP_OUT_H_M and TEMP_OUT_L_M on the DLHC.
// To read those two registers, use readMagReg() instead.
byte LSM303::readReg(int reg)
{
  // mag address == acc_address for LSM303D, so it doesn't really matter which one we use.
  // Use readMagReg so it can translate OUT_[XYZ]_[HL]_M
  if (_device == device_D || reg < CTRL_REG1_A)
  {
    return readMagReg(reg);
  }
  else
  {
    return readAccReg(reg);
  }
}

// Reads the 3 accelerometer channels and stores them in vector a
void LSM303::readAcc(void)
{
  BIOS::GPIO::I2C::BeginTransmission(acc_address);
  // assert the MSB of the address to get the accelerometer
  // to do slave-transmit subaddress updating.
  BIOS::GPIO::I2C::Write(OUT_X_L_A | (1 << 7));
  last_status = BIOS::GPIO::I2C::EndTransmission();
  BIOS::GPIO::I2C::RequestFrom(acc_address, (byte)6);
/*
  unsigned int millis_start = BIOS::SYS::GetTick();
  while (Wire.available() < 6) {
    if (io_timeout > 0 && ((unsigned int)BIOS::SYS::GetTick() - millis_start) > io_timeout)
    {
      did_timeout = true;
      return;
    }
  }
*/
  byte xla = BIOS::GPIO::I2C::Read();
  byte xha = BIOS::GPIO::I2C::Read();
  byte yla = BIOS::GPIO::I2C::Read();
  byte yha = BIOS::GPIO::I2C::Read();
  byte zla = BIOS::GPIO::I2C::Read();
  byte zha = BIOS::GPIO::I2C::Read();

  // combine high and low bytes
  // This no longer drops the lowest 4 bits of the readings from the DLH/DLM/DLHC, which are always 0
  // (12-bit resolution, left-aligned). The D has 16-bit resolution
  a.x = (int16_t)(xha << 8 | xla);
  a.y = (int16_t)(yha << 8 | yla);
  a.z = (int16_t)(zha << 8 | zla);
}

// Reads the 3 magnetometer channels and stores them in vector m
void LSM303::readMag(void)
{
  BIOS::GPIO::I2C::BeginTransmission(mag_address);
  // If LSM303D, assert MSB to enable subaddress updating
  // OUT_X_L_M comes first on D, OUT_X_H_M on others
  BIOS::GPIO::I2C::Write((_device == device_D) ? translated_regs[-OUT_X_L_M] | (1 << 7) : translated_regs[-OUT_X_H_M]);
  last_status = BIOS::GPIO::I2C::EndTransmission();
  BIOS::GPIO::I2C::RequestFrom(mag_address, (byte)6);

  unsigned int millis_start = BIOS::SYS::GetTick();
  /*
  while (Wire.available() < 6) {
    if (io_timeout > 0 && ((unsigned int)millis() - millis_start) > io_timeout)
    {
      did_timeout = true;
      return;
    }
  }
*/
  byte xlm, xhm, ylm, yhm, zlm, zhm;

  if (_device == device_D)
  {
    // D: X_L, X_H, Y_L, Y_H, Z_L, Z_H
    xlm = BIOS::GPIO::I2C::Read();
    xhm = BIOS::GPIO::I2C::Read();
    ylm = BIOS::GPIO::I2C::Read();
    yhm = BIOS::GPIO::I2C::Read();
    zlm = BIOS::GPIO::I2C::Read();
    zhm = BIOS::GPIO::I2C::Read();
  }
  else
  {
    // DLHC, DLM, DLH: X_H, X_L...
    xhm = BIOS::GPIO::I2C::Read();
    xlm = BIOS::GPIO::I2C::Read();

    if (_device == device_DLH)
    {
      // DLH: ...Y_H, Y_L, Z_H, Z_L
      yhm = BIOS::GPIO::I2C::Read();
      ylm = BIOS::GPIO::I2C::Read();
      zhm = BIOS::GPIO::I2C::Read();
      zlm = BIOS::GPIO::I2C::Read();
    }
    else
    {
      // DLM, DLHC: ...Z_H, Z_L, Y_H, Y_L
      zhm = BIOS::GPIO::I2C::Read();
      zlm = BIOS::GPIO::I2C::Read();
      yhm = BIOS::GPIO::I2C::Read();
      ylm = BIOS::GPIO::I2C::Read();
    }
  }
    BIOS::GPIO::I2C::EndTransmission();
  // combine high and low bytes
  m.x = (int16_t)(xhm << 8 | xlm);
  m.y = (int16_t)(yhm << 8 | ylm);
  m.z = (int16_t)(zhm << 8 | zlm);
}

// Reads all 6 channels of the LSM303 and stores them in the object variables
void LSM303::read(void)
{
  readAcc();
  readMag();
}
#ifdef CALC
/*
Returns the angular difference in the horizontal plane between a
default vector and north, in degrees.
The default vector here is chosen to point along the surface of the
PCB, in the direction of the top of the text on the silkscreen.
This is the +X axis on the Pololu LSM303D carrier and the -Y axis on
the Pololu LSM303DLHC, LSM303DLM, and LSM303DLH carriers.
*/
float LSM303::heading(void)
{
  if (_device == device_D)
  {
    return heading((vector<int>){1, 0, 0});
  }
  else
  {
    return heading((vector<int>){0, -1, 0});
  }
}

void LSM303::vector_normalize(vector<float> *a)
{
  float mag = sqrt(vector_dot(a, a));
  a->x /= mag;
  a->y /= mag;
  a->z /= mag;
}
#endif
// Private Methods //////////////////////////////////////////////////////////////

int LSM303::testReg(byte address, regAddr reg)
{
  if (!BIOS::GPIO::I2C::BeginTransmission(address))
  {
      BIOS::GPIO::I2C::EndTransmission();
      return TEST_REG_ERROR;
  }
  BIOS::GPIO::I2C::Write((byte)reg);
  if (!BIOS::GPIO::I2C::EndTransmission())
  {
      return TEST_REG_ERROR;
  }

  BIOS::GPIO::I2C::RequestFrom(address, (byte)1);
    int r = BIOS::GPIO::I2C::Read();
    if (!BIOS::GPIO::I2C::EndTransmission())
        return TEST_REG_ERROR;
    return r;
}

int main(void)
{
	  BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::I2c);
	  BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::I2c);
    
    LSM303 compass;
  if (!compass.init(LSM303::device_DLHC /*, LSM303::sa0_high*/))
  {
      BIOS::DBG::Print("Init failed!");
      return 0;
  }
  compass.enableDefault();
  
  /*
  Calibration values; the default values of +/-32767 for each axis
  lead to an assumed magnetometer bias of 0. Use the Calibrate example
  program to determine appropriate values for your particular unit.
  */
  compass.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
  compass.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};

    while (!BIOS::KEY::GetKey())
    {
        compass.read();
  
  /*
  When given no arguments, the heading() function returns the angular
  difference in the horizontal plane between a default vector and
  north, in degrees.
  
  The default vector is chosen by the library to point along the
  surface of the PCB, in the direction of the top of the text on the
  silkscreen. This is the +X axis on the Pololu LSM303D carrier and
  the -Y axis on the Pololu LSM303DLHC, LSM303DLM, and LSM303DLH
  carriers.
  
  To use a different vector as a reference, use the version of heading()
  that takes a vector argument; for example, use
  
    compass.heading((LSM303::vector<int>){0, 0, 1});
  
  to use the +Z axis as a reference.
  */
  //float heading = compass.heading();
  
  //BIOS::DBG::Print("heading=%f\n", heading);
  compass.readMag();
  BIOS::DBG::Print("mag=%d, %d, %d, ", compass.m.x, compass.m.y, 
    compass.m.z);
    
    compass.readAcc();
  BIOS::DBG::Print("acc=%d, %d, %d\n", compass.a.x, compass.a.y, 
    compass.a.z);
    
    
  BIOS::SYS::DelayMs(200);        
    }
    
    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Input);
	BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Input);

}