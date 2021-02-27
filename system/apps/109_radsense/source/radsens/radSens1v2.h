#ifndef _RADSENS1V2_H_
#define _RADSENS1V2_H_

#include <stdint.h>

#if defined(ARDUINO)
#include <Arduino.h>
#include <Wire.h>
#elif defined(__arm__)
#include <wiringPiI2C.h>
#include <stdio.h>
#endif

#include "defines.h"

class ClimateGuard_RadSens1v2 {
private:
#if defined(__arm__)
    int _fd = 0;
#endif
    uint8_t _sensor_address;
    uint8_t _data[RS_TOTAL_RG] = { 0 };
    uint32_t _pulse_cnt;
    bool updateData();
    bool hasData();

public:
    ClimateGuard_RadSens1v2(uint8_t sensorAddress);
    ~ClimateGuard_RadSens1v2();

    //Methods for get or set data
    bool radSens_init();
    bool getData();
    uint8_t getChipId();
    uint8_t getFirmwareVersion();
    float getRadIntensyDyanmic();
    float getRadIntensyStatic();
    uint32_t getNumberOfPulses();
    uint8_t getSensorAddress();
    bool getHVGeneratorState();
    uint8_t getSensitivity();
    bool setHVGeneratorState(bool state);
    bool setSensitivity(uint8_t sens);
};

#endif // _RADSENS1V2_H_
