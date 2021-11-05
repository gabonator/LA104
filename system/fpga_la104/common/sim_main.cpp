#include <verilated.h>

#include "Vapp.h"
#include "verify.cpp"

Vapp* CFpgaTestBench::mTop{nullptr};

int main(int argc, char** argv, char** env) {
    Verilated::debug(0);
    Verilated::randReset(2);
    Verilated::traceEverOn(true);
    Verilated::commandArgs(argc, argv);
    Verilated::mkdir("logs");

    Vapp* top = new Vapp;  // Or use a const unique_ptr, or the VL_UNIQUE_PTR wrapper

#ifndef _NOSPI
    top->SCK = 1;
    top->SSEL = 1;
    top->MOSI = 0;
    top->clk = 0;
#endif

    CFpgaTestBench::mTop = top;

    if (verify())
      printf("Test ok!\n");
    else
      printf("Test failed!\n");

    CFpgaTestBench::mTop = nullptr;

    top->final();

    delete top;
    top = NULL;
    return 0;
}
