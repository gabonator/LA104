//volatile uint32_t streamerTotalHigh;
//volatile uint32_t streamerTotalLow;

class CControlLayout : public CBlock
{
public:
    const int Height = 22;
    int mFocus{0};

    virtual void OnPaint() override
    {
        using namespace Layout;

        CBlock::OnPaint();
        Render(m_rcClient)
            << Padding(8, 4, 8, 4)
            << Select(CWnd::HasFocus() && mFocus == 0) << Button((char*)"Pause")
            << Goto(m_rcClient.CenterX())
            << Select(CWnd::HasFocus() && mFocus == 1) << Radio(appData.GetLogging(), (char*)"Logging");
    }
    
    virtual void OnKey(int key) override
    {
        if (key == BIOS::KEY::Enter && mFocus == 1)
        {
            appData.SetLogging(!appData.GetLogging());
            Invalidate();
            return;
        }
        if (key == BIOS::KEY::Up)
        {
            if (mFocus > 0)
            {
                mFocus--;
                Invalidate();
                return;
            }
        }
        if (key == BIOS::KEY::Down)
        {
            if (mFocus < 2)
            {
                mFocus++;
                Invalidate();
                return;
            }
        }
        CBlock::OnKey(key);
    }
};

class CModemLayout : public CBlock
{
public:
	const int Height = 66;
	int mFocus = 5;
	
	bool HasFocus(int i)
	{
		return CWnd::HasFocus() && mFocus == i;
	}
	
	virtual void OnPaint() override
	{
        using namespace Layout;

        auto formatMhz = [](char* buffer, int nFrequency)
        {
            nFrequency /= 10000; // 344 MHZ -> 34400
            int nMhz = nFrequency / 100;
            int nKhz = nFrequency % 100;
            sprintf(buffer, "%d.%02d", nMhz, nKhz);
            return buffer;
        };
        auto formatKhz = [](char* buffer, int nFrequency)
        {
            nFrequency /= 1000; // 344 MHZ -> 34400
            sprintf(buffer, "%d", nFrequency);
            return buffer;
        };
        auto formatNumber = [](char* buffer, int n)
        {
            sprintf(buffer, "%d", n);
            return buffer;
        };
        auto formatKNumber = [](char* buffer, int n)
        {
            n /= 100;
            sprintf(buffer, "%d.%d", n/10, n%10);
            return buffer;
        };

		CBlock::OnPaint();
        int nFrequency = appData.GetFrequency();
        int nBandwidth = appData.GetBandwidth();
        int nGain = appData.GetGain();
        int nDataRate = appData.GetDataRate();
        char buffer[16];
        
        Render r(m_rcClient);
        
        r   << Padding(8, 4, 8, 4);
        
        if (appData.GetConnected())
        {
            r   << "Status:" << " " << Color(RGB565(00ff00)) << "Connected" << Color(RGB565(b0b0b0))
                << Select(HasFocus(0)) << Button((char*)"Disconnect") << Select(false) << " ";
        } else
        {
            r   << "Status:" << " " << Color(RGB565(ff0000)) << "Disconnected" << Color(RGB565(b0b0b0))
                << Select(HasFocus(0)) << Button((char*)"Connect") << Select(false) << " ";
        }

        r   << NewLine() << Color(RGB565(b0b0b0))
            << "Freq:" << Select(HasFocus(1)) << Units(formatMhz(buffer, nFrequency), "Mhz")
            << Goto(m_rcClient.CenterX())
            << "Bandwidth:" << Color(RGB565(00ff00)) << Select(HasFocus(2)) << Units(formatKhz(buffer, nBandwidth), "kHz")
            << NewLine()
            << "Gain:" << Select(HasFocus(3)) << Units(formatNumber(buffer, nGain), "dB")
            << Goto(m_rcClient.CenterX())
            << "Datarate:" << Select(HasFocus(4)) << Units(formatKNumber(buffer, nDataRate), "kBps")
            << NewLine() << Goto(m_rcClient.CenterX())
            << Select(HasFocus(5)) << Button((char*)"Load") << Select(false) << " "
            << Select(HasFocus(6)) << Button((char*)"Save") << Select(false);
	}
	
	virtual void OnKey(int key) override
	{
		int dir = 0;
		if (key == BIOS::KEY::Left)
			dir = -1;
		if (key == BIOS::KEY::Right)
			dir = +1;
		if (dir != 0)
		{
			switch (mFocus)
			{
                case 1: appData.DeltaFrequency(dir); break;
                case 2: appData.DeltaBandwidth(dir); break;
                case 3: appData.DeltaGain(dir); break;
                case 4: appData.DeltaDataRate(dir); break;
			}
            DataChanged();
		}
        
        if (key == BIOS::KEY::Enter && mFocus == 1)
        {
            appData.ToggleFrequency();
            DataChanged();
        }

        if (key == BIOS::KEY::Enter && mFocus == 0)
        {
            // dispatch to main
            if (!appData.GetConnected())
            {
                appData.SetConnected(framerStart());
            }
            else
            {
                framerStop();
                appData.SetConnected(false);
            }
            Invalidate();
        }
        
        if (key == BIOS::KEY::Enter && mFocus == 5)
        {
            SendMessage(GetParent(), 0, (uintptr_t)"load");
        }

        if (key == BIOS::KEY::Enter && mFocus == 6)
        {
            SendMessage(GetParent(), 0, (uintptr_t)"save");
        }
        
		if (key == BIOS::KEY::Up)
		{
			if (mFocus > 0)
			{
				mFocus--;
				Invalidate();
				return;
			}
		}
		if (key == BIOS::KEY::Down)
		{
			if (mFocus < 6)
			{
				mFocus++;
				Invalidate();
				return;
			}
		}
		CBlock::OnKey(key);
	}
    
    void DataChanged()
    {
        Invalidate();
        SendMessage(GetParent(), 0, (uintptr_t)"update");
    }
};

class CPreview : public CBlock
{
public:
	const int Height = 42;
	int mSelected = 0;
	long lTime = 0;

	virtual void OnPaint() override
	{
		CBlock::OnPaint(); 
	}
    
    virtual void WindowMessage(int nMsg, int nParam /*=0*/) override
    {
        if (nMsg == WmWillShow)
        {
            SetTimer(10);
        }
        if (nMsg == WmWillHide)
        {
            KillTimer();
        }
        CWnd::WindowMessage(nMsg, nParam);
    }

    virtual void OnTimer() override
    {
        if (!appData.GetConnected())
            return;
        
        enum TLast {Z, L, H, LH};
        /*
        EVERY(100)
        {
            BIOS::LCD::Printf(0, 0, RGB565(ffffff), RGB565(000000), "%d-%d---", streamerTotalLow, streamerTotalHigh);
        }*/
        static uint32_t lastHighs = 0;
        static uint32_t lastLows = 0;
        uint32_t nowHighs = streamerTotalHigh;
        uint32_t nowLows = streamerTotalLow;
        int current = framerGet(); //(int)(nowHighs-lastHighs) > (int)(nowLows-lastLows);
        lastHighs = nowHighs;
        lastLows = nowLows;

        static int x = m_rcClient.right-2;
        static enum TLast last = L;
        //static int counter = 0;
        //static int highs = 0;
        if (last != LH)
        {
            if (current == 0 && last == H)
                last = LH;
            if (current == 1 && last == L)
                last = LH;
        }
        /*
        if (current)
            highs++;
        if (counter++ < 10) // malo byt 80
            return;
         */
        //counter = 0;
        
        uint16_t pixBuf[40];
        for (int i=0; i<40; i++)
            pixBuf[i] = RGB565(404040);
        
        if (last == LH)
        {
            int c = false ? RGB565(b0b0b0) : RGB565(ffffff);
            for (int i=5;i<35; i++)
                pixBuf[i] = c;
        } else if (last == L)
            pixBuf[5] = RGB565(ffffff);
        else if (last == H)
            pixBuf[34] = RGB565(ffffff);
        
        BIOS::LCD::BufferBegin(CRect(x, m_rcClient.top+1, x+1, m_rcClient.bottom-1));
        BIOS::LCD::BufferWrite(pixBuf, COUNT(pixBuf));
        if (++x >= m_rcClient.right-1)
            x = m_rcClient.left+1;

        memset(pixBuf, 0, sizeof(pixBuf));
        BIOS::LCD::BufferBegin(CRect(x, m_rcClient.top+1, x+1, m_rcClient.bottom-1));
        BIOS::LCD::BufferWrite(pixBuf, COUNT(pixBuf));

        last = current == 0 ? L : H;
        //highs = 0;
    }

	
	virtual void OnKey(int key) override
	{
		if (key == BIOS::KEY::Left)
		{
			return;
		}
		if (key == BIOS::KEY::Right)
		{
			return;
		}
		CBlock::OnKey(key);
	}

};
