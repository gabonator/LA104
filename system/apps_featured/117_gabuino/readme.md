# Gabuino

In browser development ide, consists of following parts:
- ACE based C++ editor
- GCC compiler service (/service) which will be replaced by webassembly GCC port later
- User application for flashing, code execution, debugging over webusb

This approach allows 300ms turnaround development time - 300 ms after taping the "build & run" the code actually runs on target