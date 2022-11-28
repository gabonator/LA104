class CPageGpio : public CPage
{
    enum TPinMode {
        InputFloat,
        InputPullUp,
        InputPullDown,
        OutputHigh,
        OutputLow
    };
    
    TPinMode mP3{InputFloat};
    TPinMode mP4{InputFloat};
    
public:
    CPageGpio(CApplication& app) : CPage(app)
    {
    }
    
    virtual const char* Name() override
    {
        return "Gpio";
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

        r << "P3: "
            << S(0, 0) << RadioButton(mP3 == InputFloat, "Input")
            << S(1, 0) << RadioButton(mP3 == InputPullDown, "In/Low")
            << S(2, 0) << RadioButton(mP3 == InputPullUp, "In/High")
            << S(3, 0) << RadioButton(mP3 == OutputLow, "Low")
            << S(4, 0) << RadioButton(mP3 == OutputHigh, "High") << NewLine();
        r << "P4: "
            << S(0, 1) << RadioButton(mP4 == InputFloat, "Input")
            << S(1, 1) << RadioButton(mP4 == InputPullDown, "In/Low")
            << S(2, 1) << RadioButton(mP4 == InputPullUp, "In/High")
            << S(3, 1) << RadioButton(mP4 == OutputLow, "Low")
            << S(4, 1) << RadioButton(mP4 == OutputHigh, "High") << NewLine();
        #undef S
    }

    virtual void OnKey(int key) override
    {
        if (key == BIOS::KEY::Enter)
        {
            switch (mApp.mY*16 + mApp.mX)
            {
                case 0x00:
                    mP3 = InputFloat;
                    Gpio::SetState(Gpio::BASEB, Gpio::P3, Gpio::StateInput | Gpio::StateInputFloating);
                    break;
                case 0x01:
                    mP3 = InputPullDown;
                    Gpio::SetState(Gpio::BASEB, Gpio::P3, Gpio::StateInputPull | Gpio::StateInputFloating);
                    Gpio::SetLevel(Gpio::BASEB, Gpio::P3, 0);
                    break;
                case 0x02:
                    mP3 = InputPullUp;
                    Gpio::SetState(Gpio::BASEB, Gpio::P3, Gpio::StateInputPull | Gpio::StateInputFloating);
                    Gpio::SetLevel(Gpio::BASEB, Gpio::P3, 1);
                    break;
                case 0x03:
                    mP3 = OutputLow;
                    Gpio::SetState(Gpio::BASEB, Gpio::P3, Gpio::StateOutput10Mhz | Gpio::StateOutputPushPull);
                    Gpio::SetLevel(Gpio::BASEB, Gpio::P3, 0);
                    break;
                case 0x04:
                    mP3 = OutputHigh;
                    Gpio::SetState(Gpio::BASEB, Gpio::P3, Gpio::StateOutput10Mhz | Gpio::StateOutputPushPull);
                    Gpio::SetLevel(Gpio::BASEB, Gpio::P3, 1);
                    break;
                case 0x10:
                    mP4 = InputFloat;
                    Gpio::SetState(Gpio::BASEB, Gpio::P4, Gpio::StateInput | Gpio::StateInputFloating);
                    break;
                case 0x11:
                    mP4 = InputPullDown;
                    Gpio::SetState(Gpio::BASEB, Gpio::P4, Gpio::StateInputPull | Gpio::StateInputFloating);
                    Gpio::SetLevel(Gpio::BASEB, Gpio::P4, 0);
                    break;
                case 0x12:
                    mP4 = InputPullUp;
                    Gpio::SetState(Gpio::BASEB, Gpio::P4, Gpio::StateInputPull | Gpio::StateInputFloating);
                    Gpio::SetLevel(Gpio::BASEB, Gpio::P4, 1);
                    break;
                case 0x13:
                    mP4 = OutputLow;
                    Gpio::SetState(Gpio::BASEB, Gpio::P4, Gpio::StateOutput10Mhz | Gpio::StateOutputPushPull);
                    Gpio::SetLevel(Gpio::BASEB, Gpio::P4, 0);
                    break;
                case 0x14:
                    mP4 = OutputHigh;
                    Gpio::SetState(Gpio::BASEB, Gpio::P4, Gpio::StateOutput10Mhz | Gpio::StateOutputPushPull);
                    Gpio::SetLevel(Gpio::BASEB, Gpio::P4, 1);
                    break;
            }
        }
        mApp.mX = max(0, min(mApp.mX, 4));
    }

};
