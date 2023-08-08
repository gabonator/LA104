// Date: 2023-08-06
// Created by: Grzegorz Rajtar, grzegorz@rajtar.info
/*
EV1527 static code remotes.

 Preamble 1 pulse HIGH, 31 pulses low
 code - 20 bits,
 d0,d1,d2,d3 - buttons bits
 
 data HIGH: 3pulses H, 1puse L
 data LOW:  1pulse H, 3 pulses L
*/
#define EV1527_GUARD_TIME_US 150

class CEV1527 : public CProtocol
{
   
public:
    
    virtual int Frequency() override
    {
        return 433920000;
    }

    virtual void Example(CAttributes& attributes) override
    {
        attributes["length"] = 24;
        attributes["data_0"] = 0x40a444d8;
    }

    virtual bool Demodulate(const CArray<uint16_t>& pulse, CAttributes& attributes) override
    {
        uint8_t nibblesData[4];
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
    	
    	if (length < 24)
    	   return;
    	   
    	if (b.GetSize() < 3) /*24 bit-s, 3bytes*/
    	{
    	    return;
    	}    	   	
    }

    void BitstreamToAttributes(CArray<uint8_t>& b, int bitLength, CAttributes& attributes)
    {
    	int btn;
    	
    	if (bitLength < 24)
    	{
    	   return;
    	}
    	
    	if (b.GetSize() < 3)
    	{
    	   return;
    	}    	
    	
    	
        attributes["length"] = bitLength; // count of bits
        
    	attributes["code"] = (unsigned)((b[0] << 16) | (b[1] << 8) | b[2]);

    	btn = (b[2] & 0x0f);
    	
    	attributes["btn"] = btn;    // buttons bitmask S2,S1,S0,S3
    	attributes["rpl_cnt"] = 1; // replay counter for remote emulation - limited to 8 due to  framer pulses array size
    }


    void AttributesToBitstream(const CAttributes& attributes, CArray<uint8_t>& b, int& bitLength)
    {
        bitLength = attributes["length"];
        
        uint32_t code = attributes["code"];
        int btn   = attributes["btn"];

        
	//code 20bit, btn  bits
	b.Add(reverse8((code >> 16) & 0xff));
	b.Add(reverse8((code >> 8)  & 0xff));
	b.Add(reverse8((code & 0xf0) | (btn & 0x0f)));
    }


    virtual bool Modulate(const CAttributes& attr, CArray<uint16_t>& pulse) override
    {
        uint8_t nibblesData[3];
        CArray<uint8_t> b(nibblesData, COUNT(nibblesData));
        int rpl_cnt = attr["rpl_cnt"];

        int length = 0;
        
        if (rpl_cnt <1 || rpl_cnt > 8)
           rpl_cnt = 1;
        
       
        AttributesToBitstream(attr, b, length);
        
        for (int rpl = 0; rpl < rpl_cnt; rpl++)
        {
	   if (!BytesToPulse(b, length, pulse))
	      return false;
	   //last pulse is for zero - add the time guard for frames separation
	   pulse[pulse.GetSize() - 1] += EV1527_GUARD_TIME_US; //frame repeat pause
        }
        
       
        return true;
    }

    virtual int PulseDivisor() override { return pulse_divisor; }
    
private:

    int pulse_hist[10];
    int pulse_hist_idx[10];
    int pulse_divisor = 250;

    unsigned char reverse8(unsigned char b) {
        b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
        b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
        b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
        return b;
    }

    int PulseLen(int microseconds)
    {
        return (microseconds+(pulse_divisor/2))/pulse_divisor;
    }

    int PulseDuration(int ticks)
    {
        return ticks*(pulse_divisor + 20);
    }
    
    int pulses_histogram(const CArray<uint16_t>& pulse, int* hist, int* hist_val_idx, int hist_size)
    {
        int res = 250;
	memset(hist, 0, hist_size);
	memset(hist_val_idx, 0, hist_size);
	
	for (int idx = 0; idx < pulse.GetSize(); idx++)
	{
	    for (int val_idx = 0; val_idx < hist_size; val_idx++)
	    {
	        if (hist_val_idx[val_idx] == pulse[idx] || hist_val_idx[val_idx] == 0)
	        {
	           hist_val_idx[val_idx] = pulse[idx];
	           hist[val_idx]++;
	        }
	    }	
	}
	
	int high_idx = -1;
	for (int val_idx = 0; val_idx < hist_size; val_idx++)
	{
	    if ( hist_val_idx[val_idx] >= 160 && hist_val_idx[val_idx] < 500)
	    {
	         if (high_idx == -1 || hist[val_idx] > hist[high_idx])
	            high_idx = val_idx;
	         
	    }
	}
	
	if (high_idx != -1)
	   res = hist_val_idx[high_idx];
	
	return res;
    }
    

    bool PulseToBytes(const CArray<uint16_t>& pulse, CArray<uint8_t>& bytes, int& length)
    {
        int i;
        bool preambule = false;
        
        int hist[4];
        int hist_idx[4];
        
        if (pulse.GetSize() < (2 + (2* 24)))
           return false; 
                     
         pulse_divisor = pulses_histogram(pulse, pulse_hist, pulse_hist_idx, sizeof(pulse_hist_idx));
        
        for (i=0; i<pulse.GetSize()-1; i++)
        {
          int t1 = PulseLen(pulse[i]);
          int t2 = PulseLen(pulse[i+1]);
          
          if (t1 == 1 && (t2 >=30 && t2 <= 34))
          {
             preambule = true;
             i += 2;
             break;
	  }

        }

        if (!preambule) {
            return false;
        }
                 
        length = 0;
        int bits = 0;
        bool buffer_fix = false;
        
        for (; i<pulse.GetSize()-1; i+=2)
        {
           int p0 = PulseLen(pulse[i]);
           int p1 = PulseLen(pulse[i+1]);
           if (p0==3 && (p1==1 || (p1 >=1 && i+2 >= pulse.GetSize()-1))) /* final pulse, artificially set to frame spacer*/
           {
              length++;
              bits <<= 1;
              bits |= 1;
           }
           else if (p0==1 && (p1==3 || (p1 >=3 && i+2 >=pulse.GetSize()-1)))  /* final pulse, artificially set to frame spacer*/
           {
              length++;
              bits <<= 1;
              bits |= 0;
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
           
           //next item is last, final zero not in buffer ?
           if (pulse.GetSize() - i == 3) 
           {
              buffer_fix = true;
           }    
          
           if (length == 24)
               break; 
        }
        
        if (buffer_fix)
        {
        
           int p0 = PulseLen(pulse[i]);
           if (p0==3 /*&& p1==1*/)
           {
              length++;
              bits <<= 1;
              bits |= 1;
           }
           else if (p0==1 /*&& p1==3*/)
           {
              length++;
              bits <<= 1;
              bits |= 0;
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
           
        }

        
        if ((length & 7) != 0)
        {
              bytes.Add(bits);
        }
                
        if (length < 24)
        {
           return false;
        }
        else
           return true;
    }

    bool BytesToPulse(const CArray<uint8_t>& bytes, int length, CArray<uint16_t>& pulse)
    {
       
        pulse.Add(PulseDuration(1));
        pulse.Add(PulseDuration(31));

	for (int i=0; i<length; i++)
	{
	     int bit = (bytes[i/8] >> (i&7)) & 1;
	     if (bit)
	     {
		pulse.Add(PulseDuration(3));
		pulse.Add(PulseDuration(1));
	     } else
	     {
		pulse.Add(PulseDuration(1));
		pulse.Add(PulseDuration(3));
	     }
        }
        
        return true;
    }

    virtual void GetName(char* name) override
    {
        strcpy(name, "EV1527 Encoder");
    }

    virtual void GetDescription(CAttributes& attributes, char* desc) override
    {
        sprintf(desc, "%d bits: <%05x %04x>",
       	(int)attributes["length"], (int)attributes["code"], attributes["btn"]);
    }
    
    virtual const char* GetString(int i) override
    {
        return nullptr;
    }
};
