#include <library.h>
typedef uint8_t byte;

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

    while (!BIOS::KEY::GetKey())
    {
        int x, y, z;
        readMag(x, y, z);
        BIOS::DBG::Print("mag=%d, %d, %d; ", x, y, z);
        readAcc(x, y, z);
        BIOS::DBG::Print("acc=%d, %d, %d\n", x, y, z);
        BIOS::SYS::DelayMs(200);
    }
    
    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Input);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Input);
}