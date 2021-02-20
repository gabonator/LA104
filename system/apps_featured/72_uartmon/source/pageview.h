class CPageView : public CPage
{
public:
    CPageView(CApplication& app) : CPage(app)
    {
    }
    
    virtual const char* Name() override
    {
        return "View";
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

        r << S(0, 0) << Button("Clear")
          << S(1, 0) << Radio(mApp.mNewLineRx, "new line on CR/LF,")
          << S(2, 0) << Radio(mApp.mMarkPause, "pause") << NewLine()
          << hig << "TX" << def << " (P1):"
                << S(0, 1) << Radio(mApp.mTxMode == f::Ascii, "ascii") << ","
                << S(1, 1) << Radio(mApp.mTxMode == f::Hex, "hex") << ","
                << S(2, 1) << Radio(mApp.mTxMode == f::Dec, "dec") << "   "
                << NewLine()
          << hig << "RX" << def << " (P2):"
                << S(0, 2) << Radio(mApp.mRxMode == f::Ascii, "ascii") << ","
                << S(1, 2) << Radio(mApp.mRxMode == f::Hex, "hex") << ","
                << S(2, 2) << Radio(mApp.mRxMode == f::Dec, "dec");
        #undef S
    }
    
    virtual void OnKey(int key) override
    {
        if (key == BIOS::KEY::Enter)
        {
            using f = CApplication::EFormatMode;

            switch (mApp.mY*16 + mApp.mX)
            {
                case 0x00: mApp.ClearLog(); break;
                case 0x01: mApp.mNewLineTx = mApp.mNewLineRx = !mApp.mNewLineRx; break;
                case 0x02: mApp.mMarkPause = !mApp.mMarkPause; break;
                case 0x10: mApp.mTxMode = f::Ascii; break;
                case 0x11: mApp.mTxMode = f::Hex; break;
                case 0x12: mApp.mTxMode = f::Dec; break;
                case 0x20: mApp.mRxMode = f::Ascii; break;
                case 0x21: mApp.mRxMode = f::Hex; break;
                case 0x22: mApp.mRxMode = f::Dec; break;
            }
        }

        switch (mApp.mY)
        {
            case 0: mApp.mX = max(0, min(mApp.mX, 2)); break;
            case 1: mApp.mX = max(0, min(mApp.mX, 2)); break;
            case 2: mApp.mX = max(0, min(mApp.mX, 2)); break;
            case 3: mApp.mY = 2; break;
        }
    }
};

