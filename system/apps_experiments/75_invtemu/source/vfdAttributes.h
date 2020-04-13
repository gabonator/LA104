class CVfdAttributes
{
	friend class CVfdManager_Fake;
	friend class CVfdManager;
	
public:
	enum EOperation
	{
		Unknown,
		ForwardRunning,
		ReverseRunning,
		Stop,
		Fault,
		Off,
	};
	
protected:
	int mErrorCode{0};
	const char* mIdentify{nullptr};
	float mPidValue{0};
	float mPidTarget{0};
	EOperation mOperation{Unknown};
	float mFrequency{0};
	float mCurrent{0};
	float mVoltage{0};
	float mBusVoltage{0};
	int mSpeed{0};
	
public:
	int GetErrorCode()
	{
		return mErrorCode;
	}

	const char* GetError()
	{
		if (mErrorCode == 0)
			return nullptr;

		const char* errorMessage = CInvtGD10::GetFaultType(mErrorCode);
		_ASSERT(errorMessage);
		return errorMessage;
	}
	
	const char* GetIdentify()
	{
		return mIdentify;
	}
	
	float GetPidValue()
	{
		return mPidValue;
	}
	
	float GetPidTarget()
	{
		return mPidTarget;
	}
	
	EOperation GetOperation()
	{
		return mOperation;
	}
	
	const char* GetOperationString()
	{
		static const char* operation[] = {"Unknown", "Running forward", "Running backward", "Stopped", "Fault", "Off"};
		return operation[GetOperation()];
	}

	float GetFrequency()
	{
		return mFrequency;
	}

	float GetCurrent()
	{
		return mCurrent;
	}

	float GetVoltage()
	{
		return mVoltage;
	}

	float GetBusVoltage()
	{
		return mBusVoltage;
	}

	int GetSpeed()
	{
		return mSpeed;
	}
};
