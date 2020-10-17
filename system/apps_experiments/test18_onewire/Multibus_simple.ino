#include <OneWire.h>
#include <DallasTemperature.h>


void setup(void) {
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature Multiple Bus Control Library Simple Demo");
}

void loop(void) 
{
  static OneWire ds18x20[] = { (int)BIOS::GPIO::P1 };
  static DallasTemperature sensor[1];
  int oneWireCount = 1;

  static bool init = true;
  if (init)
  { 
    init = false;

    Serial.print("============Ready with ");
    Serial.print(oneWireCount);
    Serial.println(" Sensors================");
    
    // Start up the library on all defined bus-wires
    DeviceAddress deviceAddress;
    for (int i = 0; i < oneWireCount; i++) {;
      sensor[i].setOneWire(&ds18x20[i]);
      sensor[i].begin();
      if (sensor[i].getAddress(deviceAddress, 0)) sensor[i].setResolution(deviceAddress, 12);
    }
  }

  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  for (int i = 0; i < oneWireCount; i++) {
    sensor[i].requestTemperatures();
  }
  Serial.println("DONE");
  
  delay(1000);
  for (int i = 0; i < oneWireCount; i++) {
    float temperature = sensor[i].getTempCByIndex(0);
    Serial.print("Temperature for the sensor ");
    Serial.print(i);
    Serial.print(" is ");
    Serial.println(temperature);
  }
  Serial.println();
}