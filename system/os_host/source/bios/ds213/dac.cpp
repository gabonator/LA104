namespace BIOS
{
  namespace DAC
  {
    void SetFrequency(int freqHz) {}
    void SetDuty(int duty) {}
    int GetFrequency() {return 0;}
    int GetDuty() {return 0;}
    void SetMode(EMode mode, uint16_t* buffer, int length) {}
  }
}