#ifndef _DEFINES_H_
#define _DEFINES_H_

//Default radSens i2c device address
#define RS_DEFAULT_I2C_ADDRESS 0x66      

//Device id, default value: 0x7D
//Size: 8 bit
#define RS_DEVICE_ID_RG 0x00   

//Firmware version
//Size: 8 bit
#define RS_FIRMWARE_VER_RG 0x01

//Radiation intensity (dynamic period T < 123 sec)
//Size: 24 bit
#define RS_RAD_INTENSY_DYNAMIC_RG 0x03  

//Radiation intensity (static period T = 500 sec)
//Size: 24 bit 
#define RS_RAD_INTENSY_STATIC_RG 0x06  

/*Contains the accumulated number of pulses registered by the module
since the last I2C data reading. The value is reset each
time it is read. Allows you to process directly the pulses
from the Geiger counter and implement other algorithms. The value is updated
when each pulse is registered.
Size: 16 bit */
#define RS_PULSE_COUNTER_RG 0x09

/*This register is used to change the device address when multiple
devices need to be connected to the same line at the same
time. By default, it contains the value 0x66. At the end of recording, the new
value is stored in the non-volatile memory of the microcontroller.
Size: 8 bit 
Access: R/W*/
#define RS_DEVICE_ADDRESS_RG 0x10

/*Control register for a high-voltage voltage Converter. By
default, it is in the enabled state. To enable the HV generator,
write 1 to the register, and 0 to disable it. If you try to write other
values, the command is ignored.
Size: 8 bit 
Access: R/W*/
#define RS_HV_GENERATOR_RG 0x11

/*Contains the value coefficient used for calculating
the radiation intensity. If necessary (for example, when installing a different
type of counter), the necessary sensitivity value in
imp/MKR is entered in the register. The default value is 105 imp/MKR. At the end of
recording, the new value is stored in the non-volatile memory of the
microcontroller.
Size: 8 bit 
Access: R/W*/
#define RS_SENSITIVITY_RG 0x12


#define RS_TOTAL_RG 0x13


#endif