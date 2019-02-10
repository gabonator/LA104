template<uint32_t nPortBase, int nPin, int nState>
void SetGpioState()
{
  constexpr int nPinPos = nPin & 7;
  constexpr uint32_t dwMask = ~(0xf << (nPinPos*4));
  constexpr uint32_t dwBits = nState << (nPinPos*4);

  if (nPin < 8)
  {
    uint32_t* pCRL = (uint32_t*)nPortBase;
    *pCRL = (*pCRL & dwMask) | dwBits;
  } else
  {
    uint32_t* pCRH = (uint32_t*)(nPortBase + 4);
    *pCRH = (*pCRH & dwMask) | dwBits;
  }
}

template<uint32_t nPortBase, int nPin, bool value>
void SetGpioLevel()
{
  if (value)
  {
    uint32_t* pBSRR = &((uint32_t*)nPortBase)[4];
    *pBSRR = 1<<nPin;
  } else
  {
    uint32_t* pBRR = &((uint32_t*)nPortBase)[5];
    *pBRR = 1<<nPin;
  }
}
