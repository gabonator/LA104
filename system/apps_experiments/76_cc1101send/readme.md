# CC1101 as OOK sender


```
         VDD   [1] 2    VDD  -> +3V
P1 <-     SI    3  4    SCK  -> P2
CH4 <-    SO    5  6    GDO2
P3 <-    /CS    7  8    GDO0 -> P4
         GND    9 10    GND  -> GND



            case CPinIoBase::MOSI: return BIOS::GPIO::EPin::P1;
            case CPinIoBase::SCK: return BIOS::GPIO::EPin::P2;
            case CPinIoBase::CS: return BIOS::GPIO::EPin::P3;
            case CPinIoBase::D: return BIOS::GPIO::EPin::P4;
            case CPinIoBase::MISO: return BIOS::GPIO::EPin::CH4;
```