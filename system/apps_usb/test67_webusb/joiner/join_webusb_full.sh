echo "#include \"opencm3.h\"" > webusb.h
cat ../source_full_clean/webusb.h | grep -E -v "\#.*include" >> webusb.h
cat ../source_full_clean/winusb.h | grep -E -v "\#.*include" >> webusb.h
cat ../source_full_clean/usb_conf.h | grep -E -v "\#.*include" >> webusb.h
cat ../source_full_clean/webusb_defs.h | grep -E -v "\#.*include" >> webusb.h
cat ../source_full_clean/winusb_defs.h | grep -E -v "\#.*include" >> webusb.h
cat ../source_full_clean/cdc.h | grep -E -v "\#.*include" >> webusb.h
cat ../source_full_clean/usb21_standard.h | grep -E -v "\#.*include" >> webusb.h

echo "#include \"webusb.h\"" > webusb.c
echo "const usbd_driver* target_usb_init(void);" >> webusb.c
echo "#define debug_println(x)" >> webusb.c
echo "#define debug_print(x)" >> webusb.c
echo "#define debug_printhex(x)" >> webusb.c
echo "#define debug_flush()" >> webusb.c
echo "#define debug_print_unsigned(x)" >> webusb.c
echo "#define dbg(x)" >> webusb.c

cat ../source_full_clean/usb_conf.c | grep -E -v "\#.*include" >> webusb.c
cat ../source_full_clean/cdc.c | grep -E -v "\#.*include" >> webusb.c
cat ../source_full_clean/usb21_standard.c | grep -E -v "\#.*include" | grep -v "\#define MIN" >> webusb.c
echo "#undef CONTROL_CALLBACK_TYPE" >> webusb.c
echo "#undef CONTROL_CALLBACK_MASK" >> webusb.c
echo "#undef DESCRIPTOR_CALLBACK_TYPE" >> webusb.c
cat ../source_full_clean/webusb.c | grep -E -v "\#.*include" | grep -v "\#define MIN" >> webusb.c
echo "#undef CONTROL_CALLBACK_TYPE" >> webusb.c
echo "#undef CONTROL_CALLBACK_MASK" >> webusb.c
echo "#undef DESCRIPTOR_CALLBACK_MASK" >> webusb.c
echo "extern int usb_descriptor_type(uint16_t wValue);" >> webusb.c
echo "extern int usb_descriptor_index(uint16_t wValue);" >> webusb.c
cat ../source_full_clean/winusb.c | grep -E -v "\#.*include" | grep -v "\#define MIN" | grep -E -v "static int usb_descriptor" >> webusb.c

echo "" >> webusb.c
echo "const usbd_driver* target_usb_init(void)" >> webusb.c
echo "{" >> webusb.c
echo "    return &st_usbfs_v1_usb_driver;" >> webusb.c
echo "}" >> webusb.c
