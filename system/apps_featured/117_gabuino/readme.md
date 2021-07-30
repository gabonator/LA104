# Gabuino

In-browser development environment for STM32:
- ACE based C++ editor (/web/v1)
- GCC compiler service (/service) which will be replaced by webassembly GCC port later (to be able to use in offline mode)
- LA104 user application for flashing (/source) responsible for code flashing, execution and debugging over webusb
- Examples package (/examples)

The compiler is configured to place generated code into RAM, currently there are 8kB available which is plenty for simple experiments. Build times are 160ms for GCC running on the same machine or approx 500ms for cloud hosted compiler. Flashing and other stuff takes less than 20ms, this allows incredibly high development speed.
