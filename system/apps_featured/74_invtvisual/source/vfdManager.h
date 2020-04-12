class CVfdManager
{
	CVfdAttributes mAttributes;
	CVfd gVfd;

	const long mRequestLimit{100};
	int mRequestBase{0};
	int mRequestCount{0};
	int mOkPacketCounter{0};
	
	bool mNotifyComm{false};
	bool mNotifyError{false};
	bool mNotifyId{false};
	bool mNotifyData{false};
	
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

		static long lastRequest = 0;
		static bool processing = false;

		long now = SYS::GetTick();
		if (now - lastRequest > mRequestLimit && !processing)
		{
			lastRequest = now;
			processing = true;

			CVfdPacket packet;
			buildRequest(packet);
			
			gVfd
				.send(packet)
				.onPacket([](const CVfdPacket& packet) {
					processing = false;
					_this->onPacket(packet);
				})
				.onError([]() {
					processing = false;
					_this->onError();
				});
		}
		
		gVfd.loop();
	}
	
	void onPacket(const CVfdPacket& packet)
	{
		// only read packets
		if (packet.getLength() < 5)
		{
			mNotifyError = true;
			return;
		}
		
		if (packet[0] != 0x01 ||
			packet[1] != 0x03 ||
			packet[2] != 2*mRequestCount ||
			packet.getLength() != 3+mRequestCount*2+2)
		{
			mNotifyError = true;
			return;
		}

		
		for (int i=3, reg = mRequestBase; i<packet.getLength()-2; i+=2, reg++)
		{
			int value = packet.getWord(i);

			switch (reg)
			{
				case CInvtGD10::RegIdentify:
					mAttributes.mIdentify = CInvtGD10::GetIdentification(value);
					mNotifyId = true;
					break;
				case CInvtGD10::RegState:
					switch (value)
					{
						case 1: mAttributes.mOperation = CVfdAttributes::EOperation::ForwardRunning; break;
						case 2: mAttributes.mOperation = CVfdAttributes::EOperation::ReverseRunning; break;
						case 3: mAttributes.mOperation = CVfdAttributes::EOperation::Stop; break;
						case 4: mAttributes.mOperation = CVfdAttributes::EOperation::Fault; break;
						case 5: mAttributes.mOperation = CVfdAttributes::EOperation::Off; break;
						default: mAttributes.mOperation = CVfdAttributes::EOperation::Unknown; break;
					}
					break;
				case CInvtGD10::RegFaultCode:
					mAttributes.mErrorCode = value;
					break;
				case CInvtGD10::RegRealtimeFrequency:
					mAttributes.mFrequency = value * 0.01f;
					break;
				case CInvtGD10::RegRealtimeBusVoltage:
					mAttributes.mBusVoltage = value * 0.1f;
					break;
				case CInvtGD10::RegRealtimeOutputVoltage:
					mAttributes.mVoltage = value;
					break;
				case CInvtGD10::RegRealtimeOutputCurrent:
					mAttributes.mCurrent = value * 0.1f;
					break;
				case CInvtGD10::RegRealtimeOperationSpeed:
					mAttributes.mSpeed = value;
					break;
				case CInvtGD10::RegRealtimeOutputPower:
					break;
				case CInvtGD10::RegRealtimeOutputTorque:
					break;
				case CInvtGD10::RegRealtimePidSetting:
					mAttributes.mPidTarget = value * 0.01f;
					break;
				case CInvtGD10::RegRealtimePidFeedback:
					mAttributes.mPidValue = value * 0.01f;
					break;
			}
		}
		
		if (++mOkPacketCounter == 3)
		{
			mNotifyData = true;
		}
		mNotifyComm = true;
	}
	
	void onError()
	{
		mNotifyError = true;
	}
		
	void buildRequest(CVfdPacket& packet)
	{
		mRequestBase = 0;
		mRequestCount = 0;
		
		if (!mAttributes.GetIdentify())
		{
			mOkPacketCounter = -1;
			mRequestBase = CInvtGD10::RegIdentify;
			mRequestCount = 1;
			gVfd.buildReadPacket(packet, mRequestBase, mRequestCount);
		} else
		{
			static int seq = 0;
			switch (seq++ % 3)
			{
				case 0:
					mOkPacketCounter = 0;
					mRequestBase = CInvtGD10::RegRealtimeBlock;
					mRequestCount = 9;
					break;
				case 1:
					mRequestBase = CInvtGD10::RegState;
					mRequestCount = 1;
					break;
				case 2:
					mRequestBase = CInvtGD10::RegFaultCode;
					mRequestCount = 1;
					break;
			}
		}
		_ASSERT(mRequestBase && mRequestCount);
		gVfd.buildReadPacket(packet, mRequestBase, mRequestCount);
	}
	
	bool IsData()
	{
		if (!mNotifyData)
			return false;
		mNotifyData = false;
		return true;
	}
	
	bool IsComm()
	{
		if (!mNotifyComm)
			return false;
		mNotifyComm = false;
		return true;
	}
	
	bool IsConnect()
	{
		if (!mNotifyId)
			return false;
		mNotifyId = false;
		return true;
	}
	
	bool IsCommError()
	{
		if (!mNotifyError)
			return false;
		mNotifyError = false;
		return true;
	}
};

class CVfdManager_Fake
{
	CVfdAttributes mAttributes;

public:
	CVfdAttributes& GetAttributes()
	{
		return mAttributes;
	}
	
	void setup()
	{
	}
	
	void loop()
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
	
	bool IsData()
	{
		static int x = 0;
		return (x++ > 10) && (x % 2 == 0);
	}
	bool IsComm()
	{
		static int x = 0;
		return x++ % 3 == 0;
	}
	bool IsConnect()
	{
		static int x = 0;
		return x++ == 5;
	}
	bool IsCommError()
	{
		return false;
	}
};
