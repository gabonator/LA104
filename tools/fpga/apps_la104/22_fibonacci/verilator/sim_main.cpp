#include <verilated.h>

#include "Vapp.h"

vluint64_t main_time = 0;

void SpiCs(Vapp* top, bool cs)
{
    top->clk = !top->clk;
    top->eval();
    top->clk = !top->clk;
    top->eval();

    top->SSEL = !cs;

    top->clk = !top->clk;
    top->eval();
    top->clk = !top->clk;
    top->eval();
}


uint8_t SpiTick(Vapp* top, bool b)
{
    top->MOSI = b;

    top->clk = !top->clk;
    top->eval();
    top->clk = !top->clk;
    top->eval();

    top->SCK = 1;

    top->clk = !top->clk;
    top->eval();
    top->clk = !top->clk;
    top->eval();

    top->SCK = 0;

    top->clk = !top->clk;
    top->eval();
    top->clk = !top->clk;
    top->eval();

//    printf("%d", top->MOSI);
    printf("%d", top->MISO);
    return top->MISO;
}

uint8_t SpiSend(Vapp* top, uint8_t byte)
{
  uint8_t aux = 0;
  for (int i=0; i<8; i++)
  {
    aux <<= 1;
    aux |= SpiTick(top, byte & 128);
    byte <<= 1;
  }
  return aux;
}

void SpiSend(Vapp* top, uint8_t* buf, int count)
{
    SpiCs(top, true);
    for (int i=0; i<count; i++)
      buf[i] = SpiSend(top, buf[i]);
    SpiCs(top, false);
}

int main(int argc, char** argv, char** env) {
    Verilated::debug(0);
    Verilated::randReset(2);
    Verilated::traceEverOn(true);
    Verilated::commandArgs(argc, argv);
    Verilated::mkdir("logs");

    Vapp* top = new Vapp;  // Or use a const unique_ptr, or the VL_UNIQUE_PTR wrapper

    top->SCK = 1;
    top->SSEL = 1;
    top->MOSI = 0;
    top->clk = 0;

    int fib_a = 0;
    int fib_b = 1;
    int count = 50;

    SpiCs(top, true);
    for (int i=0; i<count; i++)
    {
      int ret = SpiSend(top, 0);
      printf("%02x ", ret);
      if (ret != fib_a)
      {
        printf("Failed on iteration %d, Expected 0x%02x got 0x%02x!\n", i, fib_a, ret);
        break;
      }
      int temp = fib_a + fib_b;
      fib_a = (uint8_t)fib_b;
      fib_b = temp;
    }
    SpiCs(top, false);

    top->final();

    delete top;
    top = NULL;
    return 0;
}
