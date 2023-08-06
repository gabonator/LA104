// Date: 2023-08-03
// Created by: Grzegorz Rajtar, grzegorz@rajtar.info
/*
Microchip HCS200/HCS300 KeeLoq Code Hopping Encoder based remotes.

66 bits transmitted, LSB first.

|  0-31 | Encrypted Portion
| 32-59 | Serial Number
| 60-63 | Button Status (S3, S0, S1, S2)
|  64   | Battery Low
|  65   | RPT bit, Fixed 1 ??? 
*/
#define HCS_GUARD_TIME_US 15600

class CHcs200 : public CProtocol
{
   
public:
    
    virtual int Frequency() override
    {
        return 433920000;
    }

    virtual void Example(CAttributes& attributes) override
    {
        attributes["length"] = 66;
        attributes["data_0"] = 0x40a444d8;
        attributes["data_1"] = 0x27131efc;
        attributes["data_2"] = 0x3;
    }

    virtual bool Demodulate(const CArray<uint16_t>& pulse, CAttributes& attributes) override
    {
        uint8_t nibblesData[20];
        CArray<uint8_t> b(nibblesData, COUNT(nibblesData));

        int length = 0;
        if (!PulseToBytes(pulse, b, length))
            return false;

        BitstreamToAttributes(b, length, attributes);
        Analyse(attributes, b, length);
        return true;
    }

    void Analyse(CAttributes& attributes, CArray<uint8_t>& b, int length)
    {
    	int btn;
    	
    	if (length < 66)
    	   return;
    	   
    	if (b.GetSize() < 9) /*66 bit-s, 8bytes and a bit more*/
    	{
    	    _ASSERT(0);
    	    return;
    	}    	   	
    }

    void BitstreamToAttributes(CArray<uint8_t>& b, int bitLength, CAttributes& attributes)
    {
    	int btn;
    	char buf[64];
    	
    	
    	if (bitLength < 66)
    	{
    	   _ASSERT(0);
    	   return;
    	}
    	
    	if (b.GetSize() < 9)
    	{
    	   _ASSERT(0);
    	   return;
    	}    	
    	
    	
        attributes["length"] = bitLength; // count of bits
        
    	attributes["hcs200_enc"] = ((unsigned)(b[3] << 24) | (b[2] << 16) | (b[1] << 8) | (b[0]));
    	    	
    	attributes["hcs200_serial"]    = (unsigned)(((b[7] & 0x0f) << 24) | (b[6] << 16) | (b[5] << 8) | (b[4]));

    	btn = ((b[7] & 0xf0) >> 4);
    	
    	attributes["btn_mask"] = btn;    // buttons bitmask S2,S1,S0,S3
    	attributes["learn"]       = (bool)(btn == 0x0f); //all buttons presed 
    	attributes["battery_low"] = (bool)((b[8] & 0x01) == 0x01);
    	attributes["rpt"]      = (bool)((b[8] & 0x02) == 0x02); //frame repetition flag
    	attributes["rpl_cnt"] = 1; // replay counter for remote emulation - limited to 2 due to  framer pulses array size
    }


    void AttributesToBitstream(const CAttributes& attributes, CArray<uint8_t>& b, int& bitLength)
    {
        bitLength = attributes["length"];
        
        uint32_t encrypted = attributes["hcs200_enc"];
        uint32_t serial = attributes["hcs200_serial"];
        int btn   = attributes["btn_mask"];
        int learn = attributes["learn"];
        int battery_low = attributes["battery_low"];
        int repeat = attributes["rpt"];
        int status;

        
	//encrypted LSB
	b.Add(encrypted & 0xff);
	b.Add((encrypted >> 8) & 0xff);
	b.Add((encrypted >> 16) & 0xff);			
	b.Add((encrypted >> 24) & 0xff);
	//serial 28bits
	b.Add((serial & 0xff));
	b.Add(((serial >> 8) & 0xff));
	b.Add(((serial >> 16) & 0xff));
	//serial-msb + btn
	if (learn)
	    btn = 0x0f;    
	    
	b.Add((((serial >> 24) & 0x0f) | ((btn & 0x0f) << 4)));
	status = 0;
	if (battery_low)
	   status |= 0x01;
	if (repeat)
	   status |= 0x02;
	b.Add((status & 0xff));
    }


    virtual bool Modulate(const CAttributes& attr, CArray<uint16_t>& pulse) override
    {
        uint8_t nibblesData[20];
        CArray<uint8_t> b(nibblesData, COUNT(nibblesData));
        int rpl_cnt = attr["rpl_cnt"];

        int length = 0;
        
        if (rpl_cnt <1 || rpl_cnt > 2)
           rpl_cnt = 1;
        
       
        AttributesToBitstream(attr, b, length);
        
        for (int rpl = 0; rpl < rpl_cnt; rpl++)
        {
	   if (!BytesToPulse(b, length, pulse))
	      return false;
	   //last pulse is for zero - add the time guard for frames separation
	   pulse[pulse.GetSize() - 1] += HCS_GUARD_TIME_US; //frame repeat pause
        }
        
       
        return true;
    }

    virtual int PulseDivisor() override { return 400; }
    
private:

    unsigned char reverse8(unsigned char b) {
        b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
        b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
        b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
        return b;
    }

    int PulseLen(int microseconds)
    {
       //TE min 280, typ 400, max 620
        return (microseconds+(380/2))/400;
    }

    int PulseDuration(int ticks)
    {
        return ticks*380;
    }

    bool PulseToBytes(const CArray<uint16_t>& pulse, CArray<uint8_t>& bytes, int& length)
    {
        int i;
        int preambule = 0;
        int pre_error = 0;
        
        if (pulse.GetSize() < 155)
           return false;
        
        for (i=0; i<pulse.GetSize(); i++)
        {
          int t = PulseLen(pulse[i]);
          if (t >=0 && t < 2)
          {      
            preambule++;
            if (preambule  == 23)
            {
             	i++;
               break;
            }
          }
          else
          {
            preambule = 0;
          }
        }

        if (preambule < 23) // should be 23*Te
            return false;
                      
        //should be 10*Te
        if (PulseLen(pulse[i]) >=9 && PulseLen(pulse[i]) <=11)
            i++;
        else {
              return false;
        }
                 
        length = 0;
        int bits = 0;
        bool buffer_fix = false;
        
        for (; i<pulse.GetSize()-1; i+=2)
        {
           int p0 = PulseLen(pulse[i]);
           int p1 = PulseLen(pulse[i+1]);
           if (p0==2 && p1==1)
           {
              length++;
              bits <<= 1;
           bits |= 0;
           }
           else if (p0==1 && p1==2)
           {
              length++;
              bits <<= 1;
              bits |= 1;
           }
           else        
           {
              return false;
           }

           if ((length & 7) == 0)
           {
              // swap nibbles
              bits = reverse8(bits);
              bytes.Add(bits);
              bits = 0;
           }
           
           //next item is last, final zero not in buffer ?
           if (pulse.GetSize() - i == 3) 
           {
              buffer_fix = true;
           }    
        }
        
        if (buffer_fix)
        {
        
           int p0 = PulseLen(pulse[i]);
           if (p0==2 /*&& p1==1*/)
           {
              length++;
              bits <<= 1;
           bits |= 0;
           }
           else if (p0==1 /*&& p1==2*/)
           {
              length++;
              bits <<= 1;
              bits |= 1;
           }
           else        
           {
              return false;
           }

           if ((length & 7) == 0)
           {
              // swap nibbles
              bits = reverse8(bits);
              bytes.Add(bits);
              bits = 0;
           }
           
        }
       
        //emulate last bit if not received 66 pulses
        if (length == 65)
        {
       	     bits <<= 0;
       	     length++;
        }
        
        if ((length & 7) != 0)
        {
              bits = reverse8(bits);
              bytes.Add(bits);
        }
        
        if (length < 66)
           return false;
        else
           return true;
    }

    bool BytesToPulse(const CArray<uint8_t>& bytes, int length, CArray<uint16_t>& pulse)
    {
        const int preambule = 23; // 23 * TE
        for (int i=0; i <preambule; i++)
            pulse.Add(PulseDuration(1));

        pulse.Add(PulseDuration(10));

	for (int i=0; i<length; i++)
	{
	     int bit = (bytes[i/8] >> (i&7)) & 1;
	     if (bit)
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
        strcpy(name, "Keeloq HCS200/300");
    }

    virtual void GetDescription(CAttributes& attributes, char* desc) override
    {
    
        if (attributes.indexOf("encrypted") != -1)
            sprintf(desc, "encrypted: <%08x>", (int)attributes["hcs200_enc"]);
   	if (attributes.indexOf("serial") != -1)
            sprintf(desc, "serial: <%07x>", (int)attributes["hcs200_serial"]);
        else
            sprintf(desc, "%d bits: <%08x %07x %04x %01x %01x>",
            	 (int)attributes["length"], (int)attributes["hcs200_enc"], (int)attributes["hcs200_serial"], 
            	 ((uint16_t)attributes["btn_mask"]), ((uint8_t)attributes["battery_low"]), ((uint8_t)attributes["rpt"]));
    }
    
    virtual const char* GetString(int i) override
    {
        return nullptr;
    }
};
