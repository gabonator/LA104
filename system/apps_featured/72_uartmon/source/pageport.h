static constexpr int mBaudrates[] = {110, 150, 300, 1200, 2400, 4800, 9600, 19200, 31250, 38400, 57600, 115200, 230400};

class CPagePort : public CPage
{
    int mBaudOffset{5};    

public:
    CPagePort(CApplication& app) : CPage(app)
    {
    }
    
    virtual const char* Name() override
    {
        return "Port";
    }
    
    virtual void Draw(Layout::Render& r) override
    {
        using namespace Layout;
        GUI::Background(r.GetRectangle(), RGB565(404040), RGB565(404040));
        r << Padding(4, 4, 4, 2);
        
        Color def(RGB565(b0b0b0));
        Color hig(RGB565(ffffff));
        
        #define S(x, y) Select(mApp.mX == x && mApp.mY == y)

        r << hig << "Baudrate: ";
        if (mBaudOffset > 0 )
            r << def << "\x11";
        for (int i=0; i<3; i++)
            r << S(i, 0) << RadioButton(mApp.mBaudrate == mBaudrates[mBaudOffset+i], Format(mBaudrates[mBaudOffset+i]));
        if (mBaudOffset + 2 < COUNT(mBaudrates)-1)
            r << "\x10";

        r << NewLine();
        r << hig << "Data bits:" << def
            << S(0, 1) << RadioButton(mApp.GetDataBits() == 8, "8")
            << S(1, 1) << RadioButton(mApp.GetDataBits() == 9, "9") << Spacer(12);
        r << hig << "Stop bits:" << def
            << S(2, 1) << RadioButton(strcmp(mApp.GetStopBitsAsString(), "1") == 0, "1")
            << S(3, 1) << RadioButton(strcmp(mApp.GetStopBitsAsString(), "1.5") == 0, "1.5")
            << S(4, 1) << RadioButton(strcmp(mApp.GetStopBitsAsString(), "2") == 0, "2") << NewLine();
        r << hig << "Parity:" << def
            << S(0, 2) << RadioButton(strcmp(mApp.GetParityAsString(), "N") == 0, "None")
            << S(1, 2) << RadioButton(strcmp(mApp.GetParityAsString(), "E") == 0, "Even")
            << S(2, 2) << RadioButton(strcmp(mApp.GetParityAsString(), "O") == 0, "Odd");
        
        #undef S
    }
    
    virtual void OnKey(int key) override
    {
        if (key == BIOS::KEY::Left)
        {
            if (mApp.mY == 0 && mApp.mX < 0 && mBaudOffset > 0)
                mBaudOffset--;
        }
        if (key == BIOS::KEY::Right)
        {
            if (mApp.mY == 0 && mApp.mX >= 3 && mBaudOffset < COUNT(mBaudrates)-3)
                mBaudOffset++;
        }

        switch (mApp.mY)
        {
            case 0: mApp.mX = max(0, min(mApp.mX, 2)); break;
            case 1: mApp.mX = max(0, min(mApp.mX, 4)); break;
            case 2: mApp.mX = max(0, min(mApp.mX, 2)); break;
            case 3: mApp.mY = 2; break;
        }
        
        if (key == BIOS::KEY::Enter)
        {
            switch (mApp.mY*16 + mApp.mX)
            {
                case 0x00:
                case 0x01:
                case 0x02: mApp.SetBaudRate(mBaudrates[mBaudOffset+mApp.mX]); break;
                case 0x10: mApp.SetDataBits(8); break;
                case 0x11: mApp.SetDataBits(9); break;
                case 0x12: mApp.SetStopBits("1"); break;
                case 0x13: mApp.SetStopBits("1.5"); break;
                case 0x14: mApp.SetStopBits("2"); break;
                case 0x20: mApp.SetPartiy("N"); break;
                case 0x21: mApp.SetPartiy("E"); break;
                case 0x22: mApp.SetPartiy("O"); break;
            }
        }
    }
};
