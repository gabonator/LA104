int pulseData[200];
uint8_t bitstreamData[128];
CAttributes::TAttribute attributesData[8];

bool sendThermo()
{
//  uint8_t buf[] = {1, 0, 4, 8, 16, 32, 64, 128, 1, 2, 4, 8, 16, 32, 64, 128, 1, 2, 4, 8, 16, 32, 64, 128, 1, 2, 4, 0, 16, 32, 64, 128, 1, 2, 4, 8};
//  uint8_t buf[] = {255, 2, 255};
//  uint8_t buf[] = {128, 0, 32, 16, 8, 4, 2, 1, 128, 64, 32, 16, 8, 4, 2, 1, 128, 64, 32, 16, 8, 4, 2, 1, 128, 64, 32, 0, 8, 4, 2, 1, 128, 64, 32, 16};
//  uint8_t buf[] = {128, 0, 16, 128, 64, 33, 8, 4, 32, 16, 132, 2, 16, 8, 4, 32, 16, 128, 66, 1, 8, 66, 16, 8, 4, 2, 1, 0, 128, 64, 32, 16, 8, 4, 2, 16};
//  uint8_t buf[] = {128, 0, 16, 128, 64, 33, 8, 4, 32, 16, 132, 2, 16, 8, 66, 1, 8, 64, 32, 16, 8, 4, 2, 1, 0, 128, 64, 32, 16, 8, 4, 2, 1, 8, 4, 2, 16};
  // MSB first: 255, 2, 255 -> 11111111 0000 0010 1111 1111

	static int temp = 444;
	EVERY(60000) // receives every 66 seconds
	{
//		temp = (rand()%800)-400;
		temp = -(rand()%200);
		float f = temp*0.1f;
		CONSOLE::Color(RGB565(00ff00));
		CONSOLE::Print("Setting temperature %.1f\n", f);
		//CONSOLE::Print("Setting temperature %d.%d\n", (int)temp/10, abs(temp)%10);
		CONSOLE::Color(RGB565(ffffff));
	}
	
  CArray<int> pulse(pulseData, COUNT(pulseData));
  CArray<uint8_t> bitstream(bitstreamData, COUNT(bitstreamData));

  CWeather weather;
  CAttributes attrs(attributesData, COUNT(attributesData));
  weather.Example(attrs);
  attrs["temperature10"] = temp;
  weather.Modulate(attrs, pulse);

  weather.PulseToBitstream(pulse, bitstream, 500);
  sendPacket(weather.Frequency(), 2000, bitstream.GetData(), bitstream.GetSize());

  return true;
}
