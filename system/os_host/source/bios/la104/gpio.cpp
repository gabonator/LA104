#include "../Bios.h"

#include <stm32f10x_tim.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>

extern "C" uint32_t FPGA32(uint8_t Cmd, uint16_t Cnt, uint32_t Data);
extern "C" uint16_t FPGA16(uint8_t Cmd, uint16_t Cnt, uint16_t Data);

enum GpioStatus {
  Ok = 0,
  I2cErrorBegin,
  I2cBusy, // SDA, SCL tied low, check pull-ups, or the i2c transceiver is stuck (try pulling low both lines)
  I2cErrorStart,
  I2cErrorStop,
  I2cErrorAddressAck,
  I2cErrorReceiveTimeout,
  I2cErrorTransmitTimeout,
  UartWrongParity,
  UartWrongDataBits,
  UartWrongStopBits,
  UartErrorInternal,
  UartErrorOverrun,
  UartErrorNoise,
  UartErrorFraming,
  UartErrorParity,
  UartErrorOverflow,
  NotImplemented
};

uint32_t gGpioStatusCode = GpioStatus::Ok;
uint32_t gGpioI2cSpeed = 100000;

namespace PIN
{
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

  const static uint32_t arrGpioBase[] = {GPIOA_BASE, GPIOB_BASE, GPIOC_BASE, GPIOD_BASE, GPIOE_BASE, GPIOF_BASE, GPIOG_BASE};

  uint32_t* GetRegister(int nPort, int nReg)
  {
    uint32_t dwAddr = arrGpioBase[nPort];

    dwAddr |= nReg*4; 
    return (uint32_t*)dwAddr;
  }

  void SetState(int nPort, int nPin, int nState)
  {
    uint32_t dwAddr;

    if ( nPin < 8 )
      dwAddr = arrGpioBase[nPort] + 0;	// CRL
    else
    {
      dwAddr = arrGpioBase[nPort] + 4;	// CRH
      nPin &= 7;
    }

    uint32_t dwMask = ~(0xf << (nPin*4));
    uint32_t dwBits = nState << (nPin*4);

    uint32_t* pCR = ((uint32_t*)dwAddr);
    *pCR = (*pCR & dwMask) | dwBits;
  }

  void SetPin(int nPort, int nPin, bool bValue)
  {
    uint32_t dwAddr = arrGpioBase[nPort];
    if ( bValue )
    {
      uint32_t* pBSRR = &((uint32_t*)dwAddr)[4];
      *pBSRR = 1<<nPin;
    } else
    {
      uint32_t* pBRR = &((uint32_t*)dwAddr)[5];
      *pBRR = 1<<nPin;
    }
  }

  bool GetPin(int nPort, int nPin)
  {
    uint32_t dwAddr = arrGpioBase[nPort];
    uint32_t dwIDR = ((uint32_t*)dwAddr)[2];
    return (dwIDR & (1<<nPin)) ? 1 : 0;
  }                   
}

// TIM8 CH4

namespace PWM
{
  void RCCInit(void)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // TIM2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // TIM4
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  } 

  void TimerInit(uint16_t period)  // TOTO!!! TIM8_CH4 alebo TIM3_CH4
  {
    TIM_TimeBaseInitTypeDef timerInitStructure;

    // http://www.micromouseonline.com/2016/02/06/pwm-basics-on-the-stm32-general-purpose-timers/

    // TIM2
    timerInitStructure.TIM_Prescaler = 72;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = period;
    timerInitStructure.TIM_ClockDivision = 0;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timerInitStructure);
    TIM_Cmd(TIM2, ENABLE); 

    // TIM4
    TIM_TimeBaseInit(TIM4, &timerInitStructure);
    TIM_Cmd(TIM4, ENABLE); 
  }

  void GPIOInit(void)
  {
    // TIM2
    {
      GPIO_InitTypeDef gpioStructure; 
      gpioStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
      gpioStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
      gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; 
      GPIO_Init(GPIOB, &gpioStructure); 
    }
    // TIM4
    {
      GPIO_InitTypeDef gpioStructure; 
      gpioStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
      gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; 
      GPIO_Init(GPIOB, &gpioStructure); 
    }
  }

  void PWMInit(void)
  { 
    TIM_OCInitTypeDef outputChannelInit;
    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
    outputChannelInit.TIM_Pulse = 1;
    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;

    // TIM2
    TIM_OC3Init(TIM2, &outputChannelInit);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_OC4Init(TIM2, &outputChannelInit);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);

    // TIM4
    TIM_OC3Init(TIM4, &outputChannelInit);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_OC4Init(TIM4, &outputChannelInit);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
  }

  void EnablePwm(void)
  {
    RCCInit();
    TimerInit(BIOS::GPIO::AnalogRange);
    GPIOInit();
    PWMInit();
  }

  void DisablePwm(void)
  {
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE);
    TIM_Cmd(TIM2, DISABLE); 
    TIM_Cmd(TIM4, DISABLE); 

    GPIO_InitTypeDef gpioStructure; 
    gpioStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    gpioStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpioStructure); 

    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, DISABLE);
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, DISABLE);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Disable);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Disable);
  }
}

namespace I2C
{
  #define I2Cx_RCC        RCC_APB1Periph_I2C2
  #define I2Cx            I2C2
  #define I2C_GPIO_RCC    RCC_APB2Periph_GPIOB
  #define I2C_GPIO        GPIOB
  #define I2C_PIN_SDA     GPIO_Pin_11
  #define I2C_PIN_SCL     GPIO_Pin_10

  extern "C" volatile uint32_t Dly_mS;
  
  void SetTimeout(int interval)
  {
    Dly_mS = interval;
  } 

  bool Timeout()
  {
    return Dly_mS == 0;
  }

  void i2c_init()
  {
      // Initialization struct
      I2C_InitTypeDef I2C_InitStruct;
      GPIO_InitTypeDef GPIO_InitStruct;
   
      // Step 1: Initialize I2C
      RCC_APB1PeriphClockCmd(I2Cx_RCC, ENABLE);
      I2C_InitStruct.I2C_ClockSpeed = gGpioI2cSpeed;
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

  void i2c_deinit()
  {
      GPIO_InitTypeDef GPIO_InitStruct;
   
      RCC_APB1PeriphClockCmd(I2Cx_RCC, DISABLE);
      I2C_Cmd(I2Cx, DISABLE);
   
//      RCC_APB2PeriphClockCmd(I2C_GPIO_RCC, DISABLE); // internal eeprom wont work
      GPIO_InitStruct.GPIO_Pin = I2C_PIN_SCL | I2C_PIN_SDA;
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
      GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(I2C_GPIO, &GPIO_InitStruct);
  }
   
  bool i2c_start()
  {
      // In cases when we need to quickly switch from one transaction to
      // another without stopping (MLX90614), this would indicate failure
/*
      // Wait until I2Cx is not busy anymore
      SetTimeout(50);
      while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
      {
        if (Timeout()) 
        {
          gGpioStatusCode = GpioStatus::I2cBusy;
          return false;
        }      
      }
*/
      if (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
      {
        gGpioStatusCode = GpioStatus::I2cBusy;
      }

      SetTimeout(50);

      // Generate start condition
      I2C_GenerateSTART(I2Cx, ENABLE);
   
      // Wait for I2C EV5. 
      // It means that the start condition has been correctly released 
      // on the I2C bus (the bus is free, no other devices is communicating))
      while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
      {
        if (Timeout())
        {
          gGpioStatusCode = GpioStatus::I2cErrorStart;
          return false;
        }
      }
      return true;
  }
   
  bool i2c_stop(bool stop)
  {
      // Generate I2C stop condition
      I2C_GenerateSTOP(I2Cx, stop ? ENABLE : DISABLE); // STOP / RESET
      // Wait until I2C stop condition is finished
      SetTimeout(50);
      while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF))
      {
        if (Timeout()) 
        { 
          gGpioStatusCode = GpioStatus::I2cErrorStop;
          return false; 
        }
      }
/*
      if (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
      {
        gGpioStatusCode = GpioStatus::I2cBusy;
      }
*/
      // Some devices really need time to release the bus (pca9685)
      SetTimeout(3);
      while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
      {
        if (Timeout()) 
        {
          gGpioStatusCode = GpioStatus::I2cBusy;
          return false;
        }      
      }
      return true;
  }
   
  bool i2c_address_direction(uint8_t address, uint8_t direction)
  {
      // Send slave address
      I2C_Send7bitAddress(I2Cx, address, direction);
   
      // Wait for I2C EV6
      // It means that a slave acknowledges his address
      SetTimeout(50);
      if (direction == I2C_Direction_Transmitter)
      {
          while (!I2C_CheckEvent(I2Cx,
              I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
          {
            if (Timeout())
            {
                gGpioStatusCode = GpioStatus::I2cErrorAddressAck;
                return false;
            }
          }
      }
      else if (direction == I2C_Direction_Receiver)
      { 
          while (!I2C_CheckEvent(I2Cx,
              I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
          {
            if (Timeout())
            {
                gGpioStatusCode = GpioStatus::I2cErrorAddressAck;
                return false;
            }
          }
      } else
      {
        _ASSERT(0);
      }
      return true;
  }
   
  bool i2c_transmit(uint8_t byte)
  {
      // Send data byte
      I2C_SendData(I2Cx, byte);
      // Wait for I2C EV8_2.
      // It means that the data has been physically shifted out and 
      // output on the bus)
      SetTimeout(50);
      while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
      {
        if (Timeout())
        {
          gGpioStatusCode = GpioStatus::I2cErrorTransmitTimeout;
          return false;
        }
      }
      return true;
  }
   
  uint8_t i2c_receive_ack()
  {
      // Enable ACK of received data
      I2C_AcknowledgeConfig(I2Cx, ENABLE);
      // Wait for I2C EV7
      // It means that the data has been received in I2C data register
      SetTimeout(50);
      while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
      {
        if (Timeout()) 
        {
          gGpioStatusCode = GpioStatus::I2cErrorReceiveTimeout;
          return 0x00;
        }
      }

      // Read and return data byte from I2C data register
      return I2C_ReceiveData(I2Cx);
  }
   
  uint8_t i2c_receive_nack()
  {
      // Disable ACK of received data
      I2C_AcknowledgeConfig(I2Cx, DISABLE);
      // Wait for I2C EV7
      // It means that the data has been received in I2C data register
      SetTimeout(50);
      while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
      {
        if (Timeout()) 
        {
          gGpioStatusCode = GpioStatus::I2cErrorReceiveTimeout;
          return 0x00;
        }
      }
   
      // Read and return data byte from I2C data register
      return I2C_ReceiveData(I2Cx);
  }
}

namespace UART
{
  void uart_configure(int baudrate, int dataBits = 8, char parity = 0, int stopBits = 1)
  {
  	/* Configure the USART3 */
  	USART_InitTypeDef USART_InitStructure;

  	/* USART3 configuration ------------------------------------------------------*/
  	/* USART configured as follow:
  		          - BaudRate = 9600 baud
  		          - Word Length = 8 Bits
  		          - One Stop Bit
  		          - No parity
  		          - Hardware flow control disabled (RTS and CTS signals)
  		          - Receive and transmit enabled
  		          - USART Clock disabled
  		          - USART CPOL: Clock is active low
  		          - USART CPHA: Data is captured on the middle
  		          - USART LastBit: The clock pulse of the last data bit is not output to
  		                           the SCLK pin
  	 */
  	USART_InitStructure.USART_BaudRate = baudrate;
  	switch (parity)
        { 
          case 'E': dataBits++; USART_InitStructure.USART_Parity = USART_Parity_Even; break;
          case 'O': dataBits++; USART_InitStructure.USART_Parity = USART_Parity_Odd; break;
          case 0:   USART_InitStructure.USART_Parity = USART_Parity_No; break;
          default: gGpioStatusCode = GpioStatus::UartWrongParity;
        }
        switch (dataBits)
        {
          case 8: USART_InitStructure.USART_WordLength = USART_WordLength_8b; break;
          case 9: USART_InitStructure.USART_WordLength = USART_WordLength_9b; break;
          default: gGpioStatusCode = GpioStatus::UartWrongDataBits;
        }
  	switch (stopBits)
        {
          case 5: USART_InitStructure.USART_StopBits = USART_StopBits_0_5; break;
          case 1: USART_InitStructure.USART_StopBits = USART_StopBits_1; break;
          case 15: USART_InitStructure.USART_StopBits = USART_StopBits_1_5; break;
          case 2: USART_InitStructure.USART_StopBits = USART_StopBits_2; break;
          default: gGpioStatusCode = GpioStatus::UartWrongStopBits;
        }
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  	USART_Init(USART3, &USART_InitStructure);
  }

  void uart_init()
  {
  	/* Enable USART3 and GPIOA clock */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  	/* NVIC Configuration */
  	NVIC_InitTypeDef NVIC_InitStructure;
  	/* Enable the USARTx Interrupt */
  	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

  	/* Configure the GPIOs */
  	GPIO_InitTypeDef GPIO_InitStructure;

  	/* Configure USART3 Tx (PB10) as alternate function push-pull */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

  	/* Configure USART3 Rx (PB11) as input floating */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

        uart_configure(9600);

  	/* Enable USART3 */
  	USART_Cmd(USART3, ENABLE);

  	/* Enable the USART3 Receive interrupt: this interrupt is generated when the
  		USART3 receive data register is not empty */
  	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
  }


  void uart_deinit()
  {
        //gGpioStatusCode = GpioStatus::UartInternalError;
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, DISABLE);

  	/* NVIC Configuration */
  	NVIC_InitTypeDef NVIC_InitStructure;
  	/* Enable the USARTx Interrupt */
  	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  	NVIC_Init(&NVIC_InitStructure);

  	/* Configure the GPIOs */
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

  	USART_Cmd(USART3, DISABLE);
  	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
  }

  void uart_write(uint8_t data)
  {
      USART_SendData(USART3, data);
      while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
  }
}

namespace FPGA
{
  void Init()
  {
    // init trigger
  }

  void delayMicroseconds(int us)
  {
    us = us*12;
    while (us--)
      __asm__("");
  }

  void Restart()
  {
    // We should reset trigger!
    constexpr int SMPL_CLR = 0x02;
    FPGA16(SMPL_CLR, 1, 0);
    delayMicroseconds(100); // 20, 50 - not enough
  }

  int Direct()
  {       
    constexpr int MEM_READ = 0x32;
    return FPGA32(MEM_READ, 3, 0) >> 28;
  }
}

namespace BIOS
{
  namespace GPIO
  {
    /*
      P1: PB15, PB10  remap   TIM2_CH3
      P2: PB14, PB11  remap   TIM2_CH4
      P3: PB13, PB8   default TIM4_CH3
      P4: PB12, PB9   default TIM4_CH4

      B10 - SCL
      B11 - SDA
    */
    static const uint8_t arrPinAdrAPort[4] = {PIN::PortB, PIN::PortB, PIN::PortB, PIN::PortB};
    static const uint8_t arrPinAdrAPin[4] = {15, 14, 13, 12};
    static const uint8_t arrPinAdrBPort[4] = {PIN::PortB, PIN::PortB, PIN::PortB, PIN::PortB};
    static const uint8_t arrPinAdrBPin[4] = {10, 11, 8, 9};

    EMode specialMode = Input;

    void DigitalWrite(EPin pin, bool value)
    {
      _ASSERT(pin < 4);
      PIN::SetPin(arrPinAdrBPort[pin], arrPinAdrBPin[pin], value);
    }

    bool DigitalRead(EPin pin)
    {
      if (pin >= CH1)
      {
        FPGA::Restart();
        return FPGA::Direct() & (1<<((int)pin-4));
      }
      return PIN::GetPin(arrPinAdrBPort[pin], arrPinAdrBPin[pin]);
    }

    void AnalogWrite(EPin pin, int value)
    {
      // check if pwm is running
      value = max(0, min(value, AnalogRange));

      switch (pin)
      {
        case P1: TIM_SetCompare3 (TIM2, value); break;
        case P2: TIM_SetCompare4 (TIM2, value); break;
        case P3: TIM_SetCompare3 (TIM4, value); break;
        case P4: TIM_SetCompare4 (TIM4, value); break;
        default: _ASSERT(0);
      }
    }

    int AnalogRead(EPin pin)
    {
      gGpioStatusCode = GpioStatus::NotImplemented;
      _ASSERT(0);
      return 0;
    }

    void PinMode(EPin pin, EMode mode)
    {
      if (pin >= CH1)
      {
        _ASSERT(mode == Input);
        FPGA::Init();
        return;
      }

      EMode testMode = mode;
      if ((testMode & Input) || (testMode & Output))
        testMode = Input;

      if (specialMode != testMode)
      {
        switch (specialMode)
        {
          case Pwm: 
            PWM::DisablePwm(); 
          break;
          case I2c: 
            ::I2C::i2c_deinit(); 
            PIN::SetState(arrPinAdrAPort[P1], arrPinAdrAPin[P1], PIN::StateSimpleInput);
            PIN::SetState(arrPinAdrAPort[P2], arrPinAdrAPin[P2], PIN::StateSimpleInput);
            PIN::SetState(arrPinAdrBPort[P1], arrPinAdrBPin[P1], PIN::StateSimpleInput);
            PIN::SetState(arrPinAdrBPort[P2], arrPinAdrBPin[P2], PIN::StateSimpleInput);
          break;
          case Uart:
            ::UART::uart_deinit(); 
            PIN::SetState(arrPinAdrAPort[P1], arrPinAdrAPin[P1], PIN::StateSimpleInput);
            PIN::SetState(arrPinAdrAPort[P2], arrPinAdrAPin[P2], PIN::StateSimpleInput);
          break;
          default: 
          break;
        }

        specialMode = testMode;

        switch (specialMode)
        {
          case Pwm:
            PIN::SetState(arrPinAdrAPort[P1], arrPinAdrAPin[P1], PIN::StateSimpleInput);
            PIN::SetState(arrPinAdrAPort[P2], arrPinAdrAPin[P2], PIN::StateSimpleInput);
            PIN::SetState(arrPinAdrAPort[P3], arrPinAdrAPin[P3], PIN::StateSimpleInput);
            PIN::SetState(arrPinAdrAPort[P4], arrPinAdrAPin[P4], PIN::StateSimpleInput);
            PIN::SetState(arrPinAdrBPort[P1], arrPinAdrBPin[P1], PIN::StateSimpleInput);
            PIN::SetState(arrPinAdrBPort[P2], arrPinAdrBPin[P2], PIN::StateSimpleInput);
            PIN::SetState(arrPinAdrBPort[P3], arrPinAdrBPin[P3], PIN::StateSimpleInput);
            PIN::SetState(arrPinAdrBPort[P4], arrPinAdrBPin[P4], PIN::StateSimpleInput);
            ::PWM::EnablePwm(); 
          break;
          case I2c:
            ::I2C::i2c_init(); 
          break;
          case Uart:
//            ::UART::uart_configure(9600); 
            ::UART::uart_init(); 
          break;
          default: break;
        }
      }

      // useless?
      //if (specialMode != I2c)
      //  PIN::SetState(arrPinAdrAPort[pin], arrPinAdrAPin[pin], PIN::StateSimpleInput);

      switch ((int)mode)
      {
        case Input:
          PIN::SetState(arrPinAdrBPort[pin], arrPinAdrBPin[pin], PIN::StateSimpleInput);
          break;
        case Input | PullUp:
          PIN::SetState(arrPinAdrBPort[pin], arrPinAdrBPin[pin], PIN::StateInput | PIN::StateInputPull);
          PIN::SetPin(arrPinAdrBPort[pin], arrPinAdrBPin[pin], 1);
          break;
        case Input | PullDown:
          PIN::SetState(arrPinAdrBPort[pin], arrPinAdrBPin[pin], PIN::StateInput | PIN::StateInputPull);
          PIN::SetPin(arrPinAdrBPort[pin], arrPinAdrBPin[pin], 0);
          break;
        case Output:
          PIN::SetState(arrPinAdrBPort[pin], arrPinAdrBPin[pin], PIN::StateSimpleOutput);
          break;
        case Pwm:
          AnalogWrite(pin, 0);
          break;
        case I2c:
          break;
        case Uart:
          break;
        default:
          _ASSERT(0);
      }
    }

    namespace I2C
    {
      int mCount{0};

      bool BeginTransmission(uint8_t address)
      {
          if (!::I2C::i2c_start())
          {
              ::I2C::i2c_stop(true);
              return false;
          }
          if (!::I2C::i2c_address_direction(address << 1, I2C_Direction_Transmitter))
          {
              ::I2C::i2c_stop(true);
              return false;
          }

        return true;
      }

      bool RequestFrom(uint8_t address, uint8_t bytes)
      {
        if (!::I2C::i2c_start())
          return false;
        if (!::I2C::i2c_address_direction(address << 1, I2C_Direction_Receiver))
          return false;

        mCount = bytes;
        return true;
      }

      bool Write(uint8_t data)
      {
        return ::I2C::i2c_transmit(data);
      }

      uint8_t Read()
      {
        if (mCount>0)
          mCount--;
        if (mCount>0)
          return ::I2C::i2c_receive_ack();
        else
          return ::I2C::i2c_receive_nack();
      }

      bool EndTransmission(bool stop)
      {
        return ::I2C::i2c_stop(stop);
      }
    }

    namespace UART
    {
      RingBuffer<uint8_t, 300> mUartBuffer;

      void Setup(int baudrate, EConfig config)
      {
        int dataBits = 8;
        char parity = 0;
        int stopBits = 1;

        if (config & EConfig::length9)
          dataBits = 9;
        if (config & EConfig::stopBits15)
          stopBits = 15;
        if (config & EConfig::stopBits2)
          stopBits = 2;
        if (config & EConfig::parityEven)
          parity = 'E';
        if (config & EConfig::parityOdd)
          parity = 'O';

        _ASSERT(!(config & EConfig::flowHw));

        ::UART::uart_configure(baudrate, dataBits, parity, stopBits); 
      }

      bool Available()
      {
        return mUartBuffer.size() > 0;
      }

      uint8_t Read()
      {
        _ASSERT(mUartBuffer.size() > 0);
        return mUartBuffer.pull();
      }

      void Write(uint8_t data)
      {
        ::UART::uart_write(data);
      }
    }

  }
}

// C Exports
extern "C" 
{
  void UartPushByte(uint8_t data)
  {
     if (BIOS::GPIO::UART::mUartBuffer.isFull())
       gGpioStatusCode = GpioStatus::UartErrorOverflow;
     BIOS::GPIO::UART::mUartBuffer.push(data);
  }

  void UartPushError(uint16_t error) 
  {
    if (error & USART_FLAG_ORE)
      gGpioStatusCode = GpioStatus::UartErrorOverrun;
    if (error & USART_FLAG_NE)
      gGpioStatusCode = GpioStatus::UartErrorNoise;
    if (error & USART_FLAG_FE)
      gGpioStatusCode = GpioStatus::UartErrorFraming;
    if (error & USART_FLAG_PE)
      gGpioStatusCode = GpioStatus::UartErrorParity;
  }
}
