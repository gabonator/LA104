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
