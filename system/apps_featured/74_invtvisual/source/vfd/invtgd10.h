class CInvtGD10
{
	enum EIdentify
	{
		GD20EU = 0x0106,
		GD35 = 0x0108,
		GD35H1 = 0x0109,
		GD300 = 0x010a,
		GD100 = 0x010b,
		GD200 = 0x010c,
		GD10mini = 0x010d
	};

public:
	enum{
		RegIdentify = 0x2103,
		RegRealtimeBlock = 0x3001,
		RegFaultCode = 0x5000,
		RegState = 0x2100,
		RegRealtimeFrequency = 0x3001,
		RegRealtimeBusVoltage = 0x3002,
		RegRealtimeOutputVoltage = 0x3003,
		RegRealtimeOutputCurrent = 0x3004,
		RegRealtimeOperationSpeed = 0x3005,
		RegRealtimeOutputPower = 0x3006,
		RegRealtimeOutputTorque = 0x3007,
		RegRealtimePidSetting = 0x3008,
		RegRealtimePidFeedback = 0x3009
	};
  
public:
	static const char* GetRegName(int i)
	{
		switch (i)
		{
			case 0x2000: return "Operation";
			case 0x2001: return "Communication setting frequency";
			case 0x2002: return "PID reference";
			case 0x2003: return "PID feedback";
			case 0x200a: return "Virtual input terminal HI";
			case 0x200b: return "Virtual input terminal LO";
			case 0x200d: return "AO output";
			case 0x2100: return "SW1 Inverter operation";
			case 0x2101: return "SW1 Inverter";
			case 0x2102: return "Fault code";
			case 0x2103: return "Identify"; // EIdentify enum
			case 0x3001: return "Setting frequency";
			case 0x3002: return "BUS voltage";
			case 0x3003: return "Output voltage";
			case 0x3004: return "Output current";
			case 0x3005: return "Operation speed";
			case 0x3006: return "Output power";
			case 0x3007: return "Output torque";
			case 0x3008: return "PID setting";
			case 0x3009: return "PID feedback";
			case 0x300a: return "Input IO state";
			case 0x300b: return "Output IO state";
			case 0x300c: return "AI 1";
			case 0x300d: return "AI 2";
			case 0x3014: return "External counting value";
			case 0x3015: return "Torque setting";
			case 0x3016: return "Inverter code";
			case 0x5000: return "Fault code";
		}
		return nullptr;
	}

	static const char* GetFaultType(uint16_t n)
	{
		switch (n)
		{
			// page 45 https://www.invtcz.cz/data/product/product-file/4-file-2063187141.pdf
			case 0: return nullptr;
			case 11: return "Motor overload";
			case 12: return "Inverter overload";
			case 15: return "Rectifier overheat";
			case 16: return "Overheat";
			case 36: return "Underload fault LL";
			default: return "Unknown error";
		}
	}

	static const char* GetIdentification(uint16_t n)
	{
		switch (n)
		{
			case GD20EU: return "GD20-EU";
			case GD35: return "GD35";
			case GD35H1: return "GD35-H1";
			case GD300: return "GD300";
			case GD100: return "GD100";
			case GD200: return "GD200";
			case GD10mini: return "GD10 mini";
			default: return "Unknown device";
		}
	}
};
