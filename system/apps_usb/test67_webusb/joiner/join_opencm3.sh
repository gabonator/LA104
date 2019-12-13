echo \#include \"stdint.h\" > opencm3.h
echo \#include \"stdbool.h\" >> opencm3.h
echo \#include \"stdlib.h\" >> opencm3.h
echo \#include \"string.h\" >> opencm3.h

cat ../lib/libopencm3/include/libopencm3/cm3/common.h | grep -E -v "\#.*include" >> opencm3.h
cat ../lib/libopencm3/include/libopencm3/cm3/memorymap.h | grep -E -v "\#.*include" >> opencm3.h
cat ../lib/libopencm3/include/libopencm3/cm3/assert.h | grep -E -v "\#.*include" >> opencm3.h
cat ../lib/libopencm3/include/libopencm3/usb/usbstd.h | grep -E -v "\#.*include" >> opencm3.h
cat ../lib/libopencm3/include/libopencm3/usb/cdc.h | grep -E -v "\#.*include" >> opencm3.h
cat ../lib/libopencm3/include/libopencm3/usb/usbd.h | grep -E -v "\#.*include" >> opencm3.h
cat ../lib/libopencm3/include/libopencm3/stm32/desig.h | grep -E -v "\#.*include" >> opencm3.h
cat ../lib/libopencm3/include/libopencm3/stm32/st_usbfs.h | grep -E -v "\#.*include" >> opencm3.h
cat ../lib/libopencm3/include/libopencm3/stm32/memorymap.h | grep -E -v "\#.*include" >> opencm3.h
cat ../lib/libopencm3/include/libopencm3/stm32/tools.h | grep -E -v "\#.*include" >> opencm3.h
cat ../lib/libopencm3/include/libopencm3/stm32/f1/st_usbfs.h | grep -E -v "\#.*include" >> opencm3.h
cat ../lib/libopencm3/include/libopencm3/stm32/f1/memorymap.h | grep -E -v "\#.*include" >> opencm3.h
cat ../lib/libopencm3/include/libopencm3/stm32/f1/rcc.h | grep -E -v "\#.*include" >> opencm3.h
cat ../lib/libopencm3/include/libopencm3/stm32/rcc.h | grep -E -v "\#.*include" >> opencm3.h

cat ../lib/libopencm3/include/libopencm3/stm32/common/rcc_common_all.h | grep -E -v "\#.*include" >> opencm3.h
cat ../lib/libopencm3/include/libopencm3/stm32/common/st_usbfs_common.h | grep -E -v "\#.*include" >> opencm3.h
cat ../lib/libopencm3/include/libopencm3/stm32/common/st_usbfs_v1.h | grep -E -v "\#.*include" >> opencm3.h

cat ../lib/libopencm3/lib/usb/usb_private.h | grep -E -v "\#.*include" >> opencm3.h
cat ../lib/libopencm3/lib/stm32/common/st_usbfs_core.h | grep -E -v "\#.*include" >> opencm3.h

echo \#include \"opencm3.h\" > opencm3.c
cat ../lib/libopencm3/lib/cm3/assert.c | grep -E -v "\#.*include" >> opencm3.c
cat ../lib/libopencm3/lib/usb/usb_control.c | grep -E -v "\#.*include" >> opencm3.c
cat ../lib/libopencm3/lib/usb/usb.c | grep -E -v "\#.*include" >> opencm3.c
cat ../lib/libopencm3/lib/usb/usb_standard.c | grep -E -v "\#.*include" >> opencm3.c
#cat ../lib/libopencm3/lib/stm32/st_usbfs_v2.c | grep -E -v "\#.*include" >> opencm3.c
cat ../lib/libopencm3/lib/stm32/st_usbfs_v1.c | grep -E -v "\#.*include" >> opencm3.c
cat ../lib/libopencm3/lib/stm32/common/st_usbfs_core.c | grep -E -v "\#.*include" >> opencm3.c
cat ../lib/libopencm3/lib/stm32/common/rcc_common_all.c | grep -E -v "\#.*include" >> opencm3.c
cat ../lib/libopencm3/lib/stm32/desig.c | grep -E -v "\#.*include" >> opencm3.c
#cat ../lib/libopencm3/lib/stm32/f1/rcc.c | grep -E -v "\#.*include" >> opencm3.c
