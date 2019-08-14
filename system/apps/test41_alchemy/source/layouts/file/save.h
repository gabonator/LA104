class CLayoutFileSave : public CWnd
{
    CSignalView mSignal;
    
    CButton mSave;
    CButton mLoad;
    CButton mStartRecord;
    CButton mStopRecord;

public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
        mSignal.Create("Signal", CWnd::WsVisible | CWnd::WsNoActivate, CRect(0, BIOS::LCD::Height-33, BIOS::LCD::Width, BIOS::LCD::Height), this);
        
        mSave.Create("Save", CWnd::WsVisible, CRect(8, 40, 8+(5+1)*8, 40+16), this);
        mLoad.Create("Load", CWnd::WsVisible, CRect(8, 40+20, 8+(5+1)*8, 40+16+20), this);

        mStartRecord.Create("Start recording", CWnd::WsVisible, CRect(8, 40+40, 8+(30)*8, 40+40+16), this);
        mStopRecord.Create("Stop recording", CWnd::WsVisible, CRect(8, 40+60, 8+(30)*8, 40+60+16), this);
        // Save, autosave, append? Folder?
    }

    virtual void OnPaint() override
    {
        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
        BIOS::LCD::Print(m_rcClient.Center().x - 15*4, m_rcClient.Center().y-4,
                         RGB565(ffffff), RGBTRANS, "Not implemented");
        
        int x = 0;
        int y = mSignal.m_rcClient.top - 14;
        x += BIOS::LCD::Print(x, y, RGB565(404040), RGB565(b0b0b0), " Signal ");
        x += BIOS::LCD::Draw( x, y, RGB565(b0b0b0), RGBTRANS, CShapes_tab_right);
    }
    
    virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override
    {
        if (pSender == &mSave)
        {
            Save();
        }
        if (pSender == &mLoad)
        {
            Load();
        }
        if (pSender == &mStartRecord)
        {
            StartRecording();
        }
        if (pSender == &mStopRecord)
        {
            StopRecording();
        }
    }
    
    void Load()
    {
        int counter = 0; // static
        BIOS::LCD::Printf(4, 240-16, RGB565(ffffff), RGB565(202020), "loading rf%03d.sig     ", counter);
        char fileName[16];
        sprintf(fileName, "rf%03d.sig", counter++);
        mStorage.mSignalLength = 0;
        CBufferedReader reader;
        if (reader.Open( fileName ))
        {
            char buf[8];
            int bufi = 0;
            while (!reader.Eof())
            {
                uint8_t c = 0;
                reader >> c;
                if (c >= '0' && c <= '9')
                {
                    if (bufi<7)
                        buf[bufi++] = c;
                } else
                {
                    if (bufi>0)
                    {
                        buf[bufi] = 0;
                        mStorage.mSignalData[mStorage.mSignalLength++] = atoi(buf);
                        bufi = 0;
                    }
                }
            }
            reader.Close();
            BIOS::LCD::Printf(4, 240-16, RGB565(ffffff), RGB565(202020), "Ok!                ");
        } else
        {
            BIOS::LCD::Printf(4, 240-16, RGB565(ffffff), RGB565(202020), "Failed!            ");
        }
        
    }

    void Save()
    {
        int counter = 0; // static
        BIOS::LCD::Printf(4, 240-16, RGB565(ffffff), RGB565(202020), "saving rf%03d.sig     ", counter);
        char fileName[16];
        sprintf(fileName, "rf%03d.sig", counter++);
        CBufferedWriter writer;
        if (writer.Open( fileName ))
        {
            char temp[8];
            writer << "[";
            for (int i=0; i<mStorage.mSignalLength; i++)
            {
                if (i>0)
                    sprintf(temp, ", %d", mStorage.mSignalData[i]);
                else
                    sprintf(temp, "%d", mStorage.mSignalData[i]);
                writer << temp;
            }
            writer << "]";
            writer.Close();
            BIOS::LCD::Printf(4, 240-16, RGB565(ffffff), RGB565(202020), "Ok!                ");
        } else
        {
            BIOS::LCD::Printf(4, 240-16, RGB565(ffffff), RGB565(202020), "Failed!            ");
        }
    }

    void StartRecording()
    {
        mWriter.Open((char*)"alchelog.txt");
        mRuntime.mRecording = true;
    }

    void StopRecording()
    {
        mRuntime.mRecording = false;
        mWriter.Close();
    }
};
