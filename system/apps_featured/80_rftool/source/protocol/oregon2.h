class COregon2 : public CProtocol
{
    enum {
        String_THGR810 = 2000,
        
        ID_THGR810 = 0xaf82
    };
    
public:
	virtual int Frequency() override
	{
		return 433876000UL;
	}
	
    virtual const char* GetString(int i) override
    {
        switch (i)
        {
            case String_THGR810: return "THGR810";
        }
        return nullptr;
    }
  virtual void Example(CAttributes& attributes) override
  {
    // -512 (-51.2 C) ... 999 (+99.9 C)
    attributes["length"] = 64;
    attributes["data_0"] = 0x12345678;
    attributes["data_1"] = 0xabcdef01;
  }

  virtual bool Demodulate(const CArray<uint16_t>& pulse, CAttributes& attributes) override
  {
    uint8_t nibblesData[24];
    CArray<uint8_t> b(nibblesData, COUNT(nibblesData));

    int length = 0;
    if (!PulseToBytes(pulse, b, length))
      return false;

    BitstreamToAttributes(b, length, attributes);

    Analyse(attributes);
    return true;
  }

  void Analyse(CAttributes& attributes)
  {
      //50 FA28A428 20229083 4B46
    const uint32_t d0 = attributes["data_0"];
    const uint32_t d1 = attributes["data_1"];
    uint8_t message[10] = {(uint8_t)(d0 >> 24), (uint8_t)(d0 >> 16), (uint8_t)(d0 >> 8), (uint8_t)(d0),
        (uint8_t)(d1 >> 24), (uint8_t)(d1 >> 16), (uint8_t)(d1 >> 8), (uint8_t)(d1)};

      auto nibble = [](uint8_t* p, int n)
      {
          int v = ((n&1)==0) ? p[n>>1] >> 4 : p[n>>1] & 0x0f;
          _ASSERT(v <= 9);
          return v;
      };
      
        #define nib(i) nibble(message, i)
      
      int sensor_id = (message[0] << 8) | message[1];

      if (sensor_id == ID_THGR810) // 0xaf824
      {
        attributes["$model"] = String_THGR810; //(uintptr_t)"THGR810";
        int temp_c = nib(11)*100 + nib(10)*10 + nib(9);
        if (nib(12) != 0)
            temp_c = -temp_c;
        int hum = nib(14)*10 + nib(13);

        attributes["temperature10"] = temp_c;
        attributes["humidity"] = hum;
    }
#undef nib
  }
    
    virtual void Synthesize(CAttributes& attributes) override
    {
        auto setnib = [&](int nibble, int value)
        {
            const char* key = "data_0";
            if (nibble < 16)
                key = "data_2";
            else if (nibble < 8)
                key = "data_1";
            
            int i = attributes.indexOf(key);
            if (i==-1)
            {
                BIOS::DBG::Print("oregon-2 synth error");
                //_ASSERT(0);
                return;
            }
            
            TKeyValue& kv = attributes[i];

            _ASSERT(value >= 0 && value <= 15);
            int bitofs = (7-(nibble&7))*4;
            kv.value &= ~(0xf<<bitofs);
            kv.value |= value<<bitofs;
        };
        
        if (attributes.indexOf("$model") != -1 && attributes["$model"] == String_THGR810)
        {
            int temp = (int)attributes["temperature10"];
            if (temp < 0)
                setnib(12, 0xf);
            else
                setnib(12, 0x0);
            temp = abs(temp);
            setnib(11, (temp/100)%10);
            setnib(10, (temp/10)%10);
            setnib(9, temp%10);
            
            int hum = abs((int)attributes["humidity"]);
            setnib(14, (hum/10)%10);
            setnib(13, hum%10);
        }
    }

    virtual bool Modulate(const CAttributes& attr, CArray<uint16_t>& pulse) override
    {
        uint8_t nibblesData[24];
        CArray<uint8_t> b(nibblesData, COUNT(nibblesData));
        int length;
        AttributesToBitstream(attr, b, length);
        return BytesToPulse(b, length, pulse);
    }

    virtual int PulseDivisor() override { return 350; }
    
private:
  int PulseLen(int microseconds)
  {
      return (microseconds+480/2)/480;
  }

  int PulseDuration(int ticks)
  {
      return ticks*350;
  }

  bool PulseToBytes(const CArray<uint16_t>& pulse, CArray<uint8_t>& bytes, int& length)
  {
    int i;
    for (i=0; i<pulse.GetSize()-4; i++)
    {
      int t = PulseLen(pulse[i]);
      if (t == 2)
        break;
      if (t != 1)
        return false;
    }

    if (i < 40) // should be 47 (45, 46, 47)
      return false;

    int l = 1;
    length = 0;
    int bits = 0;
    for (; i<pulse.GetSize(); i++)
    {
      bool isLast = i == pulse.GetSize() - 1;
      if (PulseLen(pulse[i]) == 1 && (isLast || PulseLen(pulse[i+1]) == 1))
      {
        if (l)
            bits |= 1 << (7-(length & 7));
          
        length++;
        //bits <<= 1;
        //bits |= l;
        i++;
      }
      else
      if (PulseLen(pulse[i]) == 2)
      {
          l = 1-l;
          if (l)
              bits |= 1 << (7-(length & 7));

          length++;
        //l = 1-l;
        //bits <<= 1;
        //bits |= l;
      }
      else        
      {
        return false;
      }

      if ((length & 7) == 0)
      {
        // swap nibbles
        bits = reverse(bits);
        bits = (bits >> 4) | ((bits & 15) << 4);
        bytes.Add(bits);
          if (bytes.GetMaxSize() == bytes.GetSize())
              return true;
        bits = 0;
      }
    }
      
      if ((length & 7) != 0)
      {
        // swap nibbles
        bits = reverse(bits);
        bits = (bits >> 4) | ((bits & 15) << 4);
        bytes.Add(bits);
          if (bytes.GetMaxSize() == bytes.GetSize())
              return true;
        bits = 0;
      }

    return true;
  }

  unsigned char reverse(unsigned char b) {
     b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
     b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
     b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
     return b;
  }

  bool BytesToPulse(const CArray<uint8_t>& bytes, int length, CArray<uint16_t>& pulse)
  {                                   
    const int preambule = 47; // parne cislo?
    for (int i=0; i<preambule; i++)
      pulse.Add(PulseDuration(1));

    int l = 1;
    for (int i=0; i<length; i++)
    {
      int bit = (bytes[i/8] >> (7-(i&7)))&1;
      if (l==bit)
      {
        pulse.Add(PulseDuration(1));
        pulse.Add(PulseDuration(1));
      } else
      {
        l = 1-l;
        pulse.Add(PulseDuration(2));
      }
    }
    return true;
  }

    virtual void GetName(char* name) override
    {
        strcpy(name, "OregonScientific-2");
    }
    
    virtual void GetDescription(CAttributes& attributes, char* desc) override
    {
        int model = 0;
        if (attributes.indexOf("$model") != -1)
            model = attributes["$model"];
        
        if (model == String_THGR810)
        {
            sprintf(desc, "<THGR810:> temp <%d.%d\xf8""C> hum <%d%%>", (int)attributes["temperature10"] / 10, (int)attributes["temperature10"] % 10, (int)attributes["humidity"]);
        }
        else
            sprintf(desc, "%d bits: <%08x %08x>", (int)attributes["length"], (int)attributes["data_0"], (int)attributes["data_1"]);
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
