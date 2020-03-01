export PATH="/Users/gabrielvalky/Downloads/gcc-arm-none-eabi-7-2018-q2-update/bin/":"$PATH"
arm-none-eabi-objcopy --input-target=ihex --output-target=binary ds213v20.hex ds213v20.bin
