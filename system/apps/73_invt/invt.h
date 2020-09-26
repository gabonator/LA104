
class CVfd : public CVfdComm
{
  const long mReadTimeout{50};
  const long mNoResponseTimeout{500};
  
  typedef void(*TPacketHandler)(const CVfdPacket& p);
  typedef void(*TErrorHandler)();
  
  CVfdPacket mReceivePacket;
  long mLastReceive{0};
  long mPacketSent{0};
  uint8_t mDeviceAddress{1};
  enum ECommand
  {
    Read = 0x03,
    Write = 0x06,
    Diagnostics = 0x08,
    Abnormal = 0x80
  };

  TPacketHandler mPacketHandler{nullptr};
  TErrorHandler mErrorHandler{nullptr};
  
public:
  void setup()
  {
    CVfdComm::setup();
  }

  void loop()
  {
    long now = SYS::GetTick();
    
    if (available())
    {
      if (mReceivePacket.isFull())
      {
        Serial_print("ERROR: Receive packet full!\n");
        mReceivePacket.clear();
      }
      mReceivePacket.add(read());
      mLastReceive = now;
    }
    
    if (mLastReceive != 0 && now - mLastReceive > mReadTimeout)
    {
      _ASSERT(mReceivePacket.getLength() > 0);
      if (checkCrc(mReceivePacket))
      {
        if (mPacketHandler)
          mPacketHandler(mReceivePacket);
        else
          processPacket(mReceivePacket);
      } else
      {
        if (mErrorHandler)
          mErrorHandler();
        else
          Serial_print("ERROR: Broken packet read\n");        
      }
      
      mLastReceive = 0;
      mPacketSent = 0;
      mReceivePacket.clear();
      
      mPacketHandler = nullptr;
      mErrorHandler = nullptr;
    }

    if (mPacketSent != 0 && now - mPacketSent > mNoResponseTimeout)
    {
      if (mErrorHandler)
        mErrorHandler();
      else
        Serial_print("ERROR: Response timeout\n");

    
      mLastReceive = 0;
      mPacketSent = 0;
      mReceivePacket.clear();
      
      mPacketHandler = nullptr;
      mErrorHandler = nullptr;
    }
  }

  void send(const CVfdPacket& p)
  {
    Serial_print("Send ");
    printPacket(p);

    mReceivePacket.clear();
    write(p.getData(), p.getLength());
    mPacketSent = SYS::GetTick();
  }
  
  void buildWritePacket(CVfdPacket& p, uint16_t dataAddress, uint16_t dataValue)
  {
    p.clear();
    p.add((uint8_t)mDeviceAddress);
    p.add((uint8_t)ECommand::Write);
    p.add((uint16_t)dataAddress);
    p.add((uint16_t)dataValue);
    appendCrc(p);
  }

  void buildReadPacket(CVfdPacket& p, uint16_t dataAddress, int valueCount)
  {
    p.clear();
    p.add((uint8_t)mDeviceAddress);
    p.add((uint8_t)ECommand::Read);
    p.add((uint16_t)dataAddress);
    p.add((uint16_t)valueCount);
    appendCrc(p);
  }

  void onPacket(TPacketHandler handler)
  {
    mPacketHandler = handler;    
  }
  
  void onError(TErrorHandler handler)
  {
    mErrorHandler = handler;    
  }

  virtual void processPacket(const CVfdPacket& p)
  {
    Serial_print("Received ");
    printPacket(p);
  }

  void printPacket(const CVfdPacket& p)
  {  
    if (p.getLength() < 3)
    {
      Serial_print("Empty packet\n");
      return;
    }
    
    if (p[0] != mDeviceAddress)
      Serial_print("Unknown origin ");
    
    if (p[1] & ECommand::Abnormal)
    {
      Serial_print("Abnormal ");
      switch (p[2])
      {
        case 0x01: Serial_print("(Illegal command) "); break;
        case 0x02: Serial_print("(Illegal data address) "); break;
        case 0x03: Serial_print("(Illegal value) "); break;
        case 0x04: Serial_print("(Operation failed) "); break;
        case 0x05: Serial_print("(Password error) "); break;
        case 0x06: Serial_print("(Data frame error) "); break;
        case 0x07: Serial_print("(Read only) "); break;
        case 0x08: Serial_print("(Blocked during running) "); break;
        case 0x09: Serial_print("(Password protected) "); break;
      }
    }
      
    Serial_print("Packet: ");
    for (int i=0; i<p.getLength(); i++)
    {
      uint8_t v = p[i];
      char temp[4];
      sprintf(temp, "%02x ", v);
      Serial_print(temp);
    }
    Serial_print("\n");
  }

private:
  void appendCrc(CVfdPacket& p)
  {
    uint16_t crc = calculateCrc(p.getData(), p.getLength());
    p.add((uint8_t)(crc & 0xff));
    p.add((uint8_t)(crc >> 8));
  }

  bool checkCrc(CVfdPacket& p)
  {
    uint16_t crc = calculateCrc(p.getData(), p.getLength()-2);
    if (p[p.getLength()-2] != (crc & 0xff))
      return false;
    if (p[p.getLength()-1] != (crc >> 8))
      return false;
    return true;
  }
  
  uint16_t calculateCrc(const uint8_t *data_value, uint8_t data_length) 
  {
    uint16_t crc_value = 0xffff;
    while(data_length--)
    { 
      crc_value ^= *data_value++;
      for(uint8_t i=0; i<8; i++)
      { 
        if(crc_value&0x0001)
          crc_value=(crc_value>>1)^0xa001;
        else 
          crc_value >>= 1;
      }
    } 
    return crc_value;
  }
  
};
