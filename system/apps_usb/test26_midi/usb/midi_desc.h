#pragma once

// Following settings allow to customize the USB device descriptor
#define MIOS32_USB_VENDOR_ID    0x16c0        // sponsored by voti.nl! see http://www.voti.nl/pids
#define MIOS32_USB_VENDOR_STR   "midibox.org" // you will see this in the USB device description
#define MIOS32_USB_PRODUCT_STR  "MIOS32"      // you will see this in the MIDI device list
#define MIOS32_USB_PRODUCT_ID   0x03ff        // ==1023; 1020-1029 reserved for T.Klose, 1000 - 1009 free for lab use
#define MIOS32_USB_VERSION_ID   0x0100        // v1.00


// Following settings allow to customize the USB device descriptor
#ifndef MIOS32_USB_VENDOR_ID
#define MIOS32_USB_VENDOR_ID    0x16c0        // sponsored by voti.nl! see http://www.voti.nl/pids
#endif
#ifndef MIOS32_USB_VENDOR_STR
#define MIOS32_USB_VENDOR_STR   "midibox.org" // you will see this in the USB device description
#endif
#ifndef MIOS32_USB_PRODUCT_STR
#define MIOS32_USB_PRODUCT_STR  "MIOS32"      // you will see this in the MIDI device list
#endif
#ifndef MIOS32_USB_PRODUCT_ID
#define MIOS32_USB_PRODUCT_ID   0x03fe        // ==1022; 1020-1029 reserved for T.Klose, 1000 - 1009 free for lab use
                                              // note: Vendor ID 1022 is required if the GM5 driver should be used!
#endif
#ifndef MIOS32_USB_VERSION_ID
#define MIOS32_USB_VERSION_ID   0x0100        // v1.00
#endif


// 1 to stay compatible to USB MIDI spec, 0 as workaround for some windows versions...
#define MIOS32_USB_MIDI_USE_AC_INTERFACE 1

// allowed number of USB MIDI ports: 1..8
#define MIOS32_USB_MIDI_NUM_PORTS 1


// buffer size (should be at least >= MIOS32_USB_MIDI_DATA_*_SIZE/4)
#define MIOS32_USB_MIDI_RX_BUFFER_SIZE   64 // packages
#define MIOS32_USB_MIDI_TX_BUFFER_SIZE   64 // packages

// size of IN/OUT pipe
#define MIOS32_USB_MIDI_DATA_IN_SIZE           64
#define MIOS32_USB_MIDI_DATA_OUT_SIZE          64

// endpoint assignments (don't change!)
#define MIOS32_USB_MIDI_DATA_OUT_EP 0x02
#define MIOS32_USB_MIDI_DATA_IN_EP  0x81



// internal defines which are used by MIOS32 USB MIDI/COM (don't touch)
#define MIOS32_USB_EP_NUM   5

// buffer table base address
#define MIOS32_USB_BTABLE_ADDRESS      0x000

// EP0 rx/tx buffer base address
#define MIOS32_USB_ENDP0_RXADDR        0x040
#define MIOS32_USB_ENDP0_TXADDR        0x080

// EP1 Rx/Tx buffer base address for MIDI driver
#define MIOS32_USB_ENDP1_TXADDR        0x0c0
#define MIOS32_USB_ENDP2_RXADDR        0x100

// EP3/4/5 buffer base addresses for COM driver
#define MIOS32_USB_ENDP3_RXADDR        0x140
#define MIOS32_USB_ENDP4_TXADDR        0x180
#define MIOS32_USB_ENDP5_TXADDR        0x1c0


// a lot confusing settings, which could also be generated via software!
#ifndef MIOS32_DONT_USE_USB_MIDI
# if MIOS32_USB_MIDI_USE_AC_INTERFACE
#  define MIOS32_USB_MIDI_NUM_INTERFACES        2
#  define MIOS32_USB_MIDI_AC_INTERFACE_IX       0x00
#  define MIOS32_USB_MIDI_AS_INTERFACE_IX       0x01
#  define MIOS32_USB_MIDI_INTERFACE_OFFSET      2
# else
#  define MIOS32_USB_MIDI_AS_INTERFACE_IX       0x00 // gabo asi?
#  define MIOS32_USB_MIDI_NUM_INTERFACES        1
#  define MIOS32_USB_MIDI_INTERFACE_NUM         0x00
#  define MIOS32_USB_MIDI_INTERFACE_OFFSET      1
# endif
# define MIOS32_USB_MIDI_SIZ_CLASS_DESC         (7+MIOS32_USB_MIDI_NUM_PORTS*(6+6+9+9)+9+(4+MIOS32_USB_MIDI_NUM_PORTS)+9+(4+MIOS32_USB_MIDI_NUM_PORTS))
# define MIOS32_USB_MIDI_SIZ_CONFIG_DESC        (9+MIOS32_USB_MIDI_USE_AC_INTERFACE*(9+9)+MIOS32_USB_MIDI_SIZ_CLASS_DESC)

# define MIOS32_USB_MIDI_SIZ_CLASS_DESC_SINGLE_USB  (7+1*(6+6+9+9)+9+(4+1)+9+(4+1))
# define MIOS32_USB_MIDI_SIZ_CONFIG_DESC_SINGLE_USB (9+MIOS32_USB_MIDI_USE_AC_INTERFACE*(9+9)+MIOS32_USB_MIDI_SIZ_CLASS_DESC)

#else
# define MIOS32_USB_MIDI_NUM_INTERFACES         0
# define MIOS32_USB_MIDI_INTERFACE_OFFSET       0
# define MIOS32_USB_MIDI_SIZ_CONFIG_DESC        0
#endif

#ifdef MIOS32_USE_USB_COM
# define MIOS32_USB_COM_NUM_INTERFACES          2
# define MIOS32_USB_COM_SIZ_CONFIG_DESC         58
# define MIOS32_USB_COM_CC_INTERFACE_IX         (MIOS32_USB_MIDI_INTERFACE_OFFSET + 0x00)
# define MIOS32_USB_COM_CD_INTERFACE_IX         (MIOS32_USB_MIDI_INTERFACE_OFFSET + 0x01)
# define MIOS32_USB_COM_INTERFACE_OFFSET        (MIOS32_USB_MIDI_INTERFACE_OFFSET + 2)
#else
# define MIOS32_USB_COM_NUM_INTERFACES          0
# define MIOS32_USB_COM_SIZ_CONFIG_DESC         0
# define MIOS32_USB_COM_INTERFACE_OFFSET        (MIOS32_USB_MIDI_INTERFACE_OFFSET + 0)
#endif

#define MIOS32_USB_NUM_INTERFACES              (MIOS32_USB_MIDI_NUM_INTERFACES + MIOS32_USB_COM_NUM_INTERFACES)
#define MIOS32_USB_SIZ_CONFIG_DESC             (9 + MIOS32_USB_MIDI_SIZ_CONFIG_DESC + MIOS32_USB_COM_SIZ_CONFIG_DESC)
#define MIOS32_USB_SIZ_CONFIG_DESC_SINGLE_USB  (9 + MIOS32_USB_MIDI_SIZ_CONFIG_DESC_SINGLE_USB)


#define MIOS32_USB_SIZ_DEVICE_DESC 18

extern const u8 MIOS32_USB_ConfigDescriptor[MIOS32_USB_SIZ_CONFIG_DESC];
extern const u8 MIOS32_USB_DeviceDescriptor[MIOS32_USB_SIZ_DEVICE_DESC];
extern const u8 MIOS32_USB_ConfigDescriptor_SingleUSB[MIOS32_USB_SIZ_CONFIG_DESC_SINGLE_USB];
