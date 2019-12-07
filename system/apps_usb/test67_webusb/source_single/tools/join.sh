cd source_single

echo \#include \"webusb.h\" > webusb.c
cat ../source_minimal/usb_conf.h | grep -v \#include >> webusb.c
cat ../source_minimal/webusb.h | grep -v \#include >> webusb.c
cat ../source_minimal/webusb_defs.h | grep -v \#include >> webusb.c
cat ../source_minimal/cdc.h | grep -v \#include >> webusb.c

cat ../source_minimal/usb_conf.c | grep -v \#include >> webusb.c
cat ../source_minimal/webusb.c | grep -v \#include >> webusb.c
cat ../source_minimal/cdc.c | grep -v \#include >> webusb.c
cat ../lib/libopencm3/lib/usb/usb.c | grep -v \#include >> webusb.c
#cat ../lib/libopencm3/lib/usb/usb_standard.c | grep -v \#include  >> webusb.c
cat ../lib/libopencm3/lib/usb/usb_control.c| grep -v \#include  >> webusb.c
cat ../lib/libopencm3/lib/stm32/common/st_usbfs_core.c | grep -v \#include >> webusb.c
cat ../lib/libopencm3/lib/stm32/common/rcc_common_all.c | grep -v \#include >> webusb.c
cat ../lib/libopencm3/lib/stm32/desig.c | grep -v \#include >> webusb.c
cat ../lib/libopencm3/lib/stm32/st_usbfs_v1.c | grep -v \#include >> webusb.c
cat ../lib/libopencm3/lib/cm3/assert.c | grep -v \#include >> webusb.c


exit 1

cat ../source_minimal/usb_conf.c | grep \#include | grep -v \" > webusb.h
exit 1
cat ../source_minimal/webusb.c | grep \#include | grep -v \"  >> webusb.h
cat ../source_minimal/cdc.c | grep \#include | grep -v \"  >> webusb.h
cat ../lib/libopencm3/lib/usb/usb.c | grep \#include | grep -v \"  >> webusb.h
cat ../lib/libopencm3/lib/usb/usb_standard.c | grep \#include | grep -v \"  >> webusb.h
cat ../lib/libopencm3/lib/usb/usb_control.c | grep \#include | grep -v \"  >> webusb.h
cat ../lib/libopencm3/lib/stm32/common/st_usbfs_core.c | grep \#include | grep -v \"  >> webusb.h
cat ../lib/libopencm3/lib/stm32/common/rcc_common_all.c | grep \#include | grep -v \"  >> webusb.h
cat ../lib/libopencm3/lib/stm32/desig.c | grep \#include | grep -v \"  >> webusb.h
cat ../lib/libopencm3/lib/stm32/st_usbfs_v1.c | grep \#include | grep -v \"  >> webusb.h
cat ../lib/libopencm3/lib/cm3/assert.c | grep \#include | grep -v \"  >> webusb.h



#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>
#include <libopencm3/stm32/desig.h>
#include <libopencm3/stm32/st_usbfs.h>
#include <libopencm3/stm32/rcc.h>
#include "../lib/libopencm3/lib/usb/usb_private.h"
#include "../lib/libopencm3/lib/stm32/common/st_usbfs_core.h"

 arm-none-eabi-gcc -E -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -DSTM32F1 -DLA104 -MD -D _ARM -D STM32F10X_HD -c \
  ../source_single/webusb.h \
  -I../source/ \
  -I../../../os_library/include/ \
  -I ../lib/libopencm3/include \
  -I ../../../os_host/library/CMSIS/Include \
  -I ../../../os_host/library/STM32_USB-FS-Device_Driver/inc | grep -v \# > moj.h
