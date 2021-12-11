echo "#include \"opencm3.h\"" > webusb.c
echo "#include \"webusb.h\"" >> webusb.c
echo "#include \"webstrings.h\"" >> webusb.c

cat ../source_win_minimal/usb_conf.h | grep -E -v "\#.*include" >> webusb.c
cat ../source_win_minimal/winusb_defs.h | grep -E -v "\#.*include" >> webusb.c
cat ../source_win_minimal/winusb.h | grep -E -v "\#.*include" >> webusb.c

cat ../source_win_minimal/webusb_defs.h | grep -E -v "\#.*include" >> webusb.c
cat ../source_win_minimal/webusb.h | grep -E -v "\#.*include" >> webusb.c
cat ../source_win_minimal/usb21_standard.h | grep -E -v "\#.*include" >> webusb.c
cat ../source_win_minimal/cdc.h | grep -E -v "\#.*include" >> webusb.c

cat ../source_win_minimal/usb_conf.c | grep -E -v "\#.*include" >> webusb.c
cat ../source_win_minimal/webusb.c | grep -E -v "\#.*include" | grep -v "\#define MIN" >> webusb.c

echo "#undef MIN" >> webusb.c
echo "#undef CONTROL_CALLBACK_TYPE" >> webusb.c
echo "#undef CONTROL_CALLBACK_MASK" >> webusb.c
cat ../source_win_minimal/cdc.c | grep -E -v "\#.*include" >> webusb.c

echo "#undef MIN" >> webusb.c
echo "#undef CONTROL_CALLBACK_TYPE" >> webusb.c
echo "#undef CONTROL_CALLBACK_MASK" >> webusb.c
cat ../source_win_minimal/winusb.c | grep -E -v "\#.*include" | grep -E -v ".*static int usb_descriptor.*" >> webusb.c

echo "#undef DESCRIPTOR_CALLBACK_TYPE" >> webusb.c
echo "#undef DESCRIPTOR_CALLBACK_MASK" >> webusb.c
cat ../source_win_minimal/usb21_standard.c | grep -E -v "\#.*include" >> webusb.c


echo "#ifdef __cplusplus" > webusb.h
echo "extern \"C\" {" >> webusb.h
echo "#endif" >> webusb.h
echo "" >> webusb.h
echo "typedef void (*cdc_receive_callback)(uint8_t* buf, int len);" >> webusb.h
echo "struct usbd_device;" >> webusb.h
echo "extern usbd_device* usb_setup(void);" >> webusb.h
echo "extern void usbd_poll(usbd_device *usbd_dev);" >> webusb.h
echo "extern void cdc_set_receive_callback(cdc_receive_callback callback);" >> webusb.h
echo "extern bool cdc_transmit(const uint8_t* buffer, int len);" >> webusb.h
echo "" >> webusb.h
echo "#ifdef __cplusplus" >> webusb.h
echo "}" >> webusb.h
echo "#endif" >> webusb.h
