// Automatically generated

uint32_t GetProcAddress(char* symbol)
{
  if (strncmp(symbol, "_ZN4BIOS", 8) == 0)
  {
    symbol += 8;
  } else
  if (strncmp(symbol, "_ZN3GUI", 7) == 0)
  {
    symbol += 7;
  } else
  if (strncmp(symbol, "_Z", 2) == 0)
  {
    symbol += 2;
  } else
  {
    return 0;
  }

  while (*symbol >= '0' && *symbol <= '9')
    symbol++;


  if (strcmp(symbol, "sprintfPcPKcz") == 0)
    return (uint32_t)static_cast<int(*)(char*, char const*, ...)>(sprintf);
  if (strcmp(symbol, "BackgroundER5CRectmm") == 0)
    return (uint32_t)static_cast<void(*)(CRect&, unsigned long, unsigned long)>(GUI::Background);
  if (strcmp(symbol, "WindowERK5CRectt") == 0)
    return (uint32_t)static_cast<void(*)(CRect const&, unsigned short)>(GUI::Window);
  if (strcmp(symbol, "DBG5PrintEPKcz") == 0)
    return (uint32_t)static_cast<void(*)(char const*, ...)>(BIOS::DBG::Print);
  if (strcmp(symbol, "FAT11GetFileSizeEv") == 0)
    return (uint32_t)static_cast<ui32(*)()>(BIOS::FAT::GetFileSize);
  if (strcmp(symbol, "FAT15GetSharedBufferEv") == 0)
    return (uint32_t)static_cast<PVOID(*)()>(BIOS::FAT::GetSharedBuffer);
  if (strcmp(symbol, "FAT4InitEv") == 0)
    return (uint32_t)static_cast<BIOS::FAT::EResult(*)()>(BIOS::FAT::Init);
  if (strcmp(symbol, "FAT4OpenEPKch") == 0)
    return (uint32_t)static_cast<BIOS::FAT::EResult(*)(char const*, unsigned char)>(BIOS::FAT::Open);
  if (strcmp(symbol, "FAT4ReadEPh") == 0)
    return (uint32_t)static_cast<BIOS::FAT::EResult(*)(unsigned char*)>(BIOS::FAT::Read);
  if (strcmp(symbol, "FAT4SeekEm") == 0)
    return (uint32_t)static_cast<BIOS::FAT::EResult(*)(unsigned long)>(BIOS::FAT::Seek);
  if (strcmp(symbol, "FAT5CloseEi") == 0)
    return (uint32_t)static_cast<BIOS::FAT::EResult(*)(int)>(BIOS::FAT::Close);
  if (strcmp(symbol, "FAT5CloseEv") == 0)
    return (uint32_t)static_cast<BIOS::FAT::EResult(*)()>(BIOS::FAT::Close);
  if (strcmp(symbol, "FAT5WriteEPh") == 0)
    return (uint32_t)static_cast<BIOS::FAT::EResult(*)(unsigned char*)>(BIOS::FAT::Write);
  if (strcmp(symbol, "FAT7OpenDirEPc") == 0)
    return (uint32_t)static_cast<BIOS::FAT::EResult(*)(char*)>(BIOS::FAT::OpenDir);
  if (strcmp(symbol, "FAT8FindNextEPNS0_9TFindFileE") == 0)
    return (uint32_t)static_cast<BIOS::FAT::EResult(*)(BIOS::FAT::TFindFile*)>(BIOS::FAT::FindNext);
  if (strcmp(symbol, "KEY6GetKeyEv") == 0)
    return (uint32_t)static_cast<BIOS::KEY::EKey(*)()>(BIOS::KEY::GetKey);
  if (strcmp(symbol, "LCD10BufferPushEt") == 0)
    return (uint32_t)static_cast<void(*)(unsigned short)>(BIOS::LCD::BufferPush);
  if (strcmp(symbol, "LCD11BufferBeginERK5CRecth") == 0)
    return (uint32_t)static_cast<void(*)(CRect const&, unsigned char)>(BIOS::LCD::BufferBegin);
  if (strcmp(symbol, "LCD3BarERK5CRectt") == 0)
    return (uint32_t)static_cast<void(*)(CRect const&, unsigned short)>(BIOS::LCD::Bar);
  if (strcmp(symbol, "LCD3BarEiiiit") == 0)
    return (uint32_t)static_cast<void(*)(int, int, int, int, unsigned short)>(BIOS::LCD::Bar);
  if (strcmp(symbol, "LCD4DrawEiittPKc") == 0)
    return (uint32_t)static_cast<int(*)(int, int, unsigned short, unsigned short, char const*)>(BIOS::LCD::Draw);
  if (strcmp(symbol, "LCD5ClearEt") == 0)
    return (uint32_t)static_cast<void(*)(unsigned short)>(BIOS::LCD::Clear);
  if (strcmp(symbol, "LCD5PrintEiittPKc") == 0)
    return (uint32_t)static_cast<int(*)(int, int, unsigned short, unsigned short, char const*)>(BIOS::LCD::Print);
  if (strcmp(symbol, "LCD5PrintEiittPc") == 0)
    return (uint32_t)static_cast<int(*)(int, int, unsigned short, unsigned short, char*)>(BIOS::LCD::Print);
  if (strcmp(symbol, "LCD6ShadowEiiiij") == 0)
    return (uint32_t)static_cast<void(*)(int, int, int, int, unsigned int)>(BIOS::LCD::Shadow);
  if (strcmp(symbol, "LCD7PatternEiiiiPKti") == 0)
    return (uint32_t)static_cast<void(*)(int, int, int, int, unsigned short const*, int)>(BIOS::LCD::Pattern);
  if (strcmp(symbol, "LCD8PutPixelERK6CPointt") == 0)
    return (uint32_t)static_cast<void(*)(CPoint const&, unsigned short)>(BIOS::LCD::PutPixel);
  if (strcmp(symbol, "LCD8PutPixelEiit") == 0)
    return (uint32_t)static_cast<void(*)(int, int, unsigned short)>(BIOS::LCD::PutPixel);
  if (strcmp(symbol, "LCD9BufferEndEv") == 0)
    return (uint32_t)static_cast<void(*)()>(BIOS::LCD::BufferEnd);
  if (strcmp(symbol, "LCD9RoundRectERK5CRectt") == 0)
    return (uint32_t)static_cast<void(*)(CRect const&, unsigned short)>(BIOS::LCD::RoundRect);
  if (strcmp(symbol, "LCD9RoundRectEiiiit") == 0)
    return (uint32_t)static_cast<void(*)(int, int, int, int, unsigned short)>(BIOS::LCD::RoundRect);
  if (strcmp(symbol, "SYS4BeepEi") == 0)
    return (uint32_t)static_cast<void(*)(int)>(BIOS::SYS::Beep);
  if (strcmp(symbol, "SYS7DelayMsEi") == 0)
    return (uint32_t)static_cast<void(*)(int)>(BIOS::SYS::DelayMs);
  if (strcmp(symbol, "SYS7ExecuteEm") == 0)
    return (uint32_t)static_cast<int(*)(unsigned long)>(BIOS::SYS::Execute);
  if (strcmp(symbol, "SYS7GetTickEv") == 0)
    return (uint32_t)static_cast<uint32_t(*)()>(BIOS::SYS::GetTick);
  if (strcmp(symbol, "GPIO10AnalogReadENS0_4EPinE") == 0)
    return (uint32_t)static_cast<int(*)(BIOS::GPIO::EPin)>(BIOS::GPIO::AnalogRead);
  if (strcmp(symbol, "GPIO11AnalogWriteENS0_4EPinEi") == 0)
    return (uint32_t)static_cast<void(*)(BIOS::GPIO::EPin, int)>(BIOS::GPIO::AnalogWrite);
  if (strcmp(symbol, "GPIO11DigitalReadENS0_4EPinE") == 0)
    return (uint32_t)static_cast<bool(*)(BIOS::GPIO::EPin)>(BIOS::GPIO::DigitalRead);
  if (strcmp(symbol, "GPIO12DigitalWriteENS0_4EPinEb") == 0)
    return (uint32_t)static_cast<void(*)(BIOS::GPIO::EPin, bool)>(BIOS::GPIO::DigitalWrite);
  if (strcmp(symbol, "GPIO3I2C11RequestFromEhh") == 0)
    return (uint32_t)static_cast<bool(*)(unsigned char, unsigned char)>(BIOS::GPIO::I2C::RequestFrom);
  if (strcmp(symbol, "GPIO3I2C15EndTransmissionEv") == 0)
    return (uint32_t)static_cast<bool(*)()>(BIOS::GPIO::I2C::EndTransmission);
  if (strcmp(symbol, "GPIO3I2C17BeginTransmissionEh") == 0)
    return (uint32_t)static_cast<bool(*)(unsigned char)>(BIOS::GPIO::I2C::BeginTransmission);
  if (strcmp(symbol, "GPIO3I2C4ReadEv") == 0)
    return (uint32_t)static_cast<uint8_t(*)()>(BIOS::GPIO::I2C::Read);
  if (strcmp(symbol, "GPIO3I2C5WriteEh") == 0)
    return (uint32_t)static_cast<bool(*)(unsigned char)>(BIOS::GPIO::I2C::Write);
  if (strcmp(symbol, "GPIO4UART4ReadEv") == 0)
    return (uint32_t)static_cast<uint8_t(*)()>(BIOS::GPIO::UART::Read);
  if (strcmp(symbol, "GPIO4UART5SetupEiNS1_7EConfigE") == 0)
    return (uint32_t)static_cast<void(*)(int, BIOS::GPIO::UART::EConfig)>(BIOS::GPIO::UART::Setup);
  if (strcmp(symbol, "GPIO4UART5WriteEh") == 0)
    return (uint32_t)static_cast<void(*)(unsigned char)>(BIOS::GPIO::UART::Write);
  if (strcmp(symbol, "GPIO4UART9AvailableEv") == 0)
    return (uint32_t)static_cast<bool(*)()>(BIOS::GPIO::UART::Available);
  if (strcmp(symbol, "GPIO7PinModeENS0_4EPinENS0_5EModeE") == 0)
    return (uint32_t)static_cast<void(*)(BIOS::GPIO::EPin, BIOS::GPIO::EMode)>(BIOS::GPIO::PinMode);
  if (strcmp(symbol, "MEMORY11LinearStartEv") == 0)
    return (uint32_t)static_cast<void(*)()>(BIOS::MEMORY::LinearStart);
  if (strcmp(symbol, "MEMORY12LinearFinishEv") == 0)
    return (uint32_t)static_cast<bool(*)()>(BIOS::MEMORY::LinearFinish);
  if (strcmp(symbol, "MEMORY13LinearProgramEmPhi") == 0)
    return (uint32_t)static_cast<bool(*)(unsigned long, unsigned char*, int)>(BIOS::MEMORY::LinearProgram);
  if (strcmp(symbol, "MEMORY15GetSharedBufferEv") == 0)
    return (uint32_t)static_cast<PVOID(*)()>(BIOS::MEMORY::GetSharedBuffer);
  return 0;
}
