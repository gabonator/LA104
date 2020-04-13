#include <library.h>
#include <algorithm>

#include "assert.h"
#include "pressure.h"

using namespace BIOS;
#include "vfd/debug.h"
#include "vfd/packet.h"
#include "vfd/serial.h"
#include "vfd/invt.h"
#include "vfd/invtgd10.h"

#include "vfdAttributes.h"
#include "vfdManager.h"

static constexpr char mAnimPattern[] = "|/-\\|/-\\";

class CApplication : public CWnd
{
	CPressureGraph mGraph;
#ifdef __APPLE__
	CVfdManager_Fake mManager;
#else
	CVfdManager mManager;
#endif
	
	int mAnimPhase{0};
	bool mWasFault{false};
	bool mHasValidData{false};
	int mErrorCounter{0};
	float mLastDrawnFrequency{-1.f};
	CVfdAttributes::EOperation mLastDrawnOperation{CVfdAttributes::EOperation::Unknown};

public:
    void Create(const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent)
    {
        CWnd::Create("application", CWnd::WsVisible, rc, pParent);
		mGraph.Create(CRect(rc.left+10, rc.top + 50 + 4, rc.right-10, rc.bottom-70 + 4), this);
		mManager.setup();
        SetTimer(100);
    }
	
	void Destroy()
	{
		CWnd::Destroy();
	}

    void OnPaint() override
    {
        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
        
        CRect rc1(m_rcClient);
        rc1.bottom = 14;
        GUI::Background(rc1, RGB565(4040b0), RGB565(404040));
        BIOS::LCD::Print(8, 0, RGB565(ffffff), RGBTRANS, "Invt VFD visualizer, valky.eu, 2020");

		CRect rc2(m_rcClient);
		rc2.top = rc2.bottom-14;
		GUI::Background(rc2, RGB565(404040), RGB565(202020));
		BIOS::LCD::Print(8, rc2.top, RGB565(808080), RGBTRANS, "VFD Not connected");
		
		OnUpdate();
	}
     
    virtual void WindowMessage(int nMsg, int nParam = 0) override
    {
        if (nMsg == CWnd::WmTick)
			mManager.loop();
		
        CWnd::WindowMessage(nMsg, nParam);
	}

	void OnTimer() override
	{
		CVfdAttributes& mAttributes = mManager.GetAttributes();

		if (mManager.IsConnect())
			OnConnect();

		if (mManager.IsComm())
			OnComm();

		if (mManager.IsCommError())
		{
			OnCommError();
			if (mHasValidData && mErrorCounter++ > 10)
			{
				BIOS::SYS::Beep(1000);
				mHasValidData = false;
				OnUpdate();
			}
		}

		if (mManager.IsData())
		{
			mHasValidData = true;
			mErrorCounter = 0;
			mGraph.Update(mAttributes.GetPidTarget()*100, mAttributes.GetPidValue()*100, ((int)mAttributes.GetFrequency())*2);
			OnUpdate();
		}
	}
	
	int PrintBig(int x, int y, int c, char* text)
	{
		uint8_t* fontBase = (uint8_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::CharRom);
		int oldx = x;
		while (*text)
		{
			uint8_t* charBase = fontBase + (*text++) * 14;
			for (ui8 _y=0; _y<14; _y++)
			{
				ui8 col = *charBase++;
		
				for (ui8 _x=0; _x<8; _x++, col <<= 1)
					if ( col & 128 )
						BIOS::LCD::Bar(x + _x*3, y + _y*3, x + _x*3+3, y + _y*3+3, c);
			}
			x += 8*3;
		}
		return x - oldx;
	}
	
	void OnUpdate()
	{
		CVfdAttributes& mAttributes = mManager.GetAttributes();

		int y = m_rcClient.top + 14;
		int x = 8;
		int _x = x;
		
		if (!mHasValidData)
		{
			//mLastDrawnOperation = CVfdAttributes::Unknown;
			mLastDrawnFrequency = -1.f;
			mWasFault = true;
			const char* message = "No connection";
			GUI::Background(CRect(m_rcClient.left, y, m_rcClient.right, y + 13*3), RGB565(404040), RGB565(101010));
			BIOS::LCD::Printf(m_rcClient.CenterX() - strlen(message)*4, y+4+8+2, RGB565(ffffff), RGBTRANS, message, mAttributes.GetErrorCode());
			return;
		}
		
		if (mAttributes.GetError())
		{
			mLastDrawnFrequency = -1.f;
			mWasFault = true;
			GUI::Background(CRect(0, y, m_rcClient.CenterX() + 40, y + 13*3), RGB565(404040), RGB565(ff1010));
			BIOS::LCD::Printf(_x, y+6, RGB565(ffffff), RGBTRANS, "Error #%d:", mAttributes.GetErrorCode());
			BIOS::LCD::Printf(_x, y+22, RGB565(ffffff), RGBTRANS, mAttributes.GetError());
		} else
		{
			if (mWasFault)
			{
				GUI::Background(CRect(m_rcClient.left, y, m_rcClient.right, y + 13*3), RGB565(404040), RGB565(101010));
				mWasFault = false;
			}
			if (mLastDrawnFrequency != mAttributes.GetFrequency())
			{
				mLastDrawnFrequency = mAttributes.GetFrequency();
				_x = x + 16;
				char temp[8];
				sprintf(temp, "%.1f", mAttributes.GetFrequency());
				GUI::Background(CRect(_x, y, _x + 8*3*4, y + 12*3), RGB565(404040), RGB565(101010));
				PrintBig(_x, y, RGB565(fffffff), temp);
				_x += 4*3*8 + 8;
				_x += BIOS::LCD::Printf(_x, y + 22, RGB565(ffffff), RGBTRANS, "Hz");
				_x += BIOS::LCD::Printf(_x, y + 22, RGB565RGB(39, 101, 217), RGBTRANS, "\x7");
			}
		}
		
		if (mLastDrawnOperation != mAttributes.GetOperation())
		{
			mLastDrawnOperation = mAttributes.GetOperation();
			
			if (mAttributes.GetOperation() == CVfdAttributes::Fault)
			{
				BIOS::SYS::Beep(1000);
			}
			
			_x = m_rcClient.CenterX() + 40;
			switch (mAttributes.GetOperation())
			{
				case CVfdAttributes::Unknown:
					GUI::Background(CRect(_x, y, _x + 8*3*4, y + 13*3), RGB565(404040), RGB565(101010));
					break;
				case CVfdAttributes::ForwardRunning:
				case CVfdAttributes::ReverseRunning:
					GUI::Background(CRect(_x, y, _x + 8*3*4, y + 13*3), RGB565(405040), RGB565(10b010));
					_x += PrintBig(_x, y, RGB565(b0b0b0), (char*)"Run");
					break;
				case CVfdAttributes::Stop:
					GUI::Background(CRect(_x, y, _x + 8*3*4, y + 13*3), RGB565(404040), RGB565(101010));
					_x += PrintBig(_x, y, RGB565(b0b0b0), (char*)"Stop");
					break;
				case CVfdAttributes::Fault:
					mWasFault = true;
					GUI::Background(CRect(_x, y, m_rcClient.right, y + 13*3), RGB565(804040), RGB565(ff1010));
					_x += PrintBig(_x, y, RGB565(b0b0b0), (char*)"Fail");
					break;
				case CVfdAttributes::Off:
					GUI::Background(CRect(_x, y, _x + 8*3*4, y + 13*3), RGB565(404040), RGB565(101010));
					_x += PrintBig(_x, y, RGB565(b0b0b0), (char*)"Off");
					break;
			}
		}

		_x = x;
		y = m_rcClient.bottom - 65 + 4;
		GUI::Background(CRect(_x, y, m_rcClient.right, y+14), RGB565(404040), RGB565(101010));
		_x += BIOS::LCD::Printf(_x, y, RGB565(b0b0b0), RGBTRANS, "Bus: ", mAttributes.GetBusVoltage());
		_x += BIOS::LCD::Printf(_x, y, RGB565(ffffff), RGBTRANS, "%.1f", mAttributes.GetBusVoltage()) + 4;
		_x += BIOS::LCD::Printf(_x, y, RGB565(b0b0b0), RGBTRANS, "V");
		_x += BIOS::LCD::Printf(_x, y, RGB565(b0b0b0), RGBTRANS, ", Mode: ");
		_x += BIOS::LCD::Printf(_x, y, RGB565(ffffff), RGBTRANS, mAttributes.GetOperationString());
		y += 16;
		_x = x;
		GUI::Background(CRect(_x, y, m_rcClient.right, y+14), RGB565(404040), RGB565(101010));
		_x += BIOS::LCD::Printf(_x, y, RGB565(b0b0b0), RGBTRANS, "Motor: ");
		_x += BIOS::LCD::Printf(_x, y, RGB565(ffffff), RGBTRANS, "%.1f", mAttributes.GetVoltage()) + 4;
		_x += BIOS::LCD::Printf(_x, y, RGB565(b0b0b0), RGBTRANS, "V, ");
		_x += BIOS::LCD::Printf(_x, y, RGB565(ffffff), RGBTRANS, "%.1f", mAttributes.GetCurrent()) + 4;
		_x += BIOS::LCD::Printf(_x, y, RGB565(b0b0b0), RGBTRANS, "A, ");
		_x += BIOS::LCD::Printf(_x, y, RGB565(ffffff), RGBTRANS, "%d", mAttributes.GetSpeed()) + 4;
		_x += BIOS::LCD::Printf(_x, y, RGB565(b0b0b0), RGBTRANS, "rpm");
		y += 16;
		_x = x;
		GUI::Background(CRect(_x, y, m_rcClient.right, y+14), RGB565(404040), RGB565(101010));
		_x += BIOS::LCD::Printf(_x, y, RGB565(b0b0b0), RGBTRANS, "Pressure: ");
		_x += BIOS::LCD::Printf(_x, y, RGB565(ffff00), RGBTRANS, "\x7");
		_x += BIOS::LCD::Printf(_x, y, RGB565(ffffff), RGBTRANS, "%.2f", mAttributes.GetPidValue()) + 4;
		_x += BIOS::LCD::Printf(_x, y, RGB565(b0b0b0), RGBTRANS, "bar");
		_x += BIOS::LCD::Printf(_x, y, RGB565(b0b0b0), RGBTRANS, ", Target: ");
		_x += BIOS::LCD::Printf(_x, y, RGB565(00ff00), RGBTRANS, "\x7");
		_x += BIOS::LCD::Printf(_x, y, RGB565(ffffff), RGBTRANS, "%.2f", mAttributes.GetPidTarget()) + 4;
		_x += BIOS::LCD::Printf(_x, y, RGB565(b0b0b0), RGBTRANS, "bar");
	}
	
	void OnConnect()
	{
		CVfdAttributes& mAttributes = mManager.GetAttributes();

		CRect rc2(m_rcClient.left, m_rcClient.bottom-14, m_rcClient.right-16, m_rcClient.bottom);
		GUI::Background(rc2, RGB565(404040), RGB565(202020));
		
		const char* deviceId = mAttributes.GetIdentify();
		int x = 8;
		x += BIOS::LCD::Printf(x, rc2.top, RGB565(808080), RGBTRANS, "Connected: %s", deviceId);
	}
	
	void OnComm()
	{
		CRect rc2(m_rcClient.right-16, m_rcClient.bottom-14, m_rcClient.right-8, m_rcClient.bottom);
		GUI::Background(rc2, RGB565(404040), RGB565(202020));

		BIOS::LCD::Printf(BIOS::LCD::Width-16, BIOS::LCD::Height-14, RGB565(00ff00), RGBTRANS, "%c", mAnimPattern[mAnimPhase++&7]);
	}

	void OnCommError()
	{
		CRect rc2(m_rcClient.right-16, m_rcClient.bottom-14, m_rcClient.right-8, m_rcClient.bottom);
		GUI::Background(rc2, RGB565(404040), RGB565(202020));

		BIOS::LCD::Printf(BIOS::LCD::Width-16, BIOS::LCD::Height-14, RGB565(ff0000), RGBTRANS, "%c", mAnimPattern[mAnimPhase++&7]);
	}
};

CApplication app;

#ifndef __APPLE__
__attribute__((__section__(".entry")))
#endif
int _main(void)
{ 
    app.Create("", CWnd::WsVisible | CWnd::WsTick, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
    app.WindowMessage( CWnd::WmPaint );
    
    BIOS::KEY::EKey key;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
    {
        if (key != BIOS::KEY::None)
            app.WindowMessage(CWnd::WmKey, key);
        app.WindowMessage(CWnd::WmTick);
    }
    
    app.Destroy();
    return 0;
}
