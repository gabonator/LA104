#include <verilated.h>

#include "Vapp.h"

vluint64_t main_time = 0;

void SpiCs(Vapp* top, bool cs)
{
    top->SCK = 1;
    top->SSEL = !cs;

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

    top->SCK = 0;

    top->clk = !top->clk;
    top->eval();
    top->clk = !top->clk;
    top->eval();

    top->SCK = 1;

    top->clk = !top->clk;
    top->eval();
    top->clk = !top->clk;
    top->eval();

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

    top->clk = 0;

    uint8_t check[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x07};
    uint8_t buf[8] = {0};
    SpiSend(top, buf, sizeof(buf));
    for (int i=0; i<sizeof(buf); i++)
      printf("%02x ", buf[i]);
    printf("\n");

    if (memcmp(check, buf, sizeof(check))==0)
      printf("Test ok!\n");
    else
      printf("Test failed!\n");

    top->final();

    delete top;
    top = NULL;
    return 0;
}
