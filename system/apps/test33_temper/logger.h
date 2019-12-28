uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];

class CLogger
{
	bool mIsOpened{false};
	CBufferedWriter mWriter;
	long mBaseTime{0};
	long mLastTime{-1};
	bool mFirstLine{true};
	char* mLogName{nullptr};
	
public:
	char* GetLogName(int n)
	{
		static char name[16];
		sprintf(name, "temper%02d.htm", n);
		return name;
	}
	
	void Open()
	{
		BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);
		int i;
		for (i=0; i<99; i++)
		{
			mLogName = GetLogName(i);
			if (!Exists(mLogName))
				break;
		}
		
		mIsOpened = mWriter.Open(mLogName);
		
		_ASSERT(mIsOpened);
		mBaseTime = BIOS::SYS::GetTick();
		
		mWriter << "<script src=\"https://app.valky.eu/hw/la104-temperature-visualizer/?v=1\"></script>\n";
		mWriter << "<script>\n";
		mWriter << "_head({device:\"" << (const char*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DeviceType) 
		  << "\", file:\"" << (const char*) mLogName << "\"});\n";
		mWriter << "_data([\n";
	}
	
	bool Exists(char* fileName)
	{
		if (BIOS::FAT::Open(fileName, BIOS::FAT::EIoMode::IoRead) != BIOS::FAT::EResult::EOk)
			return false;
		
		BIOS::FAT::Close();
		return true;
	}
	
	void Close()
	{
		if (!mIsOpened)
		{
			BIOS::FAT::SetSharedBuffer(nullptr);
			return;
		}
		
		mWriter << "\n]);\n";
		mWriter << "</script>\n";

		mIsOpened = false;
		mWriter.Close();
		BIOS::FAT::SetSharedBuffer(nullptr);
	}
	
	void Append(CArray<int>& values)
	{
		int ts = (BIOS::SYS::GetTick() - mBaseTime) / 1000;
		if (ts == mLastTime)
			return;
		mLastTime = ts;

		if (mFirstLine)
			mFirstLine = false;
		else
			mWriter << ",\n";

		char buf[16];
		sprintf(buf, "[%d", ts);
		mWriter << buf;
		for (int i=0; i<values.GetSize(); i++)
			if (values[i] != CSeriesBase::Invalid)
			{
				sprintf(buf, ", %f", values[i]/16.0f);
				mWriter << buf;
			}
		mWriter << "]";
	}
	
	bool IsOpened()
	{
		return mIsOpened;
	}
	
	const char* GetLogName()
	{
		return mLogName;
	}
};

