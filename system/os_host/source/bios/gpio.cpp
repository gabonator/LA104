#include "Bios.h"

#include <stm32f10x_tim.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>

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

  /*
  #define PERIPH_BASE           ((u32)0x40000000)
  #define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
  #define GPIOA_BASE            (APB2PERIPH_BASE + 0x0800)
  #define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)
  #define GPIOC_BASE            (APB2PERIPH_BASE + 0x1000)
  #define GPIOD_BASE            (APB2PERIPH_BASE + 0x1400)
  #define GPIOE_BASE            (APB2PERIPH_BASE + 0x1800)
  #define GPIOF_BASE            (APB2PERIPH_BASE + 0x1C00)
  #define GPIOG_BASE            (APB2PERIPH_BASE + 0x2000)
  */
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

  void TimerInit(uint16_t period) 
  {
    TIM_TimeBaseInitTypeDef timerInitStructure;

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
      gpioStructure.GPIO_Speed = GPIO_Speed_50MHz ; 
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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, DISABLE);
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

  void er(const char* m)
  {
    BIOS::DBG::Print("#%s", m);
  }

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

  void i2c_deinit()
  {
      GPIO_InitTypeDef GPIO_InitStruct;
   
      RCC_APB1PeriphClockCmd(I2Cx_RCC, DISABLE);
      I2C_Cmd(I2Cx, DISABLE);
   
      RCC_APB2PeriphClockCmd(I2C_GPIO_RCC, DISABLE);
      GPIO_InitStruct.GPIO_Pin = I2C_PIN_SCL | I2C_PIN_SDA;
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
      GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(I2C_GPIO, &GPIO_InitStruct);
  }
   
  void i2c_start()
  {
      // Wait until I2Cx is not busy anymore
      SetTimeout(2000);
      while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
        if (Timeout()) {er("b1"); return;}
      
   
      // Generate start condition
      I2C_GenerateSTART(I2Cx, ENABLE);
   
      // Wait for I2C EV5. 
      // It means that the start condition has been correctly released 
      // on the I2C bus (the bus is free, no other devices is communicating))
      while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
        if (Timeout()) {er("b2"); return;}
  }
   
  void i2c_stop()
  {
      // Generate I2C stop condition
      I2C_GenerateSTOP(I2Cx, ENABLE);
      // Wait until I2C stop condition is finished
      SetTimeout(100);
      while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF))
        if (Timeout()) {er("e1"); return;};
  }
   
  void i2c_address_direction(uint8_t address, uint8_t direction)
  {
      // Send slave address
      I2C_Send7bitAddress(I2Cx, address, direction);
   
      // Wait for I2C EV6
      // It means that a slave acknowledges his address
      SetTimeout(1000);
      if (direction == I2C_Direction_Transmitter)
      {
          while (!I2C_CheckEvent(I2Cx,
              I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
            if (Timeout()) {er("a1"); return;};

      }
      else if (direction == I2C_Direction_Receiver)
      { 
          while (!I2C_CheckEvent(I2Cx,
              I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
            if (Timeout()) {er("a2"); return;};
      }
  }
   
  void i2c_transmit(uint8_t byte)
  {
      // Send data byte
      I2C_SendData(I2Cx, byte);
      // Wait for I2C EV8_2.
      // It means that the data has been physically shifted out and 
      // output on the bus)
      SetTimeout(500);
      while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
            if (Timeout()) {er("t1"); return;};
  }
   
  uint8_t i2c_receive_ack()
  {
      // Enable ACK of received data
      I2C_AcknowledgeConfig(I2Cx, ENABLE);
      // Wait for I2C EV7
      // It means that the data has been received in I2C data register
      SetTimeout(500);
      while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
            if (Timeout()) {er("r1"); return 0x00;};
   
      // Read and return data byte from I2C data register
      return I2C_ReceiveData(I2Cx);
  }
   
  uint8_t i2c_receive_nack()
  {
      // Disable ACK of received data
      I2C_AcknowledgeConfig(I2Cx, DISABLE);
      // Wait for I2C EV7
      // It means that the data has been received in I2C data register
      SetTimeout(500);
      while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
            if (Timeout()) {er("r2"); return 0x00;};
   
      // Read and return data byte from I2C data register
      return I2C_ReceiveData(I2Cx);
  }
}

namespace UART
{
  void uart_configure(int baudrate)
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
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
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
// TODO: called unexpectedly!!!
        if (1) return;
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

namespace BIOS
{
  namespace GPIO
  {
    /*
      P1: PB15, PB10  remap   TIM2_CH3
      P2: PB14, PB11  remap   TIM2_CH4
      P3: PB13, PB8   default TIM4_CH3
      P4: PB12, PB9   default TIM4_CH4
    */
    static const uint8_t arrPinAdrAPort[4] = {PIN::PortB, PIN::PortB, PIN::PortB, PIN::PortB};
    static const uint8_t arrPinAdrAPin[4] = {15, 14, 13, 12};
    static const uint8_t arrPinAdrBPort[4] = {PIN::PortB, PIN::PortB, PIN::PortB, PIN::PortB};
    static const uint8_t arrPinAdrBPin[4] = {10, 11, 8, 9};

    EMode specialMode = Input;

    void DigitalWrite(EPin pin, bool value)
    {
      PIN::SetPin(arrPinAdrBPort[pin], arrPinAdrBPin[pin], value);
    }

    bool DigitalRead(EPin pin)
    {
      return PIN::GetPin(arrPinAdrBPort[pin], arrPinAdrBPin[pin]);
    }

    void AnalogWrite(EPin pin, int value)
    {
      value = max(0, min(value, AnalogRange));

      switch (pin)
      {
        case P1: TIM_SetCompare3 (TIM2, value); break;
        case P2: TIM_SetCompare4 (TIM2, value); break;
        case P3: TIM_SetCompare3 (TIM4, value); break;
        case P4: TIM_SetCompare4 (TIM4, value); break;
      }
    }

    int AnalogRead(EPin pin)
    {
      return 0;
    }

    void PinMode(EPin pin, EMode mode)
    {
      if ((mode == Pwm || mode == I2c || mode == Uart) && specialMode != mode)
      {
        switch (specialMode)
        {
          case Pwm: 
            PWM::DisablePwm(); 
            PIN::SetState(arrPinAdrAPort[P1], arrPinAdrAPin[P1], PIN::StateSimpleInput);
            PIN::SetState(arrPinAdrAPort[P2], arrPinAdrAPin[P2], PIN::StateSimpleInput);
            PIN::SetState(arrPinAdrAPort[P3], arrPinAdrAPin[P3], PIN::StateSimpleInput);
            PIN::SetState(arrPinAdrAPort[P4], arrPinAdrAPin[P4], PIN::StateSimpleInput);
          break;
          case I2c: 
            ::I2C::i2c_deinit(); 
            PIN::SetState(arrPinAdrAPort[P1], arrPinAdrAPin[P1], PIN::StateSimpleInput);
            PIN::SetState(arrPinAdrAPort[P2], arrPinAdrAPin[P2], PIN::StateSimpleInput);
          break;
          case Uart:
            ::UART::uart_deinit(); 
            PIN::SetState(arrPinAdrAPort[P1], arrPinAdrAPin[P1], PIN::StateSimpleInput);
            PIN::SetState(arrPinAdrAPort[P2], arrPinAdrAPin[P2], PIN::StateSimpleInput);
          break;
          default: 
          break;
        }

        specialMode = mode;

        switch (specialMode)
        {
          case Pwm:
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
          break;
        case Input | PullDown:
          _ASSERT(0);
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
      uint8_t mAddress;
      bool mTransmitting{false};

      bool BeginTransmission(uint8_t address)
      {
        mAddress = address;
        mTransmitting = false;
        return true;
      }

      bool RequestFrom(uint8_t address, uint8_t bytes)
      {
        ::I2C::i2c_start();
        ::I2C::i2c_address_direction(address << 1, I2C_Direction_Receiver);
        mCount = bytes;
        return true;
      }

      bool Write(uint8_t data)
      {
        if (!mTransmitting)
        {
          mTransmitting = true;
          ::I2C::i2c_start();
          ::I2C::i2c_address_direction(mAddress << 1, I2C_Direction_Transmitter); 
        }
        ::I2C::i2c_transmit(data);
        return true;
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

      bool EndTransmission()
      {
        ::I2C::i2c_stop();
        return true;
      }
    }

    namespace UART
    {
      RingBuffer<uint8_t, 64> mUartBuffer;

      void Setup(int baudrate, EConfig config)
      {
        _ASSERT((int)config == 0);
        ::UART::uart_configure(9600); 
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
     BIOS::GPIO::UART::mUartBuffer.push(data);
  }
}
