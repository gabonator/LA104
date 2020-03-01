#70000-7d800
dd if=gabo_flash.bin bs=1 count=$((0xd800)) skip=$((0x70000)) of=gabo_flash_fpga
dd if=ds213v20.bin bs=1 count=$((0xd800)) skip=$((0x70000)) of=ds213_v20_fpga