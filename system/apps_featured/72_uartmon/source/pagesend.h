static constexpr const char* mSendStrings[] = {
    "AT\r\n", "ATI\r\n", "ATD\r\n", "AT+CMGF=1\r\n", "",
    "A", "B", "C", "D", "0", "1", ""
};

class CPageSend : public CPage
{
    bool mSendInterval{false};
    int mCurrentIndex{0};
    
public:
    CPageSend(CApplication& app) : CPage(app)
    {
    }
    virtual const char* Name() override
    {
        return "Send";
    }
    virtual void Draw(Layout::Render& r) override
    {
        using namespace Layout;
        GUI::Background(r.GetRectangle(), RGB565(404040), RGB565(404040));

        #define S(x, y) Select(mApp.mX == x && mApp.mY == y)
        r << Padding(4, 4, 4, 2);
        int x=0, y=0;
        for (int i=0; i<COUNT(mSendStrings); i++)
        {
            if (strcmp(mSendStrings[i], "") == 0)
            {
                r << NewLine();
                x = 0;
                y++;
                continue;
            }
            r << S(x, y) << Button(mSendStrings[i]);
            if (mApp.mX == x && mApp.mY == y)
                mCurrentIndex = i;
            x++;
        }
        r << S(0, 2) << Radio(mSendInterval, "Hello %d!");
        #undef S
    }
    virtual void OnKey(int key) override
    {
        if (key == BIOS::KEY::Enter)
        {
            switch (mApp.mY*16 + mApp.mX)
            {
                case 0x20: mSendInterval = !mSendInterval; break;
                default:
                    mApp.Send(mSendStrings[mCurrentIndex]);
            }
        }

        switch (mApp.mY)
        {
            case 0: mApp.mX = max(0, min(mApp.mX, 3)); break;
            case 1: mApp.mX = max(0, min(mApp.mX, 5)); break;
            case 2: mApp.mX = max(0, min(mApp.mX, 0)); break;
            case 3: mApp.mY = 2; break;
        }
    }
    
    virtual void OnTick() override
    {
        EVERY(1000)
        {
            if (mSendInterval)
            {
                static int counter = 0;
                char message[16];
                sprintf(message, "Hello %d!", counter++);
                mApp.Send(message);
            }
        }
    }
};
