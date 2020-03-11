# Flashing DS213:

## Requirements:
- ST Link V2 adapter (search for "ST-Link V2 stlink mini STM8STM32" on ebay or alibaba, it costs about $2)
- OpenOcd installed (http://openocd.org/getting-openocd/)
- Micro usb to ST Link cable - DS213 has SWD interface on USB port (black -> GND, green -> SWDIO, white -> SWCLK)

## Flashing procedue:

- At first we need to convert the flash dump (DS213 APP V.20.hex) info binary format

```
arm-none-eabi-objcopy --input-target=ihex --output-target=binary "DS213 APP V.20.hex" ds213v20.bin
```

- Install and run openocd

```
openocd -f /usr/local/share/openocd/scripts/interface/stlink.cfg -f /usr/local/share/openocd/scripts/target/stm32f1x.cfg
```

- It should respond with this:

```
Open On-Chip Debugger 0.10.0+dev-00957-g9de7d9c8 (2019-11-09-23:50)
Licensed under GNU GPL v2
For bug reports, read
        http://openocd.org/doc/doxygen/bugs.html
Info : auto-selecting first available session transport "hla_swd". To override use 'transport select <transport>'.
Info : The selected transport took over low-level target control. The results might differ compared to plain JTAG/SWD
Info : Listening on port 6666 for tcl connections
Info : Listening on port 4444 for telnet connections
Info : clock speed 1000 kHz
Info : STLINK V2J29S7 (API v2) VID:PID 0483:3748
Info : Target voltage: 3.271713
Info : stm32f1x.cpu: hardware has 6 breakpoints, 4 watchpoints
Info : Listening on port 3333 for gdb connections
```

- open new terminal window and open port 4444 (use telnet or screen, or putty)

```
telnet localhost 444
```

```
Trying ::1...
telnet: connect to address ::1: Connection refused
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
Open On-Chip Debugger
> halt
> flash banks
#0 : stm32f1x.flash (stm32f1x) at 0x08000000, size 0x00000000, buswidth 0, chipwidth 0

> flash info 0
device id = 0x10036414
flash size = 512kbytes
#0 : stm32f1x at 0x08000000, size 0x00080000, buswidth 0, chipwidth 0
	#  0: 0x00000000 (0x1000 4kB) not protected
	#  1: 0x00001000 (0x1000 4kB) not protected
	#  2: 0x00002000 (0x1000 4kB) not protected
	#  3: 0x00003000 (0x1000 4kB) not protected
	#  4: 0x00004000 (0x1000 4kB) not protected
	#  5: 0x00005000 (0x1000 4kB) not protected
	#  6: 0x00006000 (0x1000 4kB) not protected
	#  7: 0x00007000 (0x1000 4kB) not protected
	#  8: 0x00008000 (0x1000 4kB) not protected
	#  9: 0x00009000 (0x1000 4kB) not protected
	# 10: 0x0000a000 (0x1000 4kB) not protected
	# 11: 0x0000b000 (0x1000 4kB) not protected
	# 12: 0x0000c000 (0x1000 4kB) not protected
	# 13: 0x0000d000 (0x1000 4kB) not protected
	# 14: 0x0000e000 (0x1000 4kB) not protected
	# 15: 0x0000f000 (0x1000 4kB) not protected
	# 16: 0x00010000 (0x1000 4kB) not protected
	# 17: 0x00011000 (0x1000 4kB) not protected
	# 18: 0x00012000 (0x1000 4kB) not protected
	# 19: 0x00013000 (0x1000 4kB) not protected
	# 20: 0x00014000 (0x1000 4kB) not protected
	# 21: 0x00015000 (0x1000 4kB) not protected
	# 22: 0x00016000 (0x1000 4kB) not protected
	# 23: 0x00017000 (0x1000 4kB) not protected
	# 24: 0x00018000 (0x1000 4kB) not protected
	# 25: 0x00019000 (0x1000 4kB) not protected
	# 26: 0x0001a000 (0x1000 4kB) not protected
	# 27: 0x0001b000 (0x1000 4kB) not protected
	# 28: 0x0001c000 (0x1000 4kB) not protected
	# 29: 0x0001d000 (0x1000 4kB) not protected
	# 30: 0x0001e000 (0x1000 4kB) not protected
	# 31: 0x0001f000 (0x61000 388kB) not protected
STM32F10x (High Density) - Rev: Y

> flash erase_sector 0 0 last      
erased sectors 0 through 255 on flash bank 0 in 0.798275s

> flash write_bank 0 ds213v20.bin
wrote 524288 bytes from file ds213v20.bin to flash bank 0 at offset 0x00000000 in 15.454797s (33.129 KiB/s)
```
- Or you can just call the openocd tool to do the flashing automatically:
```
openocd -f /usr/local/share/openocd/scripts/interface/stlink.cfg -f /usr/local/share/openocd/scripts/target/stm32f1x.cfg -c init -c halt -c "flash erase_sector 0 0 last" -c reset -c halt -c "flash write_bank 0 ds213v20.bin" -c reset
```
## Known issues

- The device is fully working and only shows "System licence error!" message during startup

## Extras - Visual studio code target debugging
- launch.json - configuration file for microsoft visual studio code for debugging the code running on arm using open ocd with stlink v2 adapter with Cortex-Debug extension by marus25

## Credits:
- darkonerakot

