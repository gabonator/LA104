#include <library.h>
typedef uint8_t byte;

enum {
    L3GD20_ADDRESS = 0x69, //0x6B,
    GYRO_REGISTER_WHO_AM_I = 0x0F,
    L3GD20_ID = 0xD3, //0xD4,
    L3GD20H_ID = 0xD7,
    GYRO_REGISTER_OUT_X_L = 0x28,
    
    GYRO_REGISTER_CTRL_REG1 = 0x20,
    GYRO_REGISTER_CTRL_REG2 = 0x21,
    GYRO_REGISTER_CTRL_REG3 = 0x22,
    GYRO_REGISTER_CTRL_REG4 = 0x23,
    GYRO_REGISTER_CTRL_REG5 = 0x24,
    
    GYRO_RANGE_250DPS = 250,
    GYRO_RANGE_500DPS = 500,
    GYRO_RANGE_2000DPS = 2000
};

void write8(byte reg, byte value) 
{
  BIOS::GPIO::I2C::BeginTransmission(L3GD20_ADDRESS);
  BIOS::GPIO::I2C::Write(reg);
  BIOS::GPIO::I2C::Write(value);
  BIOS::GPIO::I2C::EndTransmission();
}

byte read8(byte reg) 
{
  BIOS::GPIO::I2C::BeginTransmission(L3GD20_ADDRESS);
  BIOS::GPIO::I2C::Write(reg);
  BIOS::GPIO::I2C::EndTransmission();
  BIOS::GPIO::I2C::RequestFrom(L3GD20_ADDRESS, 1);
  int v = BIOS::GPIO::I2C::Read();
  BIOS::GPIO::I2C::EndTransmission();
  return v;
}

bool gyro_begin(int range) 
{
  uint8_t id = read8(GYRO_REGISTER_WHO_AM_I);
  BIOS::DBG::Print("whoami = %02x %02x\n", id, GYRO_REGISTER_WHO_AM_I);
  if ((id != L3GD20_ID) && (id != L3GD20H_ID)) {
    return false;
  }

  write8(GYRO_REGISTER_CTRL_REG1, 0x00);
  write8(GYRO_REGISTER_CTRL_REG1, 0x0F);
  switch (range) {
  case GYRO_RANGE_250DPS:
    write8(GYRO_REGISTER_CTRL_REG4, 0x00);
    break;
  case GYRO_RANGE_500DPS:
    write8(GYRO_REGISTER_CTRL_REG4, 0x10);
    break;
  case GYRO_RANGE_2000DPS:
    write8(GYRO_REGISTER_CTRL_REG4, 0x20);
    break;
  }

  return true;
}

bool readGyro(int& x, int& y, int& z)
{
    BIOS::GPIO::I2C::BeginTransmission(L3GD20_ADDRESS);
    BIOS::GPIO::I2C::Write(GYRO_REGISTER_OUT_X_L | 0x80);
    if (!BIOS::GPIO::I2C::EndTransmission())
        return false;
      
    BIOS::GPIO::I2C::RequestFrom(L3GD20_ADDRESS, 6);

    uint8_t xlo = BIOS::GPIO::I2C::Read();
    uint8_t xhi = BIOS::GPIO::I2C::Read();
    uint8_t ylo = BIOS::GPIO::I2C::Read();
    uint8_t yhi = BIOS::GPIO::I2C::Read();
    uint8_t zlo = BIOS::GPIO::I2C::Read();
    uint8_t zhi = BIOS::GPIO::I2C::Read();

    /* Shift values to create properly formed integer (low byte first) */
    x = (int16_t)(xlo | (xhi << 8));
    y = (int16_t)(ylo | (yhi << 8));
    z = (int16_t)(zlo | (zhi << 8));
    return true;
}



enum {
    DLHC_DLM_DLH_ACC_SA0_HIGH_ADDRESS = 0b0011001,
    DLHC_DLM_DLH_MAG_ADDRESS = 0b0011110,
    OUT_X_L_A = 0x28,
    DLHC_OUT_X_H_M = 3,
    CTRL_REG4_A       = 0x23, // DLH, DLM, DLHC
    CRA_REG_M         = 0x00, // DLH, DLM, DLHC
    CRB_REG_M         = 0x01, // DLH, DLM, DLHC
    MR_REG_M          = 0x02, // DLH, DLM, DLHC
    CTRL_REG1_A       = 0x20, // DLH, DLM, DLHC
};

int acc_address = DLHC_DLM_DLH_ACC_SA0_HIGH_ADDRESS;
int mag_address = DLHC_DLM_DLH_MAG_ADDRESS;

void writeAccReg(byte reg, byte value)
{
  BIOS::GPIO::I2C::BeginTransmission(acc_address);
  BIOS::GPIO::I2C::Write(reg);
  BIOS::GPIO::I2C::Write(value);
  BIOS::GPIO::I2C::EndTransmission();
}

void writeMagReg(byte reg, byte value)
{
  BIOS::GPIO::I2C::BeginTransmission(mag_address);
  BIOS::GPIO::I2C::Write(reg);
  BIOS::GPIO::I2C::Write(value);
  BIOS::GPIO::I2C::EndTransmission();
}

void readAcc(int &x, int &y, int &z)
{
  BIOS::GPIO::I2C::BeginTransmission(acc_address);
  BIOS::GPIO::I2C::Write(OUT_X_L_A | (1 << 7));
  BIOS::GPIO::I2C::EndTransmission();
  BIOS::GPIO::I2C::RequestFrom(acc_address, (byte)6);

  byte xla = BIOS::GPIO::I2C::Read();
  byte xha = BIOS::GPIO::I2C::Read();
  byte yla = BIOS::GPIO::I2C::Read();
  byte yha = BIOS::GPIO::I2C::Read();
  byte zla = BIOS::GPIO::I2C::Read();
  byte zha = BIOS::GPIO::I2C::Read();

  x = (int16_t)(xha << 8 | xla);
  y = (int16_t)(yha << 8 | yla);
  z = (int16_t)(zha << 8 | zla);
  BIOS::GPIO::I2C::EndTransmission();
}

void readMag(int&x, int&y, int& z)
{
    BIOS::GPIO::I2C::BeginTransmission(mag_address);
    BIOS::GPIO::I2C::Write(DLHC_OUT_X_H_M);
    BIOS::GPIO::I2C::EndTransmission();
    BIOS::GPIO::I2C::RequestFrom(mag_address, (byte)6);
    byte xlm, xhm, ylm, yhm, zlm, zhm;

    // DLHC, DLM, DLH: X_H, X_L...
    xhm = BIOS::GPIO::I2C::Read();
    xlm = BIOS::GPIO::I2C::Read();
    // DLM, DLHC: ...Z_H, Z_L, Y_H, Y_L
    zhm = BIOS::GPIO::I2C::Read();
    zlm = BIOS::GPIO::I2C::Read();
    yhm = BIOS::GPIO::I2C::Read();
    ylm = BIOS::GPIO::I2C::Read();
    BIOS::GPIO::I2C::EndTransmission();
    // combine high and low bytes
    x = (int16_t)(xhm << 8 | xlm);
    y = (int16_t)(yhm << 8 | ylm);
    z = (int16_t)(zhm << 8 | zlm);
}

int main(void)
{
    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::I2c);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::I2c);
    
    writeAccReg(CTRL_REG4_A, 0x08);
    writeAccReg(CTRL_REG1_A, 0x47);
    writeMagReg(CRA_REG_M, 0x0C);
    writeMagReg(CRB_REG_M, 0x20);
    writeMagReg(MR_REG_M, 0x00);
    gyro_begin(GYRO_RANGE_2000DPS);

    while (!BIOS::KEY::GetKey())
    {
        int x, y, z;
        readGyro(x, y, z);
        BIOS::DBG::Print("gyro=%d, %d, %d; ", x, y, z);
        readMag(x, y, z);
        BIOS::DBG::Print("mag=%d, %d, %d; ", x, y, z);
        readAcc(x, y, z);
        BIOS::DBG::Print("acc=%d, %d, %d\n", x, y, z);
        BIOS::SYS::DelayMs(200);
    }
    
    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Input);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Input);
}