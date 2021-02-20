class CPageLog : public CPage
{
    bool mLogging {false};
    char mLogFileName[32] = {0};
    
public:
    CPageLog(CApplication& app) : CPage(app)
    {
    }
    
    virtual const char* Name() override
    {
        return "Logger";
    }
    
    virtual void Draw(Layout::Render& r) override
    {
        using namespace Layout;
        GUI::Background(r.GetRectangle(), RGB565(404040), RGB565(404040));
        r << Padding(4, 4, 4, 2);
        
        Color def(RGB565(b0b0b0));
        Color hig(RGB565(ffffff));
        
        using f = CApplication::EFormatMode;
        #define S(x, y) Select(mApp.mX == x && mApp.mY == y)

        r << "Logging: "
            << S(0, 0) << RadioButton(mLogging, mLogging ? "Started" : "Start")
            << S(1, 0) << RadioButton(!mLogging, mLogging ? "Stop" : "Stopped") << NewLine();
        
        if (mLogFileName[0])
            r << "File: " << def << mLogFileName << NewLine();
        
        #undef S
    }

    virtual void OnKey(int key) override
    {
        if (key == BIOS::KEY::Enter)
        {
            switch (mApp.mY*16 + mApp.mX)
            {
                case 0x00:
                    mLogging = mApp.StartLogging();
                    strcpy(mLogFileName, mApp.GetLogName());
                    break;
                case 0x01:
                    mApp.StopLogging();
                    mLogging = false;
                    strcpy(mLogFileName, "");
                    break;
            }
        }

        switch (mApp.mY)
        {
            case 0: mApp.mX = max(0, min(mApp.mX, 1)); break;
            case 1: mApp.mY = 0; break;
        }
    }

};
