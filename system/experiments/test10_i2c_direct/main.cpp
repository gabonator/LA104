#include <library.h>
#include <math.h>

#include <stdint.h>
#include <stm32f10x_conf.h>
#include <stm32f10x.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

typedef bool boolean;
void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_address_direction(uint8_t address, uint8_t direction);
void i2c_transmit(uint8_t byte);
uint8_t i2c_receive_ack(void);
uint8_t i2c_receive_nack(void);
void i2c_write(uint8_t address, uint8_t data);
void i2c_read(uint8_t address, uint8_t* data);


void delay(int d)
{
  BIOS::SYS::DelayMs(d);
}

class CWire
{
  int mCount{0};
  uint8_t mAddress;
  bool mTransmitting{false};
  bool mReceiving{false};
public:
  void begin()
  {
  }

  void beginTransmission(uint8_t address)
  {
    mAddress = address;
    mTransmitting = false;
  }
  void write(uint8_t data)
  {
    if (!mTransmitting)
    {
      mTransmitting = true;
      i2c_start();
      i2c_address_direction(mAddress << 1, I2C_Direction_Transmitter); 
    }
    i2c_transmit(data);
  }
  void endTransmission()
  {
    i2c_stop();
  }  
  void requestFrom(uint8_t address, int count)
  {
    i2c_start();
    i2c_address_direction(address << 1, I2C_Direction_Receiver);
    mCount = count;
  }
  uint8_t read()
  {
    if (mCount>0)
      mCount--;
    if (mCount>0)
      return i2c_receive_ack();
    else
      return i2c_receive_nack();

  }
} Wire;

#include "bmp085.h"

/*
  P1: PB15, PB10  remap   TIM2_CH3
  P2: PB14, PB11  remap   TIM2_CH4
  P3: PB13, PB8   default TIM4_CH3
  P4: PB12, PB9   default TIM4_CH4
*/

void assert_failed(u8* file, u32 line)
{
  while (1) ;
}


#define I2Cx_RCC        RCC_APB1Periph_I2C2
#define I2Cx            I2C2
#define I2C_GPIO_RCC    RCC_APB2Periph_GPIOB
#define I2C_GPIO        GPIOB
#define I2C_PIN_SDA     GPIO_Pin_11
#define I2C_PIN_SCL     GPIO_Pin_10
 
#define SLAVE_ADDRESS    0x08
 
 
uint8_t receivedByte;
 
 
void i2c_init()
{
    // Initialization struct
    I2C_InitTypeDef I2C_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
 
    // Step 1: Initialize I2C
    RCC_APB1PeriphClockCmd(I2Cx_RCC, ENABLE);
    I2C_InitStruct.I2C_ClockSpeed = 100000;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2Cx, &I2C_InitStruct);
    I2C_Cmd(I2Cx, ENABLE);
 
    // Step 2: Initialize GPIO as open drain alternate function
    RCC_APB2PeriphClockCmd(I2C_GPIO_RCC, ENABLE);
    GPIO_InitStruct.GPIO_Pin = I2C_PIN_SCL | I2C_PIN_SDA;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(I2C_GPIO, &GPIO_InitStruct);
}
 
void i2c_start()
{
    // Wait until I2Cx is not busy anymore
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
 
    // Generate start condition
    I2C_GenerateSTART(I2Cx, ENABLE);
 
    // Wait for I2C EV5. 
    // It means that the start condition has been correctly released 
    // on the I2C bus (the bus is free, no other devices is communicating))
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
}
 
void i2c_stop()
{
    // Generate I2C stop condition
    I2C_GenerateSTOP(I2Cx, ENABLE);
    // Wait until I2C stop condition is finished
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF));
}
 
void i2c_address_direction(uint8_t address, uint8_t direction)
{
    // Send slave address
    I2C_Send7bitAddress(I2Cx, address, direction);
 
    // Wait for I2C EV6
    // It means that a slave acknowledges his address
    if (direction == I2C_Direction_Transmitter)
    {
        while (!I2C_CheckEvent(I2Cx,
            I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    }
    else if (direction == I2C_Direction_Receiver)
    { 
        while (!I2C_CheckEvent(I2Cx,
            I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    }
}
 
void i2c_transmit(uint8_t byte)
{
    // Send data byte
    I2C_SendData(I2Cx, byte);
    // Wait for I2C EV8_2.
    // It means that the data has been physically shifted out and 
    // output on the bus)
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}
 
uint8_t i2c_receive_ack()
{
    // Enable ACK of received data
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
    // Wait for I2C EV7
    // It means that the data has been received in I2C data register
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
 
    // Read and return data byte from I2C data register
    return I2C_ReceiveData(I2Cx);
}
 
uint8_t i2c_receive_nack()
{
    // Disable ACK of received data
    I2C_AcknowledgeConfig(I2Cx, DISABLE);
    // Wait for I2C EV7
    // It means that the data has been received in I2C data register
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
 
    // Read and return data byte from I2C data register
    return I2C_ReceiveData(I2Cx);
}
                  
void i2c_write(uint8_t address, uint8_t data)
{
    i2c_start();
    i2c_address_direction(address << 1, I2C_Direction_Transmitter);
    i2c_transmit(data);
    i2c_stop();
}
 
void i2c_read(uint8_t address, uint8_t* data)
{
    i2c_start();
    i2c_address_direction(address << 1, I2C_Direction_Receiver);
    *data = i2c_receive_nack();
    i2c_stop();
}


__attribute__((__section__(".entry"))) 
int main(void)
{ 
  BIOS::LCD::Clear(RGB565(202020));
  BIOS::DBG::Print("i2c_init\n");
  i2c_init();

  Adafruit_BMP085 bmp;

  while (BIOS::KEY::GetKey() != BIOS::KEY::Escape)
  {
    if (!bmp.begin())
    {
      BIOS::DBG::Print("not found\n");
      continue;
    }

    BIOS::DBG::Print("T=%f P=%d\n", bmp.readTemperature(), bmp.readPressure());
    BIOS::SYS::DelayMs(1000);
  }

  return 0;
}
