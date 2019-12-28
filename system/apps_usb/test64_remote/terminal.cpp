#include "terminal.h"

char* pLastChar = nullptr;

void CTerminal::begin()
{
  pLastChar = (char*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::LastChar);
}

void CTerminal::end()
{
}

void CTerminal::write(uint8_t b)
{
  if (pLastChar)
    *pLastChar = b;
}

bool CTerminal::available()
{
  return false;
}

uint8_t CTerminal::read()
{
  return 0;
}


