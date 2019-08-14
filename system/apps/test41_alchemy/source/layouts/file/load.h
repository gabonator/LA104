class CLayoutFileLoad : public CWnd
{
    CSignalView mSignal;
    int mRow{-1};
    int mRows{0};
    int mSubpath{0};
    BIOS::FAT::TFindFile mCurrent;

public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
        mSignal.Create("Signal", CWnd::WsVisible | CWnd::WsNoActivate, CRect(0, BIOS::LCD::Height-33, BIOS::LCD::Width, BIOS::LCD::Height), this);
    }
    
    virtual void OnPaint() override
    {
        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));

        DrawPath();
        DrawContent();
    }
    
    void DrawPath()
    {
        int x = 8;
        int y = 40;
        GUI::Background(CRect(0, y, BIOS::LCD::Width, y+14), RGB565(404040), RGB565(101010));

        int available = BIOS::LCD::Width/8-2;
        int length = 0;
        int lastGood = 0;
        int sub = 0;
        
        
        if (strlen(mRuntime.mPath) == 0)
        {
            x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, "/");
            return;
        }
        
        for (int i=strlen(mRuntime.mPath)-1; i>0; i--)
        {
           if (mRuntime.mPath[i] == '/')
           {
               sub++;
               length += 2;
               if (length > available)
                   break;

               lastGood = i+1;
           }
           else
           {
               length += 1;
               if (length > available)
                   break;
           }
        }
        
        if (sub-1 < -mSubpath)
            mSubpath = -sub+1;

        char token[32];
        char* cur = &mRuntime.mPath[lastGood];
        while (cur)
        {
            char* slash = strstr(cur, "/");
            if (slash)
            {
                memcpy(token, cur, slash - cur);
                token[slash-cur] = 0;
                cur = slash+1;
            } else
            {
                strcpy(token, cur);
                cur = nullptr;
            }
            sub--;
            if (HasFocus() && mRow == -1 && -mSubpath == sub)
            {
                x -= 8;
                x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_left);
                x += BIOS::LCD::Print(x, y, RGB565(000000), RGB565(ffffff), token);
                x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
                x -= 8;
            } else
            {
                x += BIOS::LCD::Print(x, y, RGB565(ffffff), RGBTRANS, token);
            }
            x += 4;
            x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, "/");
            x += 4;
        }
    }
    
    void DrawContent()
    {
        int x = 8;
        int y = 40 + 16;
        mRows = 0;

        auto DrawLine = [this, &x, &y](BIOS::FAT::TFindFile& f)
        {
            if (mRows > 8)
                return;
            
            x = 8;
            CRect rcLine;
            rcLine = CRect(x-8, y, x+15*8, y+14);
            GUI::Background(rcLine, RGB565(404040), RGB565(101010));

            if (mRow == mRows)
                mCurrent = f;
            
            if (f.nAtrib & BIOS::FAT::EDirectory)
            {
                if (HasFocus() && mRow == mRows)
                {
                    x -= 8;
                    x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_left);
                    x += BIOS::LCD::Printf(x, y, RGB565(000000), RGB565(ffffff), "[%s]", f.strName);
                    x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
                    x -= 8;
                } else
                {
                    BIOS::LCD::Printf(x, y, RGB565(ffffff), RGBTRANS, "[%s]", f.strName);
                }
            } else
            {
                if (HasFocus() && mRow == mRows)
                {
                    x -= 8;
                    x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_left);
                    x += BIOS::LCD::Print(x, y, RGB565(000000), RGB565(ffffff), f.strName);
                    x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
                    x -= 8;
                } else
                {
                    BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, f.strName);
                }
            }
            y += 16;
            mRows++;
        };
        
        if (BIOS::FAT::OpenDir(mRuntime.mPath) != BIOS::FAT::EOk)
        {
            _ASSERT(0);
            return;
        }
        
        BIOS::FAT::TFindFile f;
        while (BIOS::FAT::FindNext(&f) == BIOS::FAT::EOk)
        {
            if (f.strName[0] == '.')
                continue;
            if (!(f.nAtrib & BIOS::FAT::EDirectory))
                continue;
            
            DrawLine(f);
        }
        
        if (BIOS::FAT::OpenDir(mRuntime.mPath) != BIOS::FAT::EOk)
        {
            _ASSERT(0);
            return;
        }

        while (BIOS::FAT::FindNext(&f) == BIOS::FAT::EOk)
        {
            if (f.strName[0] == '.')
                continue;
            if (f.nAtrib & BIOS::FAT::EDirectory)
                continue;
            
            DrawLine(f);
        }
    }
    
    virtual void OnKey(int nKey) override
    {
        if (nKey == BIOS::KEY::Enter)
        {
            if (mRow >= 0)
            {
                if (mCurrent.nAtrib & BIOS::FAT::EDirectory)
                {
                    if (strlen(mRuntime.mPath) > 0)
                        strcat(mRuntime.mPath, "/");
                    strcat(mRuntime.mPath, mCurrent.strName);
                    Invalidate();
                    //DrawPath();
                    //DrawContent();
                }
            } else
            {
                int nToSkip = -mSubpath;
                while (nToSkip > 0 && strlen(mRuntime.mPath) > 0)
                {
                    char* last = strrchr(mRuntime.mPath, '/');
                    _ASSERT(last);
                    if (last)
                        *last = 0;
                    else
                        break;
                    nToSkip--;
                }
                mSubpath = 0;
                Invalidate();
                //DrawPath();
                //DrawContent();
            }
        }
        if (nKey == BIOS::KEY::Left)
        {
            if (mRow == -1)
            {
                mSubpath--;
                DrawPath();
            }
        }
        if (nKey == BIOS::KEY::Right)
        {
            if (mRow == -1 && mSubpath < 0)
            {
                mSubpath++;
                DrawPath();
            }
        }
        if (nKey == BIOS::KEY::Up)
        {
            if (mRow > -1)
            {
                mRow--;
                if (mRow == -1)
                {
                    mSubpath = 0;
                    DrawPath();
                }
                DrawContent();
                return;
            }
        }
        if (nKey == BIOS::KEY::Down)
        {
            if (mRow < mRows-1)
            {
                mRow++;
                if (mRow == 0)
                {
                    DrawPath();
                }
                DrawContent();
                return;
            }
        }
        CWnd::OnKey(nKey);
    }
};
