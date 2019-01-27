#pragma once
#include <stdint.h>

typedef uint8_t boolean;
typedef uint8_t byte;

//#define cc1101_Select()
//#define cc1101_Deselect()
//#define wait_Miso()

void delayMicroseconds(int us);

class CSPI
{
public:
  void init();
  uint8_t send(uint8_t);
  void select() ;
  void deselect();
  void wait(); 
};
