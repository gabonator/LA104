# FPGA uploader and test tool

This application uploads a FPGA image and then runs a test to verify it. To build a FPGA image use Intel Quartus Subscription edition and Alta Gate Supra software. Follow this [tutorial](https://github.com/gabonator/LA104/discussions/81) to properly configure your building setup. 

Applications are placed in [system/fpga_la104](/system/fpga_la104) folder, in each application there are two folders: **source** holding the verilog source files and **verify** with C++ verification code which can be executed on PC/MAC with help of [verilator](https://www.veripool.org/verilator/). The same code runs on target machine (LA104) and is automatically executed when this loader finds the verification executable (it has the same name as FPGA image but with different extension). 

Fibonacci counter example:
  - Verilog source files: [app.v](/system/fpga_la104/03_fibonacci/source/app.v), [fibonacci.v](/system/fpga_la104/03_fibonacci/source/fibonacci.v)
  - Verification code: [verify.cpp](/system/fpga_la104/03_fibonacci/verify/verify.cpp)
  - Built FPGA image and verification module [system/apps/123_fpga/bin](/system/apps/123_fpga/bin)
