void DelayUs(int us)
{
  us = us*12;
  while (us--)
  {
    __asm__("");
  }
}

void DelayMs(int ms)
{
  while (ms--)
  {
    int us = 12000;
    while (us--)
    {
      __asm__("");
    }
  }
}
