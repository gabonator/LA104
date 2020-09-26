#include "24LC256.h"
#include "layout.h"

#ifdef __APPLE__
int main( int argc, char** argv );

class CMemory
{
public:
    void Init()
    {
    }
    
    void Deinit()
    {
    }
    
    bool Check()
    {
        return true;
    }
    
    bool Read(int address, uint8_t* data, int length)
    {
        for (int i=0; i<length; i++)
            data[i] = *((uint8_t*)main + ((address + i) & 0xffffff));
        return true;
    }
    
    bool Write(int address, uint8_t data)
    {
        return false;
    }
};
#else
class CMemory
{
    E24LC256 eeprom{0x50};

public:
    void Init()
    {
        eeprom.begin();
    }
    
    void Deinit()
    {
        eeprom.end();
    }
    
    bool Check()
    {
        return eeprom.detect();
    }
    
    bool Read(int address, uint8_t* data, int length)
    {
//        return eeprom.readPage(address, length, data) == 8;
        return eeprom.readPage(address, length, data);
//	return true;
    }
    
    bool Write(int address, uint8_t data)
    {
        if (!eeprom.writeByte(address, data))
            return false;

        BIOS::SYS::DelayMs(100);
        //eeprom.ack_polling();
        return true;
    }
    
};
#endif

class CEditValue : public CWnd
{
    int mX{0}, mY{0};
    int mValue{'g'};
    char mBuffer[8];
    uint32_t mAddress{0};
    bool mRedraw{false};
    
public:
    void SetValue(int v, uint32_t address)
    {
        mAddress = address;
        mValue = v;
        mY = 3;
        mX = 0;
    }
    
    int GetValue()
    {
        return mValue;
    }

    char* bin(int index)
    {
        sprintf(mBuffer, "%d", (mValue >> index) & 1);
        return mBuffer;
    }

    char* hex(int index)
    {
        sprintf(mBuffer, "%x", (mValue >> index*4) & 15);
        return mBuffer;
    }

    char* dec(int index)
    {
        switch (index)
        {
            case 2:
                sprintf(mBuffer, "%d", (mValue / 100) % 10);
                break;
            case 1:
                sprintf(mBuffer, "%d", (mValue / 10) % 10);
                break;
            case 0:
                sprintf(mBuffer, "%d", mValue % 10);
                break;
            default:
                _ASSERT(0);
        }
        return mBuffer;
    }
    
    char* asc(int index)
    {
        mBuffer[0] = mValue;
        mBuffer[1] = 0;
        return mBuffer;
    }
    
    void WindowMessage(int nMsg, int nParam /*=0*/) override
    {
        if (nMsg == CWnd::WmWillShow)
            mRedraw = true;
        CWnd::WindowMessage(nMsg, nParam);
    }

    void OnPaint() override
    {
        using namespace Layout;
        Color def(RGB565(b0b0b0));
        Color hig(RGB565(ffffff));
        
        #define S(y, x) Select(mX == x && mY == y)
        
        char heading[64];
        sprintf(heading, "Edit value @ %08x", mAddress);
        Render r(m_rcClient);
        // window prilis blika, iba vnutro
        r << Window(heading, mRedraw)
            << def << "Binary: " << hig
                << S(0, 0) << bin(7) << " " << S(0, 1) << bin(6) << " "
                << S(0, 2) << bin(5) << " " << S(0, 3) << bin(4) << " "
                << S(0, 4) << bin(3) << " " << S(0, 5) << bin(2) << " "
                << S(0, 6) << bin(1) << " " << S(0, 7) << bin(0) << NewLine()
            << def << "Hexadecimal: " << hig << S(1, 0) << hex(1) << " " << S(1, 1) << hex(0) << NewLine()
            << def << "Decimal: " << hig << S(2, 0) << dec(2) << " " << S(2, 1) << dec(1) << " " << S(2, 2) << dec(0) << NewLine()
            << def << "Ascii: " << hig << S(3, 0) << asc(0) << NewLine()
            << def << NewLine()
            << "    " << S(4, 0) << Button("Save") << "  " << S(4, 1) << Button("Cancel");
        mRedraw = false;
    }
    
    void OnKey(int key) override
    {
        if (key == BIOS::KEY::EKey::Down)
            mY++;
        if (key == BIOS::KEY::EKey::Up)
            mY--;
        if (key == BIOS::KEY::EKey::Right)
            mX++;
        if (key == BIOS::KEY::EKey::Left)
            mX--;

        if (mY == 3)
        {
            if (key == BIOS::KEY::EKey::Right)
                mValue++;
            if (key == BIOS::KEY::EKey::Left)
                mValue--;
        }
        
        if (key == BIOS::KEY::EKey::Enter)
        {
            switch (mY)
            {
                case 0:
                    mValue ^= 1<<(7-mX);
                    break;
                case 1:
                    switch (mX)
                    {
                        case 0: mValue += 0x10; break;
                        case 1: mValue += 0x01; break;
                    }
                    break;
                case 2:
                    switch (mX)
                    {
                        case 0: mValue += 100; break; // toto blbost
                        case 1: mValue += 10; break;
                        case 2: mValue += 1; break;
                    }
                    break;
                case 4:
                    switch (mX)
                    {
                        case 0: SendMessage(GetParent(), 0, (uintptr_t)"save"); break;
                        case 1: SendMessage(GetParent(), 0, (uintptr_t)"cancel"); break;
                    }
                    break;
            }
        }
        mValue &= 255;
        mY = max(0, min(mY, 4));
        const int maxElements[] = {8, 2, 3, 1, 2};
        mX = max(0, min(mX, maxElements[mY]-1));
        Invalidate();
    }
};

class CApplication : public CWnd
{
	uint32_t mSetAddress {0x00000000};
	uint32_t mAddress {0x00000000};
	uint32_t mLastGoodAddress {0x00000000};
	uint16_t mBuffer[16*8];
	int mPos{9};
    bool mEditMode{false};
    bool mChangeMode{false};
    int mEditOfs{0};
    CMemory mMemory;
    CEditValue mEditValue;
    
public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        mMemory.Init();
        CWnd::Create(pszId, dwFlags, rc, pParent);
		SetTimer(100);
        
        const int EditWidth = 250;
        const int EditHeight = 120;
        CRect rcEditValue((BIOS::LCD::Width-EditWidth)/2, (BIOS::LCD::Height-EditHeight)/2, (BIOS::LCD::Height+EditWidth)/2, (BIOS::LCD::Height+EditHeight)/2);
        
        mEditValue.Create("Edit value", CWnd::WsHidden, rcEditValue, this);
    }
    
    void Destroy()
    {
        mMemory.Deinit();
    }
	
    virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override
    {
        if (strcmp((char*)data, "save") == 0)
        {
            mEditValue.ShowWindow(false);
            Invalidate();
            SetFocus();
            SetCurrentValue(mEditValue.GetValue());
        }
        if (strcmp((char*)data, "cancel") == 0)
        {
            mEditValue.ShowWindow(false);
            Invalidate();
            SetFocus();
        }
    }
	void OnKey(int key) override
	{
        if (mEditMode)
        {
            if (key == BIOS::KEY::EKey::Enter)
            {
                mChangeMode = true;

                mEditValue.SetValue(GetCurrentValue(), GetCurrentAddress());
                mEditValue.ShowWindow(true);
                mEditValue.SetFocus();
                mEditValue.Invalidate();
                return;
            }
            
            if (key == BIOS::KEY::EKey::Down)
                mEditOfs += 8;
            if (key == BIOS::KEY::EKey::Up)
                mEditOfs -= 8;
            if (key == BIOS::KEY::EKey::Right)
                mEditOfs ++;
            if (key == BIOS::KEY::EKey::Left)
                mEditOfs --;
            
            if (mEditOfs < 0)
            {
                mEditOfs = 0;
                mEditMode = false;
                ShowTitle();
            }
            Redraw();
            return;
        }
        
		if (key == BIOS::KEY::EKey::Down)
		{
			if (mPos < 8)
			{
				mSetAddress -= 0x10000000 >> (mPos*4);
//                if ((int32_t)mSetAddress < 0)
//                    mSetAddress = 0;
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
		if (key == BIOS::KEY::EKey::Right && mPos < 10)
		{
			mPos++;
			ShowTitle();
		}
		if (key == BIOS::KEY::EKey::Enter && mPos == 8)
		{
			mAddress = mSetAddress;
			Redraw();
		}

        if (key == BIOS::KEY::EKey::Enter && mPos == 10)
        {
            mEditMode = true;
            ShowTitle();
            Redraw();
        }
    }
	
    void OnPaint() override
    {
		BIOS::LCD::Bar(m_rcClient, RGB565(404040));
        
        CRect rc1(m_rcClient);
        rc1.bottom = 14;
        GUI::Background(rc1, RGB565(4040b0), RGB565(404040));

        BIOS::LCD::Print(8, 0, RGB565(ffffff), RGBTRANS, "MemView I2C");
		ShowPage(mAddress);
		ShowTitle();
    }
	
	void ShowTitle()
	{
		CRect rc1(m_rcClient);
		rc1.left = 104;
		rc1.bottom = 14;
		GUI::Background(rc1, RGB565(4040b0), RGB565(404040));

		int x = rc1.left;

		if (!mMemory.Check())
		{
			x += BIOS::LCD::Print(x, 0, RGBTRANS, RGB565(ffffff), "Memory not attached");
			return;
		}

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

        x += 8;
        if (mPos == 10 && !mEditMode)
            x += BIOS::LCD::Printf(x, 0, RGBTRANS, RGB565(ffffff), "EDIT");
        else
            x += BIOS::LCD::Printf(x, 0, RGB565(ffffff), RGBTRANS, "EDIT");
	}
	
    void ShowMessage(const char* message)
    {
        CRect rc1(m_rcClient);
        rc1.left = 104;
        rc1.bottom = 14;
        GUI::Background(rc1, RGB565(4040b0), RGB565(404040));
        int x = rc1.left;
        BIOS::LCD::Print(x, 0, RGBTRANS, RGB565(ffff00), message);
    }

    void OnTimer() override
    {
		ShowPage(mAddress, true);
    }
	
	void Redraw()
	{
		ShowPage(mAddress);
	}
	
	bool ReadMemory(uint32_t address, uint8_t* data)
	{
		for (int i=0; i<8; i++)
			data[i] = 0xab;
        return mMemory.Read(address, data, 8);
	}
	
	void ShowPage(uint32_t address, bool showChanges = false) // TODO: async
	{
        const char* addressFormat = "%08x:"; // "%08x:"
        
		uint32_t lastGood = address;
        uint32_t baseAddress = address;
		
		int local = 0;
		bool changes[8] = {false};
		for (int y=16; y<=BIOS::LCD::Height-14; y+=14)
		{
			int x = 0;
			if (showChanges)
				x = 80;
			else
			{
				x += BIOS::LCD::Printf(x, y, RGB565(b0b0b0), RGB565(404040), addressFormat, address);
				x += 8;
			}
			
			uint8_t data[8];
			bool ok = ReadMemory(address, data);

            if (!ok)
            {
                x = 0;
                x += BIOS::LCD::Printf(x, y, RGB565(ff0000), RGB565(404040), addressFormat, address);
                mAddress = lastGood;
                ShowTitle(); // TODO: recover from failure
                return;
            }

			for (int i=0; i<8; i++)
			{
				if (showChanges)
					changes[i] = data[i] != mBuffer[local+i];
				mBuffer[local+i] = data[i];
			}
				
			const int k = 20; // 24
			const int s = 4; // 8
            const int t = 170; //180; // 220
			for (int i=0; i<8; i++)
			{
				if (showChanges && !changes[i])
					continue;
				
				int color = showChanges ? RGB565(ffff00) : RGB565(ffffff);
                int back = RGB565(404040);
                
                if (mEditMode)
                {
                    if (mEditOfs == address - baseAddress + i)
                    {
                        back = RGB565(ffffff);
                        color = RGB565(000000);
                    }
                }
                
				if (i>=4)
					BIOS::LCD::Printf(x + i*k+s, y, color, back, "%02x", data[i]);
				else
					BIOS::LCD::Printf(x + i*k, y, color, back, "%02x", data[i]);
				
				BIOS::LCD::Print(x + t + i*8, y, color, back, data[i]);
			}
			
			address += 8;
			local += 8;
		}
		mLastGoodAddress = lastGood;
	}
    
    uint32_t GetCurrentAddress()
    {
        uint32_t addr = mAddress + mEditOfs;
        return addr;
    }
    uint8_t GetCurrentValue()
    {
        uint8_t data = 0;
        mMemory.Read(GetCurrentAddress(), &data, 1);
        return data;
    }
    
    void SetCurrentValue(uint8_t v)
    {
        if (!mMemory.Write(GetCurrentAddress(), v))
            ShowMessage("Write failed!");
    }
};
