static const char* origin_url = "visualbluepill.github.io";
static char serial_number[24+1] = "001122334455";

static const char *usb_strings[] = {
    "Devanarchy",              //  USB Manufacturer
    "DAPBoot DFU Bootloader",  //  USB Product
    serial_number,             //  Serial number
    //"Blue Pill DFU",         //  DFU
    "DAPBoot DFU",             //  DFU
    "Blue Pill MSC",           //  MSC
    "Blue Pill Serial Port",   //  Serial Port
    "Blue Pill COMM",          //  COMM
    "Blue Pill DATA",          //  DATA
};

#define MSC_VENDOR_ID "BluePill"  //  Max 8 chars
#define MSC_PRODUCT_ID "UF2 Bootloader"  //  Max 16 chars
#define MSC_PRODUCT_REVISION_LEVEL "2.1"  //  Max 4 chars
#define USB_CLASS_MISCELLANEOUS 0xef  //  Copy from microbit.

enum usb_strings_index {  //  Index of USB strings.  Must sync with above, starts from 1.
    USB_STRINGS_MANUFACTURER = 1,
    USB_STRINGS_PRODUCT,
    USB_STRINGS_SERIAL_NUMBER,
    USB_STRINGS_DFU,
    USB_STRINGS_MSC,
    USB_STRINGS_SERIAL_PORT,
    USB_STRINGS_COMM,
    USB_STRINGS_DATA,
};
