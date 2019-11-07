#pragma once
#include <usb_core.h>
#include "midi_app.h"

#define DSCR_DEVICE	1	// Descriptor type: Device
#define DSCR_CONFIG	2	// Descriptor type: Configuration
#define DSCR_STRING	3	// Descriptor type: String
#define DSCR_INTRFC	4	// Descriptor type: Interface
#define DSCR_ENDPNT	5	// Descriptor type: Endpoint

#define CS_INTERFACE	0x24	// Class-specific type: Interface
#define CS_ENDPOINT	0x25	// Class-specific type: Endpoint


extern const USER_STANDARD_REQUESTS My_User_Standard_Requests;
extern const DEVICE_PROP My_Device_Property;
extern const DEVICE My_Device_Table;


