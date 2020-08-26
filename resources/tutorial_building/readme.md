# Building LA104/DS203/DS213 operating system

## LA/DS application switching
- LA104 or DS203 normally allows switching between multiple applications by holding one of the four buttons during startup. This method is not flexible enough and does not allow to use more than three different applications without flashing the device.  
- All the LA/DS devices are equipped with eeprom (2MB or 8MB) which is used as small FAT12 disk drive. This drive is also accessible with USB connection as Mass Storage Device. Main idea for the operating system was to allow easy switching between any number of applications which are dynamically loaded and flashed from the eeprom on demand without restarting the device. 
- This operating system does not provide any advanced features as one should expect (no multitasking, no dynamic allocations). It just loads linux .ELF files into RAM and FLASH, resolves imported methods and jumps to the application entry point. During startup the OS tries to load file **shell.elf** which can be simple file manager or graphical application switcher, or an application that should be executed right after startup.
- There are at least two applications that need to be compiled and loaded into the device to use it - operating system and shell
- Building of the operating system will produce .hex file which needs to be flashed to the device using interal DFU flasher. It you are lucky, you will be able to copy the .hex file into the DFU's virtual mass storage device after connecting the device with your computer and turning it on while holding first button. If you will be having problems with this initial flashing, look for dfuload tool in [/tools](/tools) folder. All other applications compile into .elf file which can be copied to the mass storage disk when the device is in normal operation (not DFU)
- Regular .elf files contain lengthy parts, which are useless for the LA104. After compilation process, this file is stripped to reduce it's size by [/tools/elfstrip](/tools/elfstrip) tool
- Source code of the operating system is placed in [/system/os_host](/system/os_host) folder and simple file manager is in [/system/apps_shell/test29_fileman](/system/apps_shell/test29_fileman). For testing purpose one should compile also some simple application e.g. [/system/apps/test15_charmap](/system/apps/test15_charmap) or [/system/apps_experiments/test2_import](/system/apps_experiments/test2_import)
- Whole codebase is written in C/C++ with small bits of assembly. LA104 is based on STM32F103 arm processor and to be able to build this code, you will need **arm eabi toolchain** which can be downloaded here for any common platform:
https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
- In my case, I am using lightly outdated version (gcc-arm-none-eabi-7-2018-q2-update), but I suggest downloading lastest available version

## Mac OSX / Linux
- Clone whole repository (git clone https://github.com/gabonator/LA104.git)
- Building the OS: 
    - From the [/system/os_host](/system/os_host) folder, run **build_la104.sh** or **build_ds203.sh** or **build_ds213.sh**
    - You will need to change the path to your arm toolchain by changing this line or by exporing the arm toolchain path: `````` 
    - Check the output of the script, it should look like this
        ```~/Documents/git/LA104/system/os_host$ ./build_la104.sh 
        20000000 00000000 D _addressRamBegin
        200025e8 00000000 B _addressRamEnd
        08008000 00000000 T _addressRomBegin
        08013060 00000000 T _addressRomEnd
        ```
    - Check the build folder in [/system/os_host](/system/os_host), there should be file named **system_la104.hex**
- Building the library
  - Applications refer to methods exported by operating system. For this purpose we need to build a fake dynamic library which exports all available methods the operating system exposes for applications
    - Go to [/system/os_library](/system/os_library) and run **build.sh**
    - In the build folder, there should appear three files: libbios_ds203.so, libbios_ds213.so, libbios_la104.so
- Building shell:
  - Go to [/system/apps_shell/test29_fileman](/system/apps_shell/test29_fileman)
  - Fix the path to arm toolchain in **build.sh**
  - Run the **build.sh**
  - It will produce three application files: 29fileman_ds203.elf, 29fileman_ds213.elf, 29fileman_la104.elf
  - Take the **29fileman_la104.elf**, rename it to **shell.elf**
- Building sample application: 
  - Go to [/system/apps_experiments/test2_import](/system/apps_experiments/test2_import) which is the smallest available application which draws some fractals:
  ```C++
    #include <library.h>
    
    class CPRNG
    {
    private:
      unsigned int nSeed;
      unsigned int nX;
    
    public:
      CPRNG() : nSeed(5323), nX(0)
      {
      }
    
      unsigned int get()
      {
        nSeed = (8253729 * nSeed + 2396403);
        nSeed += nX++;
        return nSeed & 32767;
      }
    };
    
    __attribute__((__section__(".entry"))) int main(void);
    
    int main(void)
    { 
      CPRNG prng;
      int Width = BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::ScreenWidth);
      int Height = BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::ScreenHeight);
    
      for (int x=0; x<Width; x++)
        for (int y=0; y<Height; y++)
        {
          uint32_t dx = (x-Width/2);
          uint32_t dy = (y-Height/2);
          dx *= dx;
          dy *= dy;
          uint32_t dl = dx+dy;
    
          BIOS::LCD::PutPixel(x, y, (uint16_t)((dl>>3) & 0x001f)<<11);
        }
    
      int ax[3] = {10, 310, 140};
      int ay[3] = {20, 40, 220};
    
      int curx = ax[0];
      int cury = ay[0];
    
      while (BIOS::KEY::GetKey() == BIOS::KEY::None) 
      {
        int c = prng.get();
        int r = c%3;
        curx = (curx+ax[r]) / 2;
        cury = (cury+ay[r]) / 2;
    
        BIOS::LCD::PutPixel(curx, cury, c<<1);
      }
    
      return 0;
    }
    
    ```

    - Build it by running **build.sh** script, it will compile the code, link it with dummy library, and then remove unnecessary parts from the .elf file. Again you will need to fix the toolchain path:
    ```
    
    mkdir -p build
    cd build

    arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -MD -c ../main.cpp -I../../../os_library/include/
    arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds ./main.o -lbios_la104 -L../../../os_library/build

    ../../../../tools/elfstrip/elfstrip output.elf 2import.elf
    ```
    - If everything went well, it should produce **build/2import.elf** file
    
    ## Linux
    - Follow the OSX tutorial, provided scripts should work well
    - Linux is more strict with the mismatched filename cases, during the compilation with provided scripts, you will need to fix few typos by renaming the file names or changing the include paths in code
    
    ## Windows
    - Follow the OSX tutorial, but rewrite provided scripts into batch files, it should be pretty easy, since there are just 4-10 commands
    
    ## Flashing and first run
    - After finishing this tutorial you should have three files: **system_la104.hex**, **shell.elf** and **2import.elf**
    - Flash it with DFU loader by holding first button when powering the device. Use the **cp_la104.sh** script if you are having troubles copying the file to DFU drive (you will need to build the dfuload tool from [/tools/dfuload](/tools/dfuload) folder)
    - Turn the device off and on, and the OS should ask for **shell.elf**
    - After reconnecting the device using USB cable, you should be able to copy new files to the internal eeprom (different from the virtual DFU drive)
    - copy **shell.elf** and **2import.elf**, press first button to reload the shell or power cycle the device
    - you should see file manager with filesystem listing
    
