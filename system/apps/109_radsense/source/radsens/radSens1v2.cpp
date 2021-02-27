#include "radSens1v2.h"

//gcc -o test test.cpp radSens1v2.cpp -lwiringPi

ClimateGuard_RadSens1v2::ClimateGuard_RadSens1v2(uint8_t sensor_address)
{
    _sensor_address = sensor_address;
}

ClimateGuard_RadSens1v2::~ClimateGuard_RadSens1v2()
{
}

/*Initialization function and sensor connection. Returns false if the sensor is not connected to the I2C bus.*/
bool ClimateGuard_RadSens1v2::radSens_init()
{   
#if defined(ARDUINO)
    Wire.begin();
    Wire.beginTransmission(_sensor_address); //safety check, make sure the sensor is connected
    Wire.write(0x0);
    if (Wire.endTransmission(true) != 0)
        return false;
#elif defined(__arm__)
	_fd = wiringPiI2CSetup(_sensor_address);
    if (_fd == -1) {
        return false;
	}
#endif
    updateData();
    return true;
}

bool ClimateGuard_RadSens1v2::hasData()
{
    return _data[RS_DEVICE_ID_RG] != 0;
}

bool ClimateGuard_RadSens1v2::updateData()
{
#if defined(ARDUINO)
    Wire.requestFrom(_sensor_address, RS_TOTAL_RG);
    for (int i = 0; i < RS_TOTAL_RG; i++) {
        _data[i] = Wire.read();
    }
#elif defined(__arm__)
    uint8_t reg_adr = 0x00;
    for (int i = 0; i < RS_TOTAL_RG; i++) {
        _data[i] = wiringPiI2CReadReg8(_fd, reg_adr);
        reg_adr += 1;
    }
#endif
    if (_data[0] == 0x7D)
    {
        _pulse_cnt += (_data[RS_PULSE_COUNTER_RG] << 8) | _data[RS_PULSE_COUNTER_RG+1];
        return true;
    }
    return false;
}

/*Get chip id, default value: 0x7D.*/
uint8_t ClimateGuard_RadSens1v2::getChipId()
{
    if (!hasData())
        updateData();

    return _data[RS_DEVICE_ID_RG];
}

/*Get firmware version.*/
uint8_t ClimateGuard_RadSens1v2::getFirmwareVersion()
{
    if (!hasData())
        updateData();

    return _data[RS_FIRMWARE_VER_RG];
}

/*Get radiation intensity (dynamic period T < 123 sec).*/
float ClimateGuard_RadSens1v2::getRadIntensyDyanmic()
{
    if (updateData())
    {
        return (
            (_data[RS_RAD_INTENSY_DYNAMIC_RG] << 16) |
            (_data[RS_RAD_INTENSY_DYNAMIC_RG+1] << 8) |
            _data[RS_RAD_INTENSY_DYNAMIC_RG+2]) / 10.0;
    }
    else
    {
        return 0;
    }
}

/*Get radiation intensity (static period T = 500 sec).*/
float ClimateGuard_RadSens1v2::getRadIntensyStatic()
{
    if (updateData())
    {
        return (
            (_data[RS_RAD_INTENSY_STATIC_RG] << 16) |
            (_data[RS_RAD_INTENSY_STATIC_RG+1] << 8) |
            _data[RS_RAD_INTENSY_STATIC_RG+2]) / 10.0;
    }
    else
    {
        return 0;
    }
}

/*Get the accumulated number of pulses registered by the module
since the last I2C data reading.*/
uint32_t ClimateGuard_RadSens1v2::getNumberOfPulses()
{
    if (updateData())
    {
        return _pulse_cnt;
    }
    else
    {
        return 0;
    }
}

/*Get sensor address.*/
uint8_t ClimateGuard_RadSens1v2::getSensorAddress()
{
    if (updateData())
    {
        _sensor_address = _data[RS_DEVICE_ADDRESS_RG];
        return _sensor_address;
    }
    return 0;
}

/*Get state of high-voltage voltage Converter.*/
bool ClimateGuard_RadSens1v2::getHVGeneratorState()
{
    if (updateData())
    {
        if (_data[RS_HV_GENERATOR_RG] == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

/*Get the value coefficient used for calculating the radiation intensity.*/
uint8_t ClimateGuard_RadSens1v2::getSensitivity()
{
    if (updateData())
    {
        return _data[RS_SENSITIVITY_RG];
    } else
    {
        return 0;
    }
}

/*Control register for a high-voltage voltage Converter. By
default, it is in the enabled state. To enable the HV generator,
write 1 to the register, and 0 to disable it. If you try to write other
values, the command is ignored.
bool state = *state**/
bool ClimateGuard_RadSens1v2::setHVGeneratorState(bool state)
{
#if defined(ARDUINO)
    Wire.beginTransmission(_sensor_address);
    #if (ARDUINO >= 100)
        Wire.write(RS_HV_GENERATOR_RG);    
        if (state) {
            Wire.write(1);
        } else {
            Wire.write(0);  
        }
    #else
        Wire.send(RS_HV_GENERATOR_RG);   
        if (state) {
            Wire.send(1);
        } else {
            Wire.send(0);  
        }
    #endif
    if (Wire.endTransmission(true) == 0) 
        return true;  //"true" sends stop message after transmission & releases I2C bus

#elif defined(__arm__)
    if (state) {
        if (wiringPiI2CWriteReg8(_fd, RS_HV_GENERATOR_RG, 1) > 0) return true;
    } else {
        if (wiringPiI2CWriteReg8(_fd, RS_HV_GENERATOR_RG, 0) > 0) return true;
    }
#endif
    return false;
}

/*Contains the value coefficient used for calculating
the radiation intensity. If necessary (for example, when installing a different
type of counter), the necessary sensitivity value in
imp/MKR is entered in the register. The default value is 105 imp/MKR. At the end of
recording, the new value is stored in the non-volatile memory of the
microcontroller.
uint8_t sens = *coefficient**/
bool ClimateGuard_RadSens1v2::setSensitivity(uint8_t sens)
{
#if defined(ARDUINO)
    Wire.beginTransmission(_sensor_address);
    #if (ARDUINO >= 100)
        Wire.write(RS_SENSITIVITY_RG);
        Wire.write(sens);
    #else
        Wire.send(RS_SENSITIVITY_RG);
        Wire.send(sens);
    #endif
    if (Wire.endTransmission(true) == 0)
        return true;
#elif defined(__arm__)
    if (wiringPiI2CWriteReg8(_fd, RS_SENSITIVITY_RG, sens) > 0)
        return true;
#endif
    return false;
}
