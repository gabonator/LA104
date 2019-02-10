class CLayoutFilter : public CWnd
{
    int mRow{0};
    
public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
    }

    virtual void OnPaint() override
    {
        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
        int _x = m_rcClient.left + 8;
        int _y = m_rcClient.top + 8;
        
        int x, y;
        x = _x; y = _y;
        
        if (HasFocus() && mRow == 0)
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_left);
            x += BIOS::LCD::Printf(x, y, RGB565(000000), RGB565(ffffff), "[%c]", mStorage.mTriggerSingle ? 'X' : ' ');
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
            x -= 8;
        } else
        {
            x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGBTRANS, "[%c]", mStorage.mTriggerSingle ? 'X' : ' ');
        }
        
        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, " Trigger single");
    }
    
    virtual void OnKey(ui16 nKey) override
    {
        if (nKey == BIOS::KEY::Enter)
        {
            switch (mRow)
            {
                case 0: mStorage.mTriggerSingle = !mStorage.mTriggerSingle; break;
            }
            Invalidate();
        }
        if (nKey == BIOS::KEY::Up)
        {
            if (mRow > 0)
            {
                mRow--;
                Invalidate();
                return;
            }
        }
        if (nKey == BIOS::KEY::Down)
        {
            if (mRow < 0)
            {
                mRow++;
                Invalidate();
                return;
            }
        }
        CWnd::OnKey(nKey);
    }
    
};

