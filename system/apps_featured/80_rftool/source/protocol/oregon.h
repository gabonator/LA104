class COregon : public CProtocol
{
public:
	virtual int Frequency() override
	{
		return 433876000UL;
	}
	
  virtual int MinIndentifyCount() override
  {
    return 0;
  }

  virtual int MinDemodulateCount() override
  {
    return 0;
  }

  virtual bool Identify(CArray<int>& pulse) override
  {
    return false;
  }

  virtual int AttackPoint(CArray<int>& pulse) override
  {
    return 0;
  }

  virtual void Example(CAttributes& attributes) override
  {
    // -512 (-51.2 C) ... 999 (+99.9 C)
    attributes["length"] = 64;
    attributes["data64_0"] = 0x12345678;
    attributes["data64_1"] = 0xabcdef01;
  }

  virtual bool Demodulate(const CArray<uint16_t>& pulse, CAttributes& attributes) override
  {
    int nibblesData[16];
    CArray<int> b(nibblesData, COUNT(nibblesData));

    int length = 0;
    if (!PulseToBytes(pulse, b, length))
      return false;

    attributes["length"] = length; // count of bits
    uint32_t data=0;
    int bytes = (length+7)/8;
    for (int i=0; i<bytes; i++) // per each byte
    {
      bool last = i==bytes-1;
      data <<= 8;
      data |= b[i];
      if ((i&3)==3 || last)
      {
        switch (i/4) // store as dword
        {
          case 0: attributes["data64_0"] = data; break;
          case 1: attributes["data64_1"] = data; break;
          case 2: attributes["data64_2"] = data; break;
          default: _ASSERT(0);
        }
        data = 0;
      }
    }
      
    return true;
  }

  virtual bool Modulate(const CAttributes& attr, CArray<uint16_t>& pulse) override
  {
    int nibblesData[16];
    CArray<int> b(nibblesData, COUNT(nibblesData));
    int length = attr["length"];
    uint32_t data;
    int bytes = (length+7)/8;
    for (int i=0; i<bytes; i++)
    {
      if ((i&3) == 0)
      {
        switch (i/4)
        {
          case 0: data = attr["data64_0"]; break;
          case 1: data = attr["data64_1"]; break;
          case 2: data = attr["data64_2"]; break;
          default: _ASSERT(0);
        }
      }
      b.Add(data >> 24);
      data <<= 8;
    }

    return BytesToPulse(b, length, pulse);
  }
    virtual int PulseDivisor() override { return 350; }

private:
//var sig1 = [380,360,320,420,300,400,300,400,340,420,280,400,340,400,320,400,300,420,300,420,300,420,300,3820,700,380,740,360,720,400,720,380,700,380,720,400,700,400,680,400,700,420,680,420,700,360,720,400,340,760,700,400,700,380,360,740,720,380,340,780,700,400,700,380,720,380,340,760,720,380,320,780,720,380,720,380,320,760,720,380,720,400,700,380,720,400,700,400,700,380,340,760,340,780,300,800,300,780,720,360,340,780,320,780,300,800,720,360,340,780,340,740,360,760,300,780,340,760,700,400,340,780,680,400,700,400,340,780,320,760,320,760,340,760,720,380,320,780,720,380,340,760,720,400,300,780,700,400,720,400,320,760,340,760,300];

  int PulseLen(int microseconds)
  {
      //169, 167 p
    // 680-720 avg 700
    // 280-420 avg 350
    //return (microseconds+350/2)/350;
      return (microseconds+480/2)/480;
  }

  int PulseDuration(int ticks)
  {
      return ticks*350;
  }

  bool PulseToBytes(const CArray<uint16_t>& pulse, CArray<int>& bytes, int& length)
  {
    int i;
    for (i=0; i<pulse.GetSize(); i++)
    {
      int ticks = PulseLen(pulse[i]);
      if (ticks >= 9 && ticks <= 12)
      {
          i++;
        break;
      }
      if (ticks != 1)
        return false;
    }

    int num = 0;
    length = 0;
    for (; i<pulse.GetSize()-1; i+=2)
    {
        int bit;
      int t0 = PulseLen(pulse[i]);
      int t1 = PulseLen(pulse[i+1]);
      if (t0 == 1 && t1 == 2)
        bit = 0;
      else if (t0 == 2 && t1 == 1)
        bit = 1;
      else
        return false;

      num >>= 1;
      num |= bit<<7;

      if ((length & 7) == 7)
      {
        bytes.Add(num);
        num = 0;
      }
      length ++;
    }
    if ((length & 7) != 0)
    {
        bytes.Add(num);
    }
    return true;
  }

  bool BytesToPulse(const CArray<int>& bytes, int length, CArray<uint16_t>& pulse)
  {                                   
    const int preambule = 23;
    for (int i=0; i<preambule; i++)
      pulse.Add(PulseDuration(1));

    for (int i=0; i<length; i++)
    {
      int bit = (bytes[i/8] >> (7-(i&7)))&1;
      if (bit == 0)
      {
        pulse.Add(PulseDuration(1));
        pulse.Add(PulseDuration(2));
      } else
      {
        pulse.Add(PulseDuration(2));
        pulse.Add(PulseDuration(1));
      }
    }
    return true;
  }

    virtual void GetName(char* name) override
    {
        strcpy(name, "OregonScientific-1");
    }
    
    virtual void GetDescription(CAttributes& attributes, char* desc) override
    {
        sprintf(desc, "%d bits: <%08x %08x>", attributes["length"], attributes["data64_0"], attributes["data64_1"]);
    }

};
/*


static int oregon_scientific_v3_parser(r_device *decoder, bitbuffer_t *bitbuffer)
{
    bitrow_t *bb = bitbuffer->bb;
    data_t *data;

    // Check stream for possible Oregon Scientific v3 protocol data (skip part of first and last bytes to get past sync/startup bit errors)
    if ((((bb[0][0]&0xf) == 0x0f) && (bb[0][1] == 0xff) && ((bb[0][2]&0xc0) == 0xc0)) ||
            (((bb[0][0]&0xf) == 0x00) && (bb[0][1] == 0x00) && ((bb[0][2]&0xc0) == 0x00))) {
        int i,j;
        unsigned char msg[BITBUF_COLS] = {0};
        unsigned int sync_test_val = (bb[0][2]<<24) | (bb[0][3]<<16) | (bb[0][4]<<8);
        int dest_bit = 0;
        int pattern_index;
        // Could be extra/dropped bits in stream.    Look for sync byte at expected position +/- some bits in either direction
        for (pattern_index=0; pattern_index<16; pattern_index++) {
            unsigned int         mask = (unsigned int)(0xfff00000>>pattern_index);
            unsigned int    pattern = (unsigned int)(0xffa00000>>pattern_index);
            unsigned int pattern2 = (unsigned int)(0xff500000>>pattern_index);
            unsigned int pattern3 = (unsigned int)(0x00500000>>pattern_index);
            unsigned int pattern4 = (unsigned int)(0x04600000>>pattern_index);
            //fprintf(stdout, "OS v3 Sync nibble search - test_val=%08x pattern=%08x    mask=%08x\n", sync_test_val, pattern, mask);
            if (((sync_test_val & mask) == pattern)    || ((sync_test_val & mask) == pattern2) ||
                    ((sync_test_val & mask) == pattern3) || ((sync_test_val & mask) == pattern4)) {
                // Found sync byte - start working on decoding the stream data.
                // pattern_index indicates    where sync nibble starts, so now we can find the start of the payload
                int start_byte = 3 + (pattern_index>>3);
                int start_bit = (pattern_index+4) & 0x07;
                //fprintf(stdout, "Oregon Scientific v3 Sync test val %08x ok, starting decode at byte index %d bit %d\n", sync_test_val, start_byte, start_bit);
                j = start_bit;
                for (i=start_byte;i<BITBUF_COLS;i++) {
                    while (j<8) {
                        unsigned char bit_val = ((bb[0][i] & (0x80 >> j)) >> (7-j));

                        // copy every    bit from source stream to dest packet
                        msg[dest_bit>>3] |= (((bb[0][i] & (0x80 >> j)) >> (7-j)) << (7-(dest_bit & 0x07)));

                        //fprintf(stdout,"i=%d j=%d dest_bit=%02x bb=%02x msg=%02x\n",i, j, dest_bit, bb[0][i], msg[dest_bit>>3]);
                        if ((dest_bit & 0x07) == 0x07) {
                            // after assembling each dest byte, flip bits in each nibble to convert from lsb to msb bit ordering
                            int k = (dest_bit>>3);
                            unsigned char indata = msg[k];
                            // flip the 4 bits in the upper and lower nibbles
                            msg[k] = ((indata & 0x11) << 3) | ((indata & 0x22) << 1) |
                                ((indata & 0x44) >> 1) | ((indata & 0x88) >> 3);
                        }
                        dest_bit++;
                        j++;
                    }
                    j=0;
                }
                break;
            }
        }
        int sensor_id = (msg[0] << 8) | msg[1];
        if (sensor_id == ID_THGR810) {
            if (validate_os_checksum(decoder, msg, 15) == 0) {
                float temp_c = get_os_temperature(msg, sensor_id);
                int humidity = get_os_humidity(msg, sensor_id);
                data = data_make(
                    "brand",                    "",                     DATA_STRING, "OS",
                    "model",                    "",                     DATA_STRING, "THGR810",
                    "id",                         "House Code", DATA_INT,        get_os_rollingcode(msg, sensor_id),
                    "channel",                "Channel",        DATA_INT,        get_os_channel(msg, sensor_id),
                    "battery",                "Battery",        DATA_STRING, get_os_battery(msg, sensor_id)?"LOW":"OK",
                    "temperature_C",    "Celsius",        DATA_FORMAT, "%.02f C", DATA_DOUBLE, temp_c,
                    "humidity",             "Humidity",     DATA_FORMAT, "%u %%", DATA_INT, humidity,
                    NULL);
                decoder_output_data(decoder, data);
            }
            return 1;                                    //msg[k] = ((msg[k] & 0x0F) << 4) + ((msg[k] & 0xF0) >> 4);
        }
        else if (sensor_id == ID_THN802) {
                if (validate_os_checksum(decoder, msg, 12) == 0) {
                    float temp_c = get_os_temperature(msg, sensor_id);
                    data = data_make(
                        "brand",                    "",                     DATA_STRING, "OS",
                        "model",                    "",                     DATA_STRING, "THN802",
                        "id",                         "House Code", DATA_INT,        get_os_rollingcode(msg, sensor_id),
                        "channel",                "Channel",        DATA_INT,        get_os_channel(msg, sensor_id),
                        "battery",                "Battery",        DATA_STRING, get_os_battery(msg, sensor_id)?"LOW":"OK",
                        "temperature_C",    "Celsius",        DATA_FORMAT, "%.02f C", DATA_DOUBLE, temp_c,
                        NULL);
                    decoder_output_data(decoder, data);
                }
                return 1;
        }
        else if (sensor_id == ID_UV800) {
            if (validate_os_checksum(decoder, msg, 13) == 0) {     // ok
                int uvidx = get_os_uv(msg, sensor_id);
                data = data_make(
                    "brand",                    "",                     DATA_STRING, "OS",
                    "model",                    "",                     DATA_STRING, "UV800",
                    "id",                         "House Code", DATA_INT,        get_os_rollingcode(msg, sensor_id),
                    "channel",                "Channel",        DATA_INT,        get_os_channel(msg, sensor_id),
                    "battery",                "Battery",        DATA_STRING, get_os_battery(msg, sensor_id)?"LOW":"OK",
                    "uv",                         "UV Index",     DATA_FORMAT, "%u", DATA_INT, uvidx,
                    NULL);
                decoder_output_data(decoder, data);
            }
        }
        else if (sensor_id == ID_PCR800) {
            if (validate_os_checksum(decoder, msg, 18) == 0) {
                float rain_rate=get_os_rain_rate(msg, sensor_id);
                float total_rain=get_os_total_rain(msg, sensor_id);
                data = data_make(
                    "brand",            "",                     DATA_STRING, "OS",
                    "model",            "",                     DATA_STRING, "PCR800",
                    "id",                 "House Code", DATA_INT,        get_os_rollingcode(msg, sensor_id),
                    "channel",        "Channel",        DATA_INT,        get_os_channel(msg, sensor_id),
                    "battery",        "Battery",        DATA_STRING, get_os_battery(msg, sensor_id)?"LOW":"OK",
                    "rain_rate",    "Rain Rate",    DATA_FORMAT, "%3.1f in/hr", DATA_DOUBLE, rain_rate,
                    "rain_total", "Total Rain", DATA_FORMAT, "%3.1f in", DATA_DOUBLE, total_rain,
                    NULL);
                decoder_output_data(decoder, data);
            }
            return 1;
        }
        else if (sensor_id == ID_PCR800a) {
            if (validate_os_checksum(decoder, msg, 18) == 0) {
                float rain_rate=get_os_rain_rate(msg, sensor_id);
                float total_rain=get_os_total_rain(msg, sensor_id);
                data = data_make(
                        "brand",            "",                     DATA_STRING, "OS",
                        "model",            "",                     DATA_STRING, "PCR800a",
                        "id",                 "House Code", DATA_INT,        get_os_rollingcode(msg, sensor_id),
                        "channel",        "Channel",        DATA_INT,        get_os_channel(msg, sensor_id),
                        "battery",        "Battery",        DATA_STRING, get_os_battery(msg, sensor_id)?"LOW":"OK",
                        "rain_rate",    "Rain Rate",    DATA_FORMAT, "%3.1f in/hr", DATA_DOUBLE, rain_rate,
                        "rain_total", "Total Rain", DATA_FORMAT, "%3.1f in", DATA_DOUBLE, total_rain,
                        NULL);
                decoder_output_data(decoder, data);
            }
            return 1;
        }
        else if (sensor_id == ID_WGR800) {
            if (validate_os_checksum(decoder, msg, 17) == 0) {
                float gustWindspeed = (msg[5]&0x0f) /10.0F + ((msg[6]>>4)&0x0f) *1.0F + (msg[6]&0x0f) * 10.0F;
                float avgWindspeed = ((msg[7]>>4)&0x0f) / 10.0F + (msg[7]&0x0f) *1.0F + ((msg[8]>>4)&0x0f) * 10.0F;
                float quadrant = (0x0f&(msg[4]>>4))*22.5F;
                data = data_make(
                        "brand",            "",                     DATA_STRING,    "OS",
                        "model",            "",                     DATA_STRING,    "WGR800",
                        "id",                 "House Code", DATA_INT,         get_os_rollingcode(msg, sensor_id),
                        "channel",        "Channel",        DATA_INT,         get_os_channel(msg, sensor_id),
                        "battery",        "Battery",        DATA_STRING,    get_os_battery(msg, sensor_id)?"LOW":"OK",
                        "gust",             "Gust",             DATA_FORMAT,    "%2.1f m/s",DATA_DOUBLE, gustWindspeed,
                        "average",        "Average",        DATA_FORMAT,    "%2.1f m/s",DATA_DOUBLE, avgWindspeed,
                        "direction",    "Direction",    DATA_FORMAT,    "%3.1f degrees",DATA_DOUBLE, quadrant,
                        NULL);
                decoder_output_data(decoder, data);
            }
            return 1;
        }
        else if ((msg[0] == 0x20) || (msg[0] == 0x21) || (msg[0] == 0x22) || (msg[0] == 0x23) || (msg[0] == 0x24)) { //    Owl CM160 Readings
            msg[0]=msg[0] & 0x0f;
            if (validate_os_checksum(decoder, msg, 22) == 0) {
                float rawAmp = (msg[4] >> 4 << 8 | (msg[3] & 0x0f )<< 4 | msg[3] >> 4);
                unsigned short int ipower = (rawAmp /(0.27*230)*1000);
                data = data_make(
                        "brand",    "",                     DATA_STRING, "OS",
                        "model",    "",                     DATA_STRING,    "CM160",
                        "id",         "House Code", DATA_INT, msg[1]&0x0F,
                        "power_W", "Power",         DATA_FORMAT,    "%d W", DATA_INT, ipower,
                        NULL);
                    decoder_output_data(decoder, data);
            }
        }
        else if (msg[0] == 0x26) { //    Owl CM180 readings
                msg[0]=msg[0] & 0x0f;
                int valid = validate_os_checksum(decoder, msg, 23);
                int k;
                for (k=0; k<BITBUF_COLS;k++) {    // Reverse nibbles
                    msg[k] = (msg[k] & 0xF0) >> 4 |    (msg[k] & 0x0F) << 4;
                }
                unsigned short int ipower = power(msg);
                unsigned long long itotal = total(msg);
                float total_energy = itotal/3600/1000.0;
                if (itotal && valid == 0) {
                    data = data_make(
                            "brand",            "",                     DATA_STRING, "OS",
                            "model",            "",                     DATA_STRING,    "CM180",
                            "id",                 "House Code", DATA_INT, msg[1]&0x0F,
                            "power_W",        "Power",            DATA_FORMAT,    "%d W",DATA_INT, ipower,
                            "energy_kWh", "Energy",         DATA_FORMAT,    "%2.1f kWh",DATA_DOUBLE, total_energy,
                            NULL);
                    decoder_output_data(decoder, data);
                }
                else if (!itotal) {
                    data = data_make(
                            "brand",    "",                     DATA_STRING, "OS",
                            "model",    "",                     DATA_STRING,    "CM180",
                            "id",         "House Code", DATA_INT, msg[1]&0x0F,
                            "power_W", "Power",         DATA_FORMAT,    "%d W",DATA_INT, ipower,
                            NULL);
                    decoder_output_data(decoder, data);
                }
        }
        else if ((msg[0] != 0) && (msg[1]!= 0)) { //    sync nibble was found    and some data is present...
            if (decoder->verbose) {
                fprintf(stderr, "Message received from unrecognized Oregon Scientific v3 sensor.\n");
                bitrow_printf(msg, bitbuffer->bits_per_row[0], "Message: ");
                bitrow_printf(bb[0], bitbuffer->bits_per_row[0], "Raw: ");
            }
        }
        else if (bb[0][3] != 0 ) {
            if (decoder->verbose) {
                fprintf(stdout, "\nPossible Oregon Scientific v3 message, but sync nibble wasn't found\n");
                bitrow_printf(bb[0], bitbuffer->bits_per_row[0], "Raw Data: ");
            }
        }
    }
    else { // Based on first couple of bytes, either corrupt message or something other than an Oregon Scientific v3 message
        if (decoder->verbose) {
            if (bb[0][3] != 0) {
                fprintf(stdout, "\nUnrecognized Msg in v3: ");
                bitrow_printf(bb[0], bitbuffer->bits_per_row[0], "Raw Data: ");
            }
        }
    }
    return 0;
}

*/
