// Date: 2023-08-16
// Created by: Grzegorz Rajtar, grzegorz@rajtar.info
/*
Somfy RTS Keytis

Preamble 12 times  2560us HIGH then 2560us LOW
4810us HIGH - soft sync
166 times half-clock(640us) manchester encoded bits

Frame repeat

Preamble 6 times  2560us HIGH then 2560us LOW
4810us HIGH - soft sync
162 times half-clock(640us) manchester encoded bits

*/
#define HALF_TIME_CLOCK 640
#define SOMFY_GUARD_TIME_US 1200
// 4 time HALF_TIME_CLOCK
#define PREAMBLE_PULSES 4
#define SOFT_SYNC_PULSES 7.5
#define SOFT_SYNC_US 4800
#define SOMFY_CODED_BITS 81


class CSomfyRts : public CProtocol
{
   
public:
    
    virtual int Frequency() override
    {
        return 433420000;
    }

    virtual void Example(CAttributes& attributes) override
    {
        attributes["length"] = SOMFY_CODED_BITS;
        attributes["data_0"] = 0x40a444d8;
        attributes["data_1"] = 0x27131efc;
        attributes["data_2"] = 0x3;
    }

    virtual bool Demodulate(const CArray<uint16_t>& pulse, CAttributes& attributes) override
    {
        uint8_t nibblesData[11];
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
    	
    	if (length < SOMFY_CODED_BITS)
    	   return;
    	   
    	if (b.GetSize() < 11) /*81 bit-s, 11bytes and a bit more*/
    	{
    	    return;
    	}    	   	
    }

    void BitstreamToAttributes(CArray<uint8_t>& b, int bitLength, CAttributes& attributes)
    {
    	if (bitLength < SOMFY_CODED_BITS)
    	{
    	   return;
    	}
    	
    	if (b.GetSize() < 11)
    	{
    	   return;
    	}    	
    	
    	
        attributes["length"] = bitLength; // count of bits
        
    	attributes["data_0"] = (unsigned)((b[0] << 24) | (b[1] << 16) | (b[2] << 8) | (b[3]));
      	attributes["data_1"] = (unsigned)((b[4] << 24) | (b[5] << 16) | (b[6] << 8) | (b[7]));  	    	
      	attributes["data_2"] = (unsigned)((b[8] << 24) | (b[9] << 16) | (b[10] << (8+7)));


    	attributes["rpl_cnt"] = 1; // replay counter for remote emulation - limited to 2 due to  framer pulses array size
    }


    void AttributesToBitstream(const CAttributes& attributes, CArray<uint8_t>& b, int& bitLength)
    {
        bitLength = attributes["length"];
        
        uint32_t d0 = attributes["data_0"];
        uint32_t d1 = attributes["data_1"];
        uint32_t d2 = attributes["data_2"];     
        
	b.Add(reverse8((d0 >> 24) & 0xff));
	b.Add(reverse8((d0 >> 16) & 0xff));
	b.Add(reverse8((d0 >> 8) & 0xff));
	b.Add(reverse8(d0 & 0xff));
	
	b.Add(reverse8((d1 >> 24) & 0xff));
	b.Add(reverse8((d1 >> 16) & 0xff));
	b.Add(reverse8((d1 >> 8) & 0xff));
	b.Add(reverse8(d1 & 0xff));
	
	b.Add(reverse8((d2 >> 24) & 0xff));
	b.Add(reverse8((d2 >> 16) & 0xff));
	b.Add(reverse8((d2 >> 8) & 0x80));
    }


    virtual bool Modulate(const CAttributes& attr, CArray<uint16_t>& pulse) override
    {
        uint8_t nibblesData[24];
        CArray<uint8_t> b(nibblesData, COUNT(nibblesData));
        int rpl_cnt = attr["rpl_cnt"];

        int length = 0;
        
        if (rpl_cnt <1 || rpl_cnt > 2)
           rpl_cnt = 1;
        
       
        AttributesToBitstream(attr, b, length);
        
        for (int rpl = 0; rpl < rpl_cnt; rpl++)
        {
	   if (!BytesToPulse(b, length, pulse, rpl))
	      return false;
	   //last pulse is for zero - add the time guard for frames separation
	   pulse[pulse.GetSize() - 1] += SOMFY_GUARD_TIME_US; //frame repeat pause
        }
        
       
        return true;
    }

    virtual int PulseDivisor() override { return HALF_TIME_CLOCK; }
    
private:

    unsigned char reverse8(unsigned char b) {
        b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
        b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
        b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
        return b;
    }

    int PulseLen(int microseconds)
    {
        return (microseconds+(HALF_TIME_CLOCK/2))/HALF_TIME_CLOCK;
    }

    int PulseDuration(int ticks)
    {
        return ticks*HALF_TIME_CLOCK;
    }

    bool PulseToBytes(const CArray<uint16_t>& pulse, CArray<uint8_t>& bytes, int& length)
    {
        int i;
        int preambule = 0;
        int pre_error = 0;
        
        
        //12 pulses Preamble, 1 pulse soft sync, 166 halfbits       
        
      
        if (pulse.GetSize() < 164+13)
           return false;
        
        for (i=0; i<pulse.GetSize(); i++)
        {
          int t = PulseLen(pulse[i]);
          if (t >=PREAMBLE_PULSES && t <= PREAMBLE_PULSES+1)
          {      
            preambule++;
            if (preambule  == 24 || (preambule == 12 && i > 30 /* repeated frame*/))
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
        

        if (preambule < 12 && preambule < 24) // should be 24 or 12 for repeated frame
            return false;
                     
        //should be SOFT_SYNC_PULSES --> 4800us
        if (pulse[i] >= SOFT_SYNC_US-(HALF_TIME_CLOCK/2) && pulse[i] <= SOFT_SYNC_US+(HALF_TIME_CLOCK/2)) {
            i++;
        }
        else {
              return false;
        }
            
        length = 0;
        int bits = 0;
        
        for (; i<pulse.GetSize()-1; i+=2)
        {
           //Manchester encoding 
           int p0 = PulseLen(pulse[i]);
           int p1 = PulseLen(pulse[i+1]);
           
           if (p0==1 && p1 ==1) //NRZ double transition means HIGH
           {
              length++;
              bits <<= 1;
              bits |= 1;
           }
           else if (p0==2) //NRZ no transition means LOW
           {
              length++;
              bits <<= 1;
              bits |= 0;
              i--;
           }
           else if (p0==1 && p1 ==3) //NRZ double transition plus frame gap ??
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
              bytes.Add(bits);
              bits = 0;
           }  
           
           
           if (length >= SOMFY_CODED_BITS)
               break;
        }
        
        
        if ((length & 7) != 0)
        {
              bytes.Add(bits);
        }
        
        if (length < SOMFY_CODED_BITS)
           return false;
        else
           return true;
    }

    bool BytesToPulse(const CArray<uint8_t>& bytes, int length, CArray<uint16_t>& pulse, int packet)
    {
        int preamble = 24;
        
        if (packet)
            preamble = 12;
        
        for (int i=0; i <preamble; i++)
            pulse.Add(PulseDuration(PREAMBLE_PULSES));

        pulse.Add(SOFT_SYNC_US);

	for (int i=0; i<length; i++)
	{
	     int bit = (bytes[i/8] >> (i&7)) & 1;
	     if (bit)
	     {
		pulse.Add(PulseDuration(1));
		pulse.Add(PulseDuration(1));
	     } else
	     {
		pulse.Add(PulseDuration(2));
	     }
        }
        
        return true;
    }
    

    virtual void GetName(char* name) override
    {
        strcpy(name, "Somfy RTS Keytis");
    }

    virtual void GetDescription(CAttributes& attributes, char* desc) override
    {
            sprintf(desc, "%d bits: <%08x %08x %08x>",
            	 (int)attributes["length"], 
            	 (int)attributes["data_0"], 
            	 (int)attributes["data_1"], 
            	 (int)attributes["data_2"]
            	 );
    }
    
    virtual const char* GetString(int i) override
    {
        return nullptr;
    }
};
