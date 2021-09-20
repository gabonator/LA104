#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"

// Basic demo for readings from Adafruit SCD30
#include "scd30/Adafruit_SCD30.h"

Adafruit_SCD30  scd30;

bool setup(void) {
  CONSOLE::Print("Adafruit SCD30 test!\n");

  // Try to initialize!
  if (!scd30.begin()) {
    CONSOLE::Print("Failed to find SCD30 chip\n");
    BIOS::SYS::DelayMs(1000);
    return false;
  }
  CONSOLE::Print("SCD30 Found!\n");


  if (!scd30.setMeasurementInterval(10)){
     CONSOLE::Print("Failed to set measurement interval");
  //   while(1){ delay(10);}
     return false;
   }
  CONSOLE::Print("Measurement Interval: %d seconds\n\n", scd30.getMeasurementInterval()); 
  CONSOLE::Print("| Temperature | Humidity | CO2    |\n");    
  CONSOLE::Print("|-------------|----------|--------|\n");   
  return true;
}

void loop() 
{
  if (scd30.dataReady()) {
//    CONSOLE::Print("Data available!\n");

    if (!scd30.read()){ CONSOLE::Print("\nError reading sensor data\n"); return; }

    for (int i=0; i<60; i++)
      CONSOLE::Putch('\x08');
    CONSOLE::Print("| %.1f C      | %.1f %%   | %4d   |", 
      scd30.temperature, scd30.relative_humidity, (int)scd30.CO2);
/*
    CONSOLE::Print("Temperature: %.1f degrees C\n", scd30.temperature);    
    CONSOLE::Print("Relative Humidity: %.1f %%\n", scd30.relative_humidity);
    CONSOLE::Print("CO2: %.0f ppm\n", scd30.CO2);
*/
  } else {
    //Serial.println("No data");
  }
  BIOS::SYS::DelayMs(100);
}


#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    CONSOLE::colorBack = RGB565(202020);

    APP::Init("DEMO: test");
    APP::Status("");

    if (setup())
    {
        BIOS::KEY::EKey key;
        while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
        {
          loop();
        }
    }
    commoni2c.end();
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(" [%d]: %s\n", line, cond);
    while (1);
}
