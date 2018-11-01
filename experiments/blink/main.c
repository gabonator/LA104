#include "stm32f10x_lib.h"
#include "pinio.h"

// Blinks led diode conneted to P1 pin

int main(void)
{ 
  volatile unsigned long i;
  
  while (1) 
  {
//    pinWrite(1, 15, 1); // 54 P1 MOSI_PB15
//    pinWrite(1, 10, 1); // 47 P1 T23_TX3_SCL2_PB10  
//    pinWrite(1, 14, 1); // 53 P2 MISO_PB14
//    pinWrite(1, 11, 1); // 48 P2 T24_RX3_SDA2_PB11  

    pinWrite(1, 10, 1); // PB10 - P1
    for (i=0; i<1000000; i++);

    pinWrite(1, 10, 0); // PB10 - P1
    for (i=0; i<1000000; i++);
  }

  return 0;
}
