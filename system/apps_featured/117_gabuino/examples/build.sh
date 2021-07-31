node build.js > ../web/v1/examples.js

arm-none-eabi-nm -a --print-size -C ../../../os_host/build/output_la104.elf > os_la104.txt
node ../service/nmparse.js os_la104.txt os_la104 > ../web/v1/dsym_os.js
rm os_la104.txt

arm-none-eabi-nm -a --print-size -C ../build/output.elf > gabuino.txt
node ../service/nmparse.js gabuino.txt gabuino > ../web/v1/dsym_gabuino.js
rm gabuino.txt
