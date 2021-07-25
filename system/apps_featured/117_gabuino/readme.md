# Gabuino

In browser development IDE for STM32, consists of following parts:
- ACE based C++ editor (/web/v1)
- GCC compiler service (/service) which will be replaced by webassembly GCC port later. GCC compiler app
- LA104 user application for flashing (/source), code execution, debugging over webusb
- Examples package (/examples)

In browser editing, compiling on cloud hosted gcc, flashing the code&variables into RAM allow 300ms turnaround development time - 300 ms after taping the "build & run" the code actually runs on target, heavily improving the development and debugging speed on target.