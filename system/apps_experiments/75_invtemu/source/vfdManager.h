
class CVfdManager
{
	CVfdAttributes mAttributes;
	CVfd gVfd;
	
public:
	CVfdAttributes& GetAttributes()
	{
		return mAttributes;
	}
	
	void setup()
	{
		gVfd.setup();
	}
		
	void loop()
	{
		static CVfdManager* _this = this;
			
		gVfd.onPacket([](const CVfdPacket& packet) {
					_this->onPacket(packet);
				})
				.onError([]() {
					_this->onError();
				});
		
		gVfd.loop();
	}
	
	void onPacket(const CVfdPacket& packet)
	{
		int nReqAddress = 0, nReqCount = 0;
		if (isDataRequestPacket(packet, nReqAddress, nReqCount))
		{
			CONSOLE::Print(".");
			CVfdPacket response;
			response.add((uint8_t)0x01);
			response.add((uint8_t)0x03);
			response.add((uint8_t)(2*nReqCount));
			for (int i=nReqAddress; i<nReqAddress+nReqCount; i++)
				response.add((uint16_t)getRegister(i));
			gVfd.appendCrc(response);

			gVfd.send(response);
		} else
		{
			CONSOLE::Print("Unrecognized: ");
			gVfd.printPacket(packet);
		}
	}

	int getRegister(int reg)
	{
			switch (reg)
			{
				case CInvtGD10::RegIdentify: return CInvtGD10::EIdentify::GD100;
				case CInvtGD10::RegState: return (int)mAttributes.mOperation;
				case CInvtGD10::RegFaultCode: return mAttributes.mErrorCode;
				case CInvtGD10::RegRealtimeFrequency: return mAttributes.mFrequency * 100;
				case CInvtGD10::RegRealtimeBusVoltage: return mAttributes.mBusVoltage * 10;
				case CInvtGD10::RegRealtimeOutputVoltage: return mAttributes.mVoltage;
				case CInvtGD10::RegRealtimeOutputCurrent: return mAttributes.mCurrent * 10;
				case CInvtGD10::RegRealtimeOperationSpeed: return mAttributes.mSpeed;
				case CInvtGD10::RegRealtimeOutputPower: return 0;
				case CInvtGD10::RegRealtimeOutputTorque: return 0;
				case CInvtGD10::RegRealtimePidSetting: return mAttributes.mPidTarget * 100;
				case CInvtGD10::RegRealtimePidFeedback: return mAttributes.mPidValue * 100;
			}
		_ASSERT(0);
		return 0;
	}

	void randomize()
	{
		mAttributes.mErrorCode = ((BIOS::SYS::GetTick()/5000)&1) ? 36 : 0;
		mAttributes.mIdentify = "GD10 mini";
		mAttributes.mPidValue = {3.1};
		mAttributes.mPidTarget = ((BIOS::SYS::GetTick()/10)%550) / 100.0f;
		mAttributes.mOperation = mAttributes.mErrorCode ? CVfdAttributes::Fault : CVfdAttributes::ForwardRunning;
		mAttributes.mFrequency = mAttributes.mPidTarget > 3.0f ? 35.0f : 0.0f;
		mAttributes.mCurrent = 1.7f;
		mAttributes.mVoltage = 231.f;
		mAttributes.mBusVoltage = 450.9f;
		mAttributes.mSpeed = 1234;
	}

	bool isDataRequestPacket(const CVfdPacket& packet, int& baseAddress, int& readCount)
	{
		if (packet.getLength() != 8)
			return false;

		if (packet[0] != 0x01 || packet[1] != CVfd::ECommand::Read)
			return false;

		baseAddress = packet.getWord(2);
		readCount = packet.getWord(4);
//CONSOLE::Color(RGB565(ff00ff));
//CONSOLE::Print("base=%x, cnt=%x, ", baseAddress, readCount);
//CONSOLE::Color(RGB565(ffffff));
//		if (packet.getLength() != 2 + readCount*2 + 2)
//			return false;
		return true;
	}

	void onError()
	{
		CONSOLE::Print("Read error\n");
	}
};
