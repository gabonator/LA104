uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];
CBufferedReader mCommonReader;
CBufferedWriter mCommonWriter;


CRfid Rfid;

class CScanLayout : public CWnd
{
public:
    void Create(const char* pszId, int dwFlags, const CRect& rc, CWnd* pParent)
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
        SetTimer(50);
    }

    virtual void OnPaint() override
    {
        using namespace Layout;

        CRect rcWindow(m_rcClient);
        rcWindow.Deflate(16, 16, 16, 16);
        Render r(rcWindow);
        r << Window("Scanning...")
            << "Sak  Device type     UID" << NewLine()
            << HLine(RGB565(808080));
        CONSOLE::window = r.GetRectangle();
        CONSOLE::cursor = CONSOLE::window.TopLeft();
        CONSOLE::colorFront = RGB565(ffffff);
        CONSOLE::colorBack = RGB565(404040);
    }
    
    virtual void OnTimer() override
    {
        CRfidCard card = Rfid.GetNewCard();
        if (card)
        {
            BIOS::SYS::Beep(200);
            char typeAligned[40];
            strcpy(typeAligned, card.GetType());
            for (int i=strlen(typeAligned); i<16; i++)
                typeAligned[i] = ' ';
            typeAligned[16] = 0;
            CONSOLE::Print("%02x   %s%s\n", card.GetSak(), typeAligned, card.GetIdAsString());
        }
    }
};

class CReadLayout : public CWnd
{
    CRfidCard mCard;
    CRfidKey mKey;
    int mBlockNum{0};
    CRfidBlock mBlocks[64];
    
public:
    void Create(const char* pszId, int dwFlags, const CRect& rc, CWnd* pParent)
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
        SetTimer(50);
    }

    virtual void OnPaint() override
    {
        using namespace Layout;

        CRect rcWindow(m_rcClient);
        rcWindow.Deflate(4, 16, 4, 16);
        Render r(rcWindow);
        r << Window("Reading...")
        << Color(RGB565(ffffff));
        CONSOLE::window = r.GetRectangle();
        CONSOLE::cursor = CONSOLE::window.TopLeft();
        CONSOLE::colorFront = RGB565(ffffff);
        CONSOLE::colorBack = RGB565(404040);
        
        uint8_t key[CRfidKey::Size] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
        mKey.Set(key);
    }
    
    virtual void OnTimer() override
    {
        if (mCard)
        {
            CRfidBlock& block = mBlocks[mBlockNum];
            block = mCard.ReadBlock(mBlockNum, mKey);
            if (block)
            {
                char strBlock[34];
                block.ToString(strBlock);
                CONSOLE::Print("%02d: %s\n", mBlockNum, strBlock);
                mBlockNum++;
            } else
            {
                CONSOLE::Print("%02d: ", mBlockNum);
                CONSOLE::Color(RGB565(ff0000));
                CONSOLE::Print("%s\n", mCard.GetLastError());
                CONSOLE::Color(RGB565(ffffff));

                if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
                {
                    ERROR << "RestartFail";
                    mCard = CRfidCard();
                }
                    
                //mCard = CRfidCard();
                mBlockNum++;
                //BIOS::SYS::DelayMs(2000);
            }
            if (mBlockNum >= 64)
            {
                for (int i=0; i<64; i++)
                    if (!mBlocks[i])
                    {
                        mCard = CRfidCard();
                        return;
                    }
                
                char name[16];
                sprintf(name, "rfid%04x.txt", mCard.GetId() >> 16);
                
                Save(name);
                CONSOLE::Color(RGB565(ffff00));
                CONSOLE::Print("Dump written to: '%s'\n", name);
                CONSOLE::Color(RGB565(ffffff));

                mCard = CRfidCard();
            }
        } else
        {
            mCard = Rfid.GetNewCard();
            if (mCard)
            {
                CONSOLE::Print("Found card: %s\n", mCard.GetIdAsString());
                //if (mfrc522.PICC_ReadCardSerial())
               //     CONSOLE::Print("ReadOk\n");

                mBlockNum = 0;
            }
        }
    }
    
    void Save(char* name)
    {
        mCommonWriter.Open(name);
        for (int i=0; i<64; i++)
        {
            char line[64];
            mBlocks[i].ToString(line);
            mCommonWriter << line << "\n";
        }
        mCommonWriter.Close();
    }
};

class CWriteLayout : public CWnd
{
    CRfidCard mCard;
    CRfidKey mKeyA;
    CRfidKey mKeyB;
    int mBlockNum{0};
    CRfidBlock mBlocks[64];
    CFileDialog mFile;
    bool mEntered{false};
    bool mLoaded{false};
    
public:
    void Create(const char* pszId, int dwFlags, const CRect& rc, CWnd* pParent)
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
        SetTimer(50);
        
        uint8_t key[CRfidKey::Size] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
        mKeyA.Set(key);
        mKeyB.Set(key);
    }

    virtual void OnPaint() override
    {
        using namespace Layout;

        if (mEntered)
        {
            BIOS::DBG::Print("Does not work, will brick your card!");
            BIOS::SYS::DelayMs(2000);

            mEntered = false;
            CFileFilterSuffix filter(".TXT");
            if (mFile.ModalShow(this, "Load log", &filter))
            {
                mLoaded = Load(mFile.GetFilename());
                mBlockNum = 5;
            }
            GetParent()->SetFocus();
        }

        CRect rcWindow(m_rcClient);
        rcWindow.Deflate(4, 16, 4, 16);
        Render r(rcWindow);
        r << Window("Writing...")
        << Color(RGB565(ffffff));
        CONSOLE::window = r.GetRectangle();
        CONSOLE::cursor = CONSOLE::window.TopLeft();
        CONSOLE::colorFront = RGB565(ffffff);
        CONSOLE::colorBack = RGB565(404040);
                
        Dump();
    }
    
    void Dump()
    {
        //for (int i=0; i<64; i++)
        for (int i=0; i<16; i++)
        {
            char strBlock[34];
            mBlocks[i].ToString(strBlock);
            CONSOLE::Print("%02d: %s\n", i, strBlock);
        }
    }
    
    bool Load(char* name)
    {
        auto hexval = [](char ch)
        {
            if ( ch >= '0' && ch <= '9' )
                return ch - '0';
            if ( ch >= 'A' && ch <= 'F' )
                return ch - 'A' + 10;
            if ( ch >= 'a' && ch <= 'f' )
                return ch - 'a' + 10;
            _ASSERT(0);
            return -1;
        };

        if (!mCommonReader.Open(name))
            return false;
        
        for (int i=0; i<64; i++)
        {
            char line[64];
            mCommonReader >> line;
            if (strlen(line) != 32)
            {
                _ASSERT(0);
                return false;
            }
            
            uint8_t data[16];
            for (int j=0; j<16; j++)
                data[j] = (hexval(line[j*2]) << 4) | hexval(line[j*2+1]);
            
            mBlocks[i] = CRfidBlock(data);
        }
        mCommonReader.Close();
        return true;
    }

    
    virtual void WindowMessage(int nMsg, int nParam) override
    {
        if (nMsg == WmWillShow)
        {
            mEntered = true;
            mCard = CRfidCard();
            mLoaded = false;
        }
        CWnd::WindowMessage(nMsg, nParam);
    }

    virtual void OnTimer() override
    {
        if (!mLoaded)
            return;
        
        if (mCard)
        {
            if (mCard.WriteBlock(mBlockNum, mKeyA, mKeyB, mBlocks[mBlockNum]))
            //if (mCard.WriteMagic(mBlockNum, mBlocks[mBlockNum]))
            {
                CONSOLE::Print("%02d: OK, ", mBlockNum);
                mBlockNum++;
            } else
            {
                CONSOLE::Print("%02d: failed!\n", mBlockNum);
                mLoaded = false;
            }
            if (mBlockNum >= 8)
            {
                CONSOLE::Color(RGB565(ffff00));
                CONSOLE::Print("Done!\n");
                CONSOLE::Color(RGB565(ffffff));
            }
        } else
        {
            mCard = Rfid.GetNewCard();
            if (mCard)
            {
                CONSOLE::Print("Found card: %s\n", mCard.GetIdAsString());
            }
        }
    }
};

class CApplication : public CWnd
{
    CMenuMain mMenu;
    CScanLayout mScanLayout;
    CReadLayout mReadLayout;
    CWriteLayout mWriteLayout;

public:
	void Create()
	{
		CWnd::Create("Application", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
		
        if (!Rfid.Init())
        {
            BIOS::DBG::Print("MRFC522 not found!");
            BIOS::SYS::DelayMs(2000);
        }
		const int padding = 20;

        mMenu.Create("MainMenu", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, 14), this);
        
        CRect rcContent(0, 14, BIOS::LCD::Width, BIOS::LCD::Height-14);
        mScanLayout.Create("ScanLayout", CWnd::WsVisible | CWnd::WmTick, rcContent, this);
        mReadLayout.Create("ReadLayout", CWnd::WmTick, rcContent, this);
        mWriteLayout.Create("WriteLayout", CWnd::WmTick, rcContent, this);

        mMenu.SetFocus();
        OnMessage(&mMenu, 0, 1);
        
	}
	
	void Destroy()
	{
	}
    
	virtual void OnTimer() override
	{
	}
    
	virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override
	{
        if (pSender == &mMenu)
        {
            mScanLayout.ShowWindow(data == 1);
            mReadLayout.ShowWindow(data == 2);
            mWriteLayout.ShowWindow(data == 3);
            Invalidate();
        }
	}
	
private:
	virtual void OnPaint() override
	{
		GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
		
		CRect rc1(m_rcClient);
		rc1.bottom = 14;
		GUI::Background(rc1, RGB565(4040b0), RGB565(404040));
		BIOS::LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "RFID tool");
        
        ShowStatus();
	}

public:
    void ShowStatus(const char* message = nullptr)
    {
        CRect rcClient(m_rcClient);
        rcClient.bottom = BIOS::LCD::Height;
        rcClient.top = BIOS::LCD::Height-14;
        
        BIOS::LCD::Bar(rcClient, RGB565(404040));
        
        using namespace Layout;
        Render r(rcClient);
        r << Padding(8, 0, 4, 0);
        Color h(RGB565(ffffff));
        Color l(RGB565(b0b0b0));
        r << Rfid.GetInfo();
    }
};
