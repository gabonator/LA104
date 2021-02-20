#include <iostream>
#include "library.h"
#include "../../../os_host/source/framework/Classes.h"
#include "../source/protocol/all.h"
#include "../source/testwave.h"

#define signal sample12

int main()
{
  CAttributes::TAttribute attributesData[20];
  CAttributes attributes(attributesData, COUNT(attributesData));

/*
OregonScientific-2: 87 bits: <a19840e2 010c6202 20f3c000>
OregonScientific-2: <THGR810:> temp <6.0 C> hum <64%>
OregonScientific-2: 88 bits: <a19840e2 010c2203 20c3f100>
OregonScientific-2: 91 bits: <a291405a 00000573 06043f50>
OregonScientific-2: 87 bits: <a19840e2 010c6305 2034c000>
..{"id":"84cca8","class":"rfsniff-meteo-2021","uptime":61,"debug":[1, 4, 5],"temp":6.0,"hum":64}
Api: Connecting...
Api: Send ok
Api: Response: ''
Upload ok!
OregonScientific-2: 86 bits: <a19840e2 010c6306 20443300>
OregonScientific-2: <THGR810:> temp <6.0 C> hum <64%>
.OregonScientific-2: 91 bits: <a291405a 00000573 06043f50>
OregonScientific-2: 87 bits: <a19840e2 000c5209 20448100>
*/
  attributes["data_0"] = 0xa19840e2; 
  attributes["data_1"] = 0x010c6202; 
  attributes["data_2"] = 0x20f3c000; 

//  attributes["data_0"] = 0xa19840e2; 
//  attributes["data_1"] = 0x010c2203; 
//  attributes["data_2"] = 0x20c3f100; 

  attributes["data_0"] = 0xa291405a; 
  attributes["data_1"] = 0x00000573; 
  attributes["data_2"] = 0x06043f50; 

  oregon.Analyse(attributes);
      for (int i=0; i<attributes.GetSize(); i++)
        std::cout << attributes[i].key << " = " << std::hex << attributes[i].value << "\n";
      std::cout << "\n";
  return 0;

  CArray<uint16_t> pulse(signal, COUNT(signal));
  pulse.SetSize(COUNT(signal));

  static CProtocol* protocols[] = {&weather, &oregon, &key360, &vw, &nexus, &gate};
  for (int i=0; i<COUNT(protocols); i++)
  {
    if (protocols[i]->Demodulate(pulse, attributes))
    {
      char name[64];
      protocols[i]->GetName(name);
      std::cout << "Protocol: " << name << "\n";
      for (int i=0; i<attributes.GetSize(); i++)
        std::cout << attributes[i].key << " = " << std::hex << attributes[i].value << "\n";
      std::cout << "\n";

      uint16_t reverseData[1024];
      CArray<uint16_t> reverse(reverseData, COUNT(reverseData));

      protocols[i]->Modulate(attributes, reverse);
      attributes.RemoveAll();
      if (protocols[i]->Demodulate(reverse, attributes))
      {
        std::cout << "Remodulated signal:\n";
        for (int i=0; i<attributes.GetSize(); i++)
          std::cout << attributes[i].key << " = " << std::hex << attributes[i].value << "\n";
        std::cout << "\n";
      } else
      {               
        std::cout << "Failed: Not able to remodulate\n";
      }
    }
    return 0;
  }

  std::cout << "Failed: Unknown protocol\n";
  return 1;
}
