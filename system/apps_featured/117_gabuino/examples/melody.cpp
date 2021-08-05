// plays Axel F on internal piezo speaker of LA104
#include <library.h>
#include <math.h>

void WriteTim8(int reg, uint16_t val)
{
  *((uint16_t*)(0x40013400 + reg)) = val;
}

void SoundOn()
{
  WriteTim8(0x20, 0x3000);
  WriteTim8(0x00, 0x0081);
  WriteTim8(0x40, 150); // volume
}

void SoundOff()
{
  WriteTim8(0x20, 0x0000);
  WriteTim8(0x00, 0x0080);
}

void Sound(int f)
{
    WriteTim8(0x24, 0);
    WriteTim8(0x28, 15-1);
    int div = 72e6/15/f;
    WriteTim8(0x2c, div-1);
    //BIOS::DBG::Print("f=%d, div=%d, ", f, div);
    SoundOn();
}

int C=0, Cis=1, D=2, Dis=3, E=4, F=5, Fis=6, G=7, Gis=8, A=9, Ais=10, B=11, None=12;
const int BPM = 120;

const int Beat = 60000 / BPM;
int Whole = Beat*4;
int Half = Whole/2;
int Quarter = Whole/4;
int Eigth = Whole/8;
int Sixteenth = Whole/16;
int Thirtysecond = Whole/32;
int WholeDot = Whole*1.5;
int HalfDot = Half*1.5;
int QuarterDot = Quarter*1.5;
int EigthDot = Eigth*1.5;
int SixteenthDot = Sixteenth*1.5;
int ThirtysecondDot = Thirtysecond*1.5;

int Frequency(int note, int octave)
{
  const int table[] = {4186, 4434, 4698, 4978, 5274, 5587, 5915, 6271, 6644, 7040, 7458, 7902};
  return table[note] >> (4-octave);
}

void Play(int note, int octave, int length)
{
  if (note != None) 
    Sound(Frequency(note, octave));

  //BIOS::DBG::Print("len=%d, ", length);
  BIOS::SYS::DelayMs(length);
  SoundOff();
  BIOS::SYS::DelayMs(10);
}

int main()
{
    // generated from http://x.valky.eu/klavirgen
    Play(G, 2, Quarter);
    Play(Ais, 2, EigthDot);
    Play(G, 2, Sixteenth);
    Play(None, 0, Sixteenth);
    Play(G, 2, Sixteenth);
    Play(C, 3, Eigth);
    Play(G, 2, Eigth);
    Play(F, 2, Eigth);
    Play(G, 2, Quarter);
    Play(D, 3, EigthDot);
    Play(G, 2, Sixteenth);
    Play(None, 0, Sixteenth);
    Play(G, 2, Sixteenth);
    Play(Dis, 3, Eigth);
    Play(D, 3, Eigth);
    Play(Ais, 2, Eigth);
    Play(G, 2, Eigth);
    Play(D, 3, Eigth);
    Play(G, 3, Eigth);
    Play(G, 2, Sixteenth);
    Play(F, 2, Sixteenth);
    Play(None, 0, Sixteenth);
    Play(F, 2, Sixteenth);
    Play(D, 2, Eigth);
    Play(A, 2, Eigth);
    Play(G, 2, Half);

    return 0;
}
