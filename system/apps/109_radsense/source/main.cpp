#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include "radsens/radSens1v2.h"

ClimateGuard_RadSens1v2 radSens(RS_DEFAULT_I2C_ADDRESS); /*Constructor of the class ClimateGuard_RadSens1v2,
                                                           sets the address parameter of I2C sensor.
                                                           Default address: 0x66.*/

char* T(const char* line)
{
  static char temp[80];
  static int row = 0; 
  strcpy(temp, line);
  for (int i=0; temp[i]; i++)
  {
    switch (temp[i])
    {
      case '-': 
        temp[i] = 205; break;
      case '+': 
        if (row==0)
        {
          if (i==0) { temp[i] = 201; break; }
          if (i==36) { temp[i] = 187; break; }
          temp[i] = 209; break;
        } else if (row==1)
        { 
          if (i==0) { temp[i] = 204; break; }
          if (i==36) { temp[i] = 185; break; }
          temp[i] = 216; break;
        } else
        {
          if (i==0) { temp[i] = 200; break; }
          if (i==36) { temp[i] = 188; break; }
          temp[i] = 207; break;
        }
      case '|':
        if (i==0) { temp[i] = 186; break; }
        if (i==36) { temp[i] = 186; break; }
        temp[i] = 179; break;
    }
  }
  if (line[0] == '+') 
    row++;
  return temp;
}

void setup() 
{
  radSens.radSens_init(); /*Initialization function and sensor connection. 
                            Returns false if the sensor is not connected to the I2C bus.*/
  
  uint8_t sensorChipId = radSens.getChipId(); /*Returns chip id, default value: 0x7D.*/

  CONSOLE::Print("Chip id: 0x%02x\n", sensorChipId);
  if (sensorChipId == 0x7d)
    CONSOLE::Print("Wrong chip id, should be 0x7d!\n");

  uint8_t firmWareVer = radSens.getFirmwareVersion(); /*Returns firmware version.*/

  CONSOLE::Print("Firmware version: %d\n", firmWareVer);
  CONSOLE::Print("-------------------------------------\n");
  CONSOLE::Print("Set Sensitivity example:\n");

  uint8_t sensitivity = radSens.getSensitivity(); /*Rerutns the value coefficient used for calculating
                                                    the radiation intensity or 0 if sensor isn't connected.*/

  CONSOLE::Print("getSensitivity() = %d\n", sensitivity);
  CONSOLE::Print("setSensitivity(55)... ");

  radSens.setSensitivity(55); /*Sets the value coefficient used for calculating
                                the radiation intensity*/

  if (radSens.getSensitivity() == 55) 
    CONSOLE::Print("Ok\n");
  else
    CONSOLE::Print("Fail!\n");

  CONSOLE::Print("setSensitivity(105)... ");
  radSens.setSensitivity(105);
  if (radSens.getSensitivity() == 105) 
    CONSOLE::Print("Ok\n");
  else
    CONSOLE::Print("Fail!\n");

  delay(1000);

//  bool hvGeneratorState = radSens.getHVGeneratorState(); /*Returns state of high-voltage voltage Converter.
//                                                           If return true -> on
//                                                           If return false -> off or sensor isn't conneted*/
//
//  CONSOLE::Print("HV generator state: %s\n", hvGeneratorState ? "true" : "false");
//  delay(2000);
//  CONSOLE::Print("setHVGeneratorState(false), ");
//
//  radSens.setHVGeneratorState(false); /*Set state of high-voltage voltage Converter.
//                                        if setHVGeneratorState(true) -> turn on HV generator
//                                        if setHVGeneratorState(false) -> turn off HV generator*/
//  
//  hvGeneratorState = radSens.getHVGeneratorState();
//  CONSOLE::Print("state = %s\n", hvGeneratorState ? "true" : "false");
//  delay(2000);
  CONSOLE::Print("setHVGeneratorState(true)... ");
  radSens.setHVGeneratorState(true);
  bool hvGeneratorState = radSens.getHVGeneratorState();
  if(hvGeneratorState)
    CONSOLE::Print("Ok\n\n");               
  else
    CONSOLE::Print("Fail\n\n");               

  CONSOLE::Print(T("+----------+----------+-------------+\n"));
  CONSOLE::Print(T("| Dynamic  | Static   | Pulse count |\n"));
  CONSOLE::Print(T("| [uR/h]   | [uR/h]   |             |\n"));
  CONSOLE::Print(T("+----------+----------+-------------+\n"));
}                                       

void loop()
{
  static int lastPulses = 0;
  int pulses = radSens.getNumberOfPulses();
  if (pulses != lastPulses)
  {
    lastPulses = pulses;
    BIOS::SYS::Beep(10); 
  }

  EVERY(2000)
  {
    char line[80];   
    sprintf(line, "  %.1f        %.1f        %d                 ",
      radSens.getRadIntensyDyanmic(), 
      radSens.getRadIntensyStatic(),
      pulses);

    line[0] = 186;
    line[11] = 179;
    line[22] = 179;
    line[36] = 186;
    line[37] = '\n';
    line[38] = 0;

    CONSOLE::Print(line);
    CONSOLE::Print(T("+----------+----------+-------------+\n"));
    CONSOLE::cursor.x = CONSOLE::window.left;
    CONSOLE::cursor.y -= 2*14;
  }
}

void deinit()
{
  CONSOLE::Print("\n\n\nShutting down HV generator\n");
  radSens.setHVGeneratorState(false);
  bool hvGeneratorState = radSens.getHVGeneratorState();
  CONSOLE::Print("HV generator state: %s\n", hvGeneratorState ? "ON" : "OFF");
  delay(200);
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    APP::Init("RadSense ver 1v2");
    APP::Status("P1: SCL, P2: SDA");

    setup();

    BIOS::KEY::EKey key;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
    {
      loop();
    }

    deinit();
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(" [%d]: %s\n", line, cond);
    while (1);
}
