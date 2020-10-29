int main( int argc, char** argv );

bool memError = false;

class CApplication : public CWnd
{
	uint32_t mSetAddress {0x1FFFF000};
	uint32_t mAddress {0x08000000};
	uint32_t mLastGoodAddress {0x08000000};
	uint16_t mBuffer[16*8];
	int mPos{9};
	
public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
		/*
		enum EInterruptVector {
		  IStackTop, IReset, INMIException, IHardFaultException, IMemManageException,
		  IBusFaultException, IUsageFaultException, _Dummy1, _Dummy2,
		  _Dummy3, _Dummy4, ISVC, IDebugMonitor, _Dummy5, IPendSVC,
		  ISysTick, IWWDG_IRQ, IPVD_IRQ, ITAMPER_IRQ, IRTC_IRQ, IFLASH_IRQ,
		  IRCC_IRQ, IEXTI0_IRQ, IEXTI1_IRQ, IEXTI2_IRQ, IEXTI3_IRQ, IEXTI4_IRQ,
		  IDMA1_Channel1_IRQ, IDMA1_Channel2_IRQ, IDMA1_Channel3_IRQ,
		  IDMA1_Channel4_IRQ, IDMA1_Channel5_IRQ, IDMA1_Channel6_IRQ,
		  IDMA1_Channel7_IRQ, IADC1_2_IRQ, IUSB_HP_CAN_TX_IRQ,
		  IUSB_LP_CAN_RX0_IRQ, ICAN_RX1_IRQ, ICAN_SCE_IRQ, IEXTI9_5_IRQ,
		  ITIM1_BRK_IRQ, ITIM1_UP_IRQ, ITIM1_TRG_COM_IRQ, ITIM1_CC_IRQ,
		  ITIM2_IRQ, ITIM3_IRQ, ITIM4_IRQ, II2C1_EV_IRQ, II2C1_ER_IRQ,
		  II2C2_EV_IRQ, II2C2_ER_IRQ, ISPI1_IRQ, ISPI2_IRQ, IUSART1_IRQ,
		  IUSART2_IRQ, IUSART3_IRQ, IEXTI15_10_IRQ, IRTCAlarm_IRQ,
		  IUSBWakeUp_IRQ, ITIM8_BRK_IRQ, ITIM8_UP_IRQ, ITIM8_TRG_COM_IRQ,
		  ITIM8_CC_IRQ, IADC3_IRQ, IFSMC_IRQ, ISDIO_IRQ, ITIM5_IRQ,
		  ISPI3_IRQ, IUART4_IRQ, IUART5_IRQ, ITIM6_IRQ, ITIM7_IRQ,
		  IDMA2_Channel1_IRQ, IDMA2_Channel2_IRQ, IDMA2_Channel3_IRQ,
		  IDMA2_Channel4_5_IRQ };
*/
		BIOS::OS::SetInterruptVector(BIOS::OS::EInterruptVector::IHardFaultException, [](){BIOS::DBG::Print("hard"); memError = true;});
		BIOS::OS::SetInterruptVector(BIOS::OS::EInterruptVector::IMemManageException, [](){BIOS::DBG::Print("mem"); memError = true;});
		BIOS::OS::SetInterruptVector(BIOS::OS::EInterruptVector::IBusFaultException, [](){BIOS::DBG::Print("bus");memError = true;});
		BIOS::OS::SetInterruptVector(BIOS::OS::EInterruptVector::IUsageFaultException, [](){BIOS::DBG::Print("use");memError = true;});

        CWnd::Create(pszId, dwFlags, rc, pParent);

		/*
        mSeries.Init(mSeriesData, COUNT(mSeriesData));
        mSeries.SetSize(COUNT(mSeriesData));
        
        CRect rcGraph(10+4+40, 16+20+20-8, BIOS::LCD::Width-10-4, BIOS::LCD::Height-20-8-16+4-8);

        mWndGraph.Create("graph", CWnd::WsVisible, rcGraph, this);
        for (int i=0; i<mSeries.GetSize(); i++)
            mWndGraph.AddSeries(&mSeries[i]);

        CRect rcXAxis(10+4+40, BIOS::LCD::Height-20-8-16+4-8, BIOS::LCD::Width-10-4, BIOS::LCD::Height-20-8+4-8);
        mXAxis.Create("xaxis", CWnd::WsVisible, rcXAxis, this);

        CRect rcYAxis(10+4, 16+20+20-8, 10+4+40, BIOS::LCD::Height-20-8-16+4-8);
        mYAxis.Create("yaxis", CWnd::WsVisible, rcYAxis, this);

		mLog.Open();
        SetTimer(100);
		 */
		
		SetTimer(100);
    }
	
	void OnKey(int key) override
	{
		if (key == BIOS::KEY::EKey::Down)
		{
			if (mPos < 8)
			{
				mSetAddress -= 0x10000000 >> (mPos*4);
				ShowTitle();
			}
			if (mPos == 9)
			{
				mAddress += 8;
				Redraw();
			}
		}
		if (key == BIOS::KEY::EKey::Up)
		{
			if (mPos < 8)
			{
				mSetAddress += 0x10000000 >> (mPos*4);
				ShowTitle();
			}
			if (mPos == 9)
			{
				mAddress -= 8;
				Redraw();
			}
		}
		if (key == BIOS::KEY::EKey::Left && mPos > 0)
		{
			mPos--;
			ShowTitle();
		}
		if (key == BIOS::KEY::EKey::Right && mPos < 9)
		{
			mPos++;
			ShowTitle();
		}
		if (key == BIOS::KEY::EKey::Enter && mPos == 8)
		{
			mAddress = mSetAddress;
			Redraw();
		}
	}
	
    void OnPaint() override
    {
        //GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
		BIOS::LCD::Bar(m_rcClient, RGB565(404040));
        
        CRect rc1(m_rcClient);
        rc1.bottom = 14;
        GUI::Background(rc1, RGB565(4040b0), RGB565(404040));

        BIOS::LCD::Print(8, 0, RGB565(ffffff), RGBTRANS, "MemView");
		ShowPage(mAddress);
		ShowTitle();
    }
	
	void ShowTitle()
	{
		CRect rc1(m_rcClient);
		rc1.left = 80;
		rc1.bottom = 14;
		GUI::Background(rc1, RGB565(4040b0), RGB565(404040));

		int x = rc1.left;
		for (int i=0; i<8; i++)
		{
			if (mPos == i)
				x += BIOS::LCD::Printf(x, 0, RGBTRANS, RGB565(ffffff), "%x", (mSetAddress >> (28-i*4)) & 0xf);
			else
				x += BIOS::LCD::Printf(x, 0, RGB565(ffffff), RGBTRANS, "%x", (mSetAddress >> (28-i*4)) & 0xf);
		}
		
		x += 8;
		if (mPos == 8)
			x += BIOS::LCD::Printf(x, 0, RGBTRANS, RGB565(ffffff), "GO");
		else
			x += BIOS::LCD::Printf(x, 0, RGB565(ffffff), RGBTRANS, "GO");

		x += 8;
		if (mPos == 9)
			x += BIOS::LCD::Printf(x, 0, RGBTRANS, RGB565(ffffff), "MOVE");
		else
			x += BIOS::LCD::Printf(x, 0, RGB565(ffffff), RGBTRANS, "MOVE");

	}
	
    void OnTimer() override
    {
		ShowPage(mAddress, true);
    }
	
	void Redraw()
	{
		ShowPage(mAddress);
	}
	
	void ReadMemory(uint32_t address, uint8_t* data)
	{
		//if (address < 0x8)
		for (int i=0; i<8; i++)
		{
#ifndef __APPLE__
			data[i] = ((uint8_t*)address)[i];
			if (memError)
			{
				BIOS::DBG::Print("<Memory rd error!>");
				return;
			}
#else
			//data[i] = rand();
			data[i] = *((uint8_t*)main + ((address + i) & 0xffffff));
#endif
		}
	}
	
	void ShowPage(uint32_t address, bool showChanges = false)
	{
		uint32_t lastGood = address;
		
		int local = 0;
		bool changes[8] = {false};
		for (int y=16; y<=BIOS::LCD::Height-14; y+=14)
		{
			int x = 0;
			if (showChanges)
				x = 80;
			else
			{
				x += BIOS::LCD::Printf(x, y, RGB565(b0b0b0), RGB565(404040), "%08x:", address);
				x += 8;
			}
			
			uint8_t data[8];
			ReadMemory(address, data);
			
			for (int i=0; i<8; i++)
			{
				if (showChanges)
					changes[i] = data[i] != mBuffer[local+i];
				mBuffer[local+i] = data[i];
			}
				
			if (memError)
			{
				BIOS::LCD::Printf(x, y, RGB565(ff0000), RGB565(404040), "%08x:", address);
				memError = false;
				mAddress = lastGood;
				return;
			}
			//memcpy(mBuffer + local, data, 8);

			//x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGB565(404040), "%02x %02x %02x %02x  %02x %02x %02x %02x  ", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);

			for (int i=0; i<8; i++)
			{
				if (showChanges && !changes[i])
					continue;
				
				int color = showChanges ? RGB565(ffff00) : RGB565(ffffff);
					
				if (i>=4)
					BIOS::LCD::Printf(x + i*24+8, y, color, RGB565(404040), "%02x", data[i]);
				else
					BIOS::LCD::Printf(x + i*24, y, color, RGB565(404040), "%02x", data[i]);
				
				BIOS::LCD::Print(x + 220 + i*8, y, color, RGB565(404040), data[i]);
			}
			
			address += 8;
			local += 8;
		}
		mLastGoodAddress = lastGood;
	}
};
