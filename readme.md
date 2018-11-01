# LA104 pocket logic analyser operating systems

LA104 is another nice piece of handheld hardware coming from [e-design / miniware company](http://www.miniware.com.cn/) similar to their previous product - pocket digital storage oscillosope DS203. 

This time they designed wholly digital device without analog circuitry featuring 4 general purpose IO signals and 4 digital inputs processed by [AG1K FPGA chip](http://www.alta-gate.com/products.aspx?id=43&p=14), the processor used is the same as in DS203 - STM32F103VCT6. Device is battery powered, charged through USB micro connector, its display offers resolution of 320x240 pixels. Check the scheme [here](resources/LA104 V1.5B_SCH.pdf)

![Product brief](resources/productBrief.png)

Flashing is done by holding first function button during powering up and by copying firmware HEX file onto the virtual flash USB drive. This could be tricky, since this approach does not work on Mac OSX. For some reason the default copying procedure after mounting this USB drive on OSX is different than the programmers at e-design expected, it works reliably only on Windows machines. During extensive research and testing I found a method how to upload firmware on any unix/linux operating system. Just build [tools/dfuload](tools/dfuload) by running build.sh script and you are ready to flash new firmwares. 

When I was developing alternative firmware for DS203, I was trying to design some simple operating system that would allow me to upload easily new programs to the devie with ability to switch between them without flashing it again. Something similar that [jpa did before](https://jpa.kapsi.fi/dsoquad/) with his PAWN interpreter on DS203 with a little difference that the application will be native C/C++ compiled code offering full computational power of the device. Unfortunately the GCC toolchain I was using had some bug which produced ARM32 instructions in PLT section which are not supported by STM32F103 and I was not able to force the compiler to use Thumb instruction set. So the dynamic relocation of imported symbols was an impossible task to implement. Now after few years, this problem was fixed and finally I could continue developing this operating system I started before.

#### In this repository you will find

  - [resources](resources) - official resources from manufacturer, shematics and memory layout information
  - [experiments](experiments) - the first simple applications to verify if we have proper toolchain, libraries, linker script and startup code
      - blink - When getting familiar with new hardware platform, this is the most common application - just blink a LED attached to P1 port. It is worth noting that the P1..P4 pins are connected always to two different IO pins of the MCU. This allows you to use each pin as general purpose IO pin, or as UART or I2C bus signal. For example pin P1 is connected to MOSI_PB15 and T23_TX3_SCL2_PB10. And P2 is connected to MISO_PB14 and T24_RX3_SDA2_PB11. So be very careful when configuring the IO pins so you won't create a short circuit 
    - triangle_oldlib - code taken from DS203 project. Simple sierpinski fractal to test LCD in C language
    - triangle_newlib - the same code, but with new library (2012 instead of 2008), this library is taken from official LA104 firmware project  
    - triangle_cpp - the same code, but in C++ with linker script that supports C++ features
    - worm - project taken from DS203, simple worm game but the controls are not very intuitive. Just a demonstration of simple event based window system
    - dynamic_simple - first step towards building own operating system, main application ELF just calls few imported functions (Test1, Test2, Test3, Test4). Imported library (shared object) has only empty implementation of those functions Test1..Test4 functions so the linker won't shout at us during compilation. Then by using [tools/elfdump](tools/elfdump) utility, a C code [dump.c](dynamic_simple/dump/dump.c) is generated which shows what will be loaded in which memory regions during ELF load process. [Secondary application](dynamic_simple/secondary/main.c) then calls this generated code and matches all imports into real functions
    - dynamic_advanced - the same, but this time the code is organized in three folders - library, host and client application. Instead of dummy functions, this time imported functions are putpixel and random.
- [official_fw_gcc](official_fw_gcc) - official LA104 firmware ported to use GCC compiler. No need to use IAR workbench. Only slight changes were done in the code - e.g. replacing sprintf function with malloc independent version of this function. 
- tools - various commandline tools
       - [dfuload](tools/dfuload) - DFU load utility mentioned before
       - [elfstrip](tools/elfstrip) - utlity to remove unnecessary parts from ELF file
       - [elfdump](tools/elfdump) - shows some information about ELF file structure and dumps used memory regions as C code
- [system](system) - LA104 custom firmware / operating system 
        - [bin](system/bin) - here you will find main firmware HEX file (manager.hex) and few interesting applications
        - os_host - operating system implementation with file manager and ELF loader
        - os_library - dummy library used by client applications
        - apps_experiments - TBD
        - apps_arduino - demonstration how to use arduino libraries in LA104
        - apps - TBD
