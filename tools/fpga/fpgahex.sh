export PATH="/Users/gabrielvalky/Downloads/gcc-arm-none-eabi-7-2018-q2-update/bin/":"$PATH"
arm-none-eabi-objcopy --input-target=binary --output-target=ihex --change-addresses 0x08070000 ds213_v20_fpga ds213v20_fpga.hex 
