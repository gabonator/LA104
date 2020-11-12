#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"

extern "C" {
#include "stm32/stm32f10x_i2c.h"
#include "stm32/la104_i2c.h"
}


long timeout = 0;
void TimeoutReset(long duration)
{
    timeout = BIOS::SYS::GetTick() + duration;
}
bool Timeout()
{
    EVERY(500)
    {
        CONSOLE::Color(RGB565(b0b0b0));
        CONSOLE::Print(".");
        CONSOLE::Color(RGB565(ffffff));
    }
    return BIOS::SYS::GetTick() > timeout;
}

#define TIMEOUT(msg) if (Timeout()) { \
    CONSOLE::Color(RGB565(ff0000)); \
    CONSOLE::Print("Timeout: %s ", msg); \
    CONSOLE::Color(RGB565(ffffff)); \
    return false; \
  }

bool I2C_check_presence(uint8_t HW_address)
{
    HW_address <<= 1;

    BIOS::GPIO::PinMode(BIOS::GPIO::EPin::P1, BIOS::GPIO::EMode::Output);
    BIOS::GPIO::PinMode(BIOS::GPIO::EPin::P2, BIOS::GPIO::EMode::Output);
    BIOS::GPIO::DigitalWrite(BIOS::GPIO::EPin::P1, true);
    BIOS::GPIO::DigitalWrite(BIOS::GPIO::EPin::P2, true);
    BIOS::SYS::DelayMs(100);
    BIOS::GPIO::PinMode(BIOS::GPIO::EPin::P1, BIOS::GPIO::EMode::I2c);
    BIOS::GPIO::PinMode(BIOS::GPIO::EPin::P2, BIOS::GPIO::EMode::I2c);
    BIOS::SYS::DelayMs(100);

/*    
//    I2C_Cmd(I2Cx, DISABLE);
    BIOS::GPIO::PinMode(BIOS::GPIO::EPin::P1, BIOS::GPIO::EMode::Input);
    BIOS::SYS::DelayMs(100);
    BIOS::GPIO::PinMode(BIOS::GPIO::EPin::P1, BIOS::GPIO::EMode::I2c);
//    I2C_Cmd(I2Cx, ENABLE);

    I2C_AcknowledgeConfig(I2Cx, DISABLE);
    I2C_GenerateSTOP(I2Cx, ENABLE);
    TimeoutReset(1000);
*/
/*
    TimeoutReset(200);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)) 
    {
      if (Timeout())
      {
        BIOS::GPIO::PinMode(BIOS::GPIO::EPin::P1, BIOS::GPIO::EMode::Output);
        BIOS::GPIO::PinMode(BIOS::GPIO::EPin::P2, BIOS::GPIO::EMode::Output);
        BIOS::GPIO::DigitalWrite(BIOS::GPIO::EPin::P1, true);
        BIOS::GPIO::DigitalWrite(BIOS::GPIO::EPin::P2, true);
    BIOS::SYS::DelayMs(100);
        BIOS::GPIO::PinMode(BIOS::GPIO::EPin::P1, BIOS::GPIO::EMode::I2c);
        BIOS::GPIO::PinMode(BIOS::GPIO::EPin::P2, BIOS::GPIO::EMode::I2c);
    BIOS::SYS::DelayMs(100);
        CONSOLE::Print("[busy i2c workaround]");
//        _ASSERT(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
        break;
      }
    } //TIMEOUT("1-busy");
*/
    TimeoutReset(500);

//    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)) TIMEOUT("1-busy");
    I2C_GenerateSTART(I2Cx, ENABLE);
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) TIMEOUT("2-start");
    I2C_Send7bitAddress(I2Cx, HW_address, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) TIMEOUT("3-transmit");

//    I2C_SendData(I2Cx, addr); 
//    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) TIMEOUT("4-address");

    //I2C_AcknowledgeConfig(I2Cx, DISABLE);

    I2C_GenerateSTOP(I2Cx, ENABLE);
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)) TIMEOUT("5-stop");
    return true;
}
/*
void I2C_burst_read(uint8_t HW_address, uint8_t addr, uint8_t n_data, uint8_t *data)
{
// <start><addr><ack><command><ack><start><addr><ack><data><nack><stop>

HW_address <<= 1;
		while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)) q("a");
	I2C_GenerateSTART(I2Cx, ENABLE);
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) q("b");
	I2C_Send7bitAddress(I2Cx, HW_address, I2C_Direction_Transmitter);
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) q("c");
	I2C_SendData(I2Cx, addr);
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) q("d");

//	I2C_GenerateSTOP(I2Cx, ENABLE);


	I2C_GenerateSTART(I2Cx, ENABLE);
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) q("e");
	I2C_Send7bitAddress(I2Cx, HW_address, I2C_Direction_Receiver);
		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) q("f");

	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	while(n_data--) {
		if(!n_data) I2C_AcknowledgeConfig(I2Cx, DISABLE);
			while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) q("g");
		*data++ = I2C_ReceiveData(I2Cx);
	}
	I2C_AcknowledgeConfig(I2Cx, DISABLE);

	I2C_GenerateSTOP(I2Cx, ENABLE);
		while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)) q("h");
}
*/


uint32_t* gpioStatus = nullptr;

bool check()
{
  if (*gpioStatus == 0)
  {
    return true;
  }
  else
    CONSOLE::Print("[err:%d]", *gpioStatus);

  *gpioStatus = 0;
  return false;
}

void setup()
{
    BIOS::GPIO::PinMode(BIOS::GPIO::EPin::P1, BIOS::GPIO::EMode::I2c);
    gpioStatus = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::GpioStatus);
}

void loop()
{
  if (I2C_check_presence(0x40))
    CONSOLE::Print("Good, ");
  else
  {
    CONSOLE::Print("Bad, ");
  }
  BIOS::SYS::DelayMs(1000);
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    APP::Init("I2C low level test");
    APP::Status("P1: SCL, P2: SDA");

    setup();

    BIOS::KEY::EKey key;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
    {
      loop();
    }
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(" [%d]: %s\n", line, cond);
    while (1);
}
