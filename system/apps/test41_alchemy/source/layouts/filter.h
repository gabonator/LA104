
class CForm
{
protected:
    int mMaxFields{0};
    bool mHasFocus{false};
    int mRow{0};
    bool mToggle{false};
    int mChange{0};
    bool mNewLine{false};
    bool mInterrupted{false};
    
public:
    void Begin(bool hasFocus)
    {
        mMaxFields = 0;
        mHasFocus = hasFocus;
        mNewLine = true;
        mInterrupted = false;
    }
    
    void End()
    {
        mToggle = false;
        mChange = 0;
    }
    
    void ClearLine(int y)
    {
        GUI::Background(CRect(0, y, BIOS::LCD::Width, y+16), RGB565(404040), RGB565(101010));
    }
    
    bool WasInterrupted()
    {
        return mInterrupted;
    }
    
    template <typename T>
    void Static(int &x, int &y, T text)
    {
        if (Interrupt())
            return;
        
        if (mNewLine)
        {
            mNewLine = false;
            ClearLine(y);
        }

        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, text);
    }
    
    template <typename T>
    void Field(int &x, int &y, T text)
    {
        if (mInterrupted)
        {
            mMaxFields++;
            return;
        }
        
        if (Interrupt())
        {
            mInterrupted = true;
            mMaxFields++;
            return;
        }

        if (mNewLine)
        {
            mNewLine = false;
            ClearLine(y);
        }

        if (mHasFocus && mMaxFields == mRow)
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_left);
            x += BIOS::LCD::Print(x, y, RGB565(000000), RGB565(ffffff), text);
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
            x -= 8;
        } else
        {
            x += BIOS::LCD::Print(x, y, RGB565(ffffff), RGBTRANS, text);
        }
        mMaxFields++;
    }
    
    bool Interrupt()
    {
        return false; // TODO: fix
//        return BIOS::KEY::KeyPressed();
    }
    
    void OnToggle(void(*handler)())
    {
        if (mToggle && mMaxFields == mRow)
            handler();
    }

    void OnChange(void(*handler)(int))
    {
        if (mChange != 0 && mMaxFields == mRow)
            handler(mChange);
    }

    void NewLine(int& x, int& y, int& _x)
    {
        x = _x;
        y += 15;
        mNewLine = true;
    }
    
    char* format(char* out, const char *format, ...)
    {
        va_list args;
        va_start( args, format );
        vsprintf(out, format, args );
        va_end(args);
        return out;
    }
    
    void Toggle()
    {
        mToggle = true;
    }
    
    void Change(int n)
    {
        static long lLast = 0;
        long lNow = BIOS::SYS::GetTick();
        long lPassed = lNow - lLast;
        if (lPassed < 150)
            n *= 5;
        lLast = lNow;
        
        mChange = n;
    }
};

class CLayoutFilter : public CWnd, public CForm
{
    CHistogramView mHistogram;
    
public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);

        constexpr int padding = 4;
        constexpr int histogramHeight = 80;
        constexpr int caption = 14;

        CRect rcHistogram(m_rcClient.left + padding, m_rcClient.top + padding + caption,
                          m_rcClient.right - 0 - padding, m_rcClient.top + padding + histogramHeight);
        mHistogram.Create("Histogram", CWnd::WsNoActivate | CWnd::WsVisible, rcHistogram, this);

    }

    virtual void OnPaint() override
    {
        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
        int x, y;
        x = 4;
        y = mHistogram.m_rcClient.top - 14;
        x += BIOS::LCD::Print(x, y, RGB565(404040), RGB565(b0b0b0), " Histogram ");
        x += BIOS::LCD::Draw( x, y, RGB565(b0b0b0), RGBTRANS, CShapes_tab_right);
        DrawForm();
    }
    
    void DrawForm()
    {
        int _x = m_rcClient.left + 8;
        int _y = m_rcClient.top + 8 + 80;
        int x, y;

        char field[16];
        x = _x; y = _y;
        Begin(HasFocus());
        OnToggle([](){mSettings.mTriggerSingle = !mSettings.mTriggerSingle;});
        Field(x, y, mSettings.mTriggerSingle ? "[X]" : "[ ]");
        Static(x, y, " Trigger single");
        
        NewLine(x, y, _x);
        OnToggle([](){mSettings.mBeepCapture = !mSettings.mBeepCapture;});
        Field(x, y, mSettings.mBeepCapture ? "[X]" : "[ ]");
        Static(x, y, " Beep on capture");
        
        NewLine(x, y, _x);
        OnToggle([](){mSettings.mBeepDiscard = !mSettings.mBeepDiscard;});
        Field(x, y, mSettings.mBeepDiscard ? "[X]" : "[ ]");
        Static(x, y, " Beep on discard");
        
        NewLine(x, y, _x);
        Static(x, y, "Gap duration > ");
        OnChange([](int i){ mSettings.mMinGapTime += i * 1000; });
        Field(x, y, format(field, "%d", mSettings.mMinGapTime / 1000));
        Static(x, y, " ms");
        
        NewLine(x, y, _x);
        Static(x, y, "Sample max ");
        OnChange([](int i){
            int& t = mSettings.mSampleMax;
            t = max(4, t+i);
            if (mSettings.mTriggerTransitionsMin > t)
                mSettings.mTriggerTransitionsMin = t;
            if (mSettings.mTriggerTransitionsMax > t)
                mSettings.mTriggerTransitionsMax = t;
        });
        Field(x, y, format(field, "%d", mSettings.mSampleMax));
        Static(x, y, " transitions");
        
        NewLine(x, y, _x);
        Static(x, y, "Transitions ");
        OnChange([](int i){
            int& t = mSettings.mTriggerTransitionsMin;
            t = min(max(4, t+i), mSettings.mSampleMax);
            if (t > mSettings.mTriggerTransitionsMax)
                mSettings.mTriggerTransitionsMax = -1;
        });
        Field(x, y, format(field, "%d", mSettings.mTriggerTransitionsMin));
        Static(x, y, "..");
        OnChange([](int i){
            int& t = mSettings.mTriggerTransitionsMax;
            if (i<0 && t+i<mSettings.mTriggerTransitionsMin)
                t = -1;
            else if (i>0 && t<mSettings.mTriggerTransitionsMin)
                t = mSettings.mTriggerTransitionsMin;
            else
                t+=i;

            if (t > mSettings.mSampleMax)
                t = mSettings.mSampleMax;
        });
        if (mSettings.mTriggerTransitionsMax == -1)
            Field(x, y, "Any");
        else
            Field(x, y, format(field, "%d", mSettings.mTriggerTransitionsMax));

        NewLine(x, y, _x);
        OnToggle([](){mSettings.mTriggerFirstPulse = !mSettings.mTriggerFirstPulse;});
        Field(x, y, mSettings.mTriggerFirstPulse ? "[X]" : "[ ]");
        Static(x, y, " First pulse ");
        Field(x, y, format(field, "%d", 15));
        Static(x, y, "..");
        Field(x, y, format(field, "%d", 150));
        Static(x, y, " ms");
        
        NewLine(x, y, _x);
        OnToggle([](){mSettings.mTriggerFirstGap = !mSettings.mTriggerFirstGap;});
        Field(x, y, mSettings.mTriggerFirstGap ? "[X]" : "[ ]");
        Static(x, y, " First gap ");
        Field(x, y, format(field, "%d", 15));
        Static(x, y, "..");
        Field(x, y, format(field, "%d", 150));
        Static(x, y, " ms");
        
        NewLine(x, y, _x);
        Static(x, y, "Inhibit ");
        OnChange([](int i){
            int& t = mSettings.mInhibit;
            t = min(max(0, t+i), 20);
        });
        Field(x, y, format(field, "%d", mSettings.mInhibit));
        Static(x, y, " ms");

        End();
        
        if (WasInterrupted())
        {
            KillTimer();
            SetTimer(30);
        }
    }
    
    virtual void OnTimer() override
    {
        BIOS::DBG::Print("OnTimer!");
        DrawForm();
        if (!WasInterrupted())
            KillTimer();
    }
    
    virtual void OnKey(int nKey) override
    {
        KillTimer();
        BIOS::SYS::Beep(20);
        if (nKey == BIOS::KEY::Enter)
        {
            Toggle();
            DrawForm();
        }
        if (nKey == BIOS::KEY::Left)
        {
            Change(-1);
            DrawForm();
        }
        if (nKey == BIOS::KEY::Right)
        {
            Change(+1);
            DrawForm();
        }
        if (nKey == BIOS::KEY::Up)
        {
            if (mRow > 0)
            {
                mRow--;
                DrawForm();
                return;
            }
        }
        if (nKey == BIOS::KEY::Down)
        {
            if (mRow < mMaxFields-1)
            {
                mRow++;
                DrawForm();
                return;
            }
        }
        CWnd::OnKey(nKey);
    }
    
};

