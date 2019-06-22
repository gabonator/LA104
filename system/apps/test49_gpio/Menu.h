class CTopMenu : public CWnd
{
public:
    struct TItem
    {
        const char* strName;
        enum EState {None, Static, Default, Selected} eState;
    };

    int mItem{-1};
    int mSelected{-1};

public:
    virtual TItem GetItem(int i) = 0;
    
    virtual void OnPaint()
    {
        GUI::Background(m_rcClient, RGB565(4040b0), RGB565(404040));

        if (!HasFocus())
            mItem = mSelected;

        int x = m_rcClient.left, y = m_rcClient.top;
        TItem item;
        for (int i=0; (item = GetItem(i)).strName; i++)
        {
            TItem::EState state = item.eState;
            if (mItem == -1 && state != TItem::Static)
            {
                mItem = i;
                mSelected = i;
            }
            
            if (mItem == i && state == TItem::Default)
                state = TItem::Selected;
            
            if (x+8+strlen(item.strName)*8+8 >= BIOS::LCD::Width)
                return;

            switch (state)
            {
                case TItem::Static:
                    BIOS::LCD::Bar(x, y, x+4, y+14, RGB565(b0b0b0));
                    x += 4;
                    x += BIOS::LCD::Print(x, y, RGB565(000000), RGB565(b0b0b0), item.strName);
                    x += BIOS::LCD::Draw( x, y, RGB565(b0b0b0), RGBTRANS, CShapes_sel_right);
                    break;
                case TItem::Default:
                    x += 8;
                    x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, item.strName);
                    x += 8;
                    break;
                case TItem::Selected:
                    if (HasFocus())
                    {
                        x += BIOS::LCD::Draw( x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_left);
                        x += BIOS::LCD::Print(x, y, RGB565(000000), RGB565(ffffff), item.strName);
                        x += BIOS::LCD::Draw( x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
                    } else
                    {
                        x += BIOS::LCD::Draw( x, y, RGB565(b0b0b0), RGBTRANS, CShapes_sel_left);
                        x += BIOS::LCD::Print(x, y, RGB565(000000), RGB565(b0b0b0), item.strName);
                        x += BIOS::LCD::Draw( x, y, RGB565(b0b0b0), RGBTRANS, CShapes_sel_right);
                    }
                    break;
                default:
                    break;
            }
        }
    }
    
    virtual void OnKey(ui16 nKey)
    {
        if (nKey == BIOS::KEY::Left)
        {
            for (int i=mItem-1; i>=0; i--)
                if (GetItem(i).eState == TItem::Default)
                {
                    mItem = i;
                    Invalidate();
                    break;
                }
        }
        
        if (nKey == BIOS::KEY::Right)
        {
            for (int i=mItem+1; true; i++)
            {
                if (GetItem(i).eState == TItem::None)
                    break;
                if (GetItem(i).eState == TItem::Default)
                {
                    mItem = i;
                    Invalidate();
                    break;
                }
            }
        }
                
        if (nKey == BIOS::KEY::Enter)
        {
            mSelected = mItem;
            SendMessage(m_pParent, ToWord('M', 'S'), mItem);
            //OnItem(mItem);
        }
        CWnd::OnKey(nKey);
    }

};

class CMenu : public CWnd
{
public:
    struct TItem
    {
        const char* strName;
        enum EState {None, Static, Default, Radio, Selected, SelectedRadio, Focus, SelectedFocus} eState;
        void(*fCallback)();
    };
    
    int mItem{-1};
    
public:
    void Create(const char *pszId, ui16 dwFlags, CWnd* pParent)
    {
        uint8_t nHeight = 16;
        CRect rcRect;
        
        if ( pParent->GetLast() )
        {
            rcRect = pParent->GetLast()->m_rcClient;
            rcRect.top = rcRect.bottom;
            rcRect.bottom = rcRect.top + nHeight;
        } else
        {
            rcRect = pParent->m_rcClient;
            rcRect.top += 20;
            rcRect.bottom = rcRect.top + nHeight;
            rcRect.Deflate(4, 0, 4, 0);
        }
        CWnd::Create( pszId, dwFlags, rcRect, pParent );
    }

    virtual TItem GetItem(int i) = 0;
    
    virtual void OnPaint()
    {
        if (HasFocus())
        {
            BIOS::LCD::Bar(m_rcClient, RGB565(8080d0));
        } else
        {
            BIOS::LCD::Bar(m_rcClient, RGB565(b0b0b0));
        }

        int x = m_rcClient.left, y = m_rcClient.top + 1;
        TItem item;
        for (int i=0; (item = GetItem(i)).strName; i++)
        {
            TItem::EState state = item.eState;
            if (mItem == -1 && state != TItem::Static)
                mItem = i;
            
            if (mItem == i && state == TItem::Default)
                state = TItem::Selected;

            if (mItem == i && state == TItem::Radio)
                state = TItem::SelectedRadio;

            if (x+8+strlen(item.strName)*8+8 >= BIOS::LCD::Width)
                return;

            switch (state)
            {
                case TItem::Static:
                    x += 4;
                    x += BIOS::LCD::Print(x, y, RGB565(000000), RGBTRANS, item.strName);
                    break;
                case TItem::Default:
                    x += BIOS::LCD::Draw( x, y, RGB565(909090), RGBTRANS, CShapes_sel_left);
                    x += BIOS::LCD::Print(x, y, RGB565(000000), RGB565(909090), item.strName);
                    x += BIOS::LCD::Draw( x, y, RGB565(909090), RGBTRANS, CShapes_sel_right);
                    break;
                case TItem::Radio:
                    if (HasFocus())
                    {
                        x += BIOS::LCD::Draw( x, y, RGB565(909090), RGBTRANS, CShapes_sel_left);
                        x += BIOS::LCD::Print(x, y, RGB565(000000), RGB565(909090), item.strName);
                        x += BIOS::LCD::Draw( x, y, RGB565(909090), RGBTRANS, CShapes_sel_right);
                    } else
                    {
                        x += BIOS::LCD::Draw( x, y, RGB565(909090), RGBTRANS, CShapes_sel_left);
                        x += BIOS::LCD::Print(x, y, RGB565(000000), RGB565(909090), item.strName);
                        x += BIOS::LCD::Draw( x, y, RGB565(909090), RGBTRANS, CShapes_sel_right);
                    }
                    break;
                case TItem::SelectedRadio:
                    if (HasFocus())
                    {
                        x += BIOS::LCD::Draw( x, y, RGB565(FFFFFF), RGBTRANS, CShapes_sel_left);
                        x += BIOS::LCD::Print(x, y, RGB565(000000), RGB565(FFFFFF), item.strName);
                        x += BIOS::LCD::Draw( x, y, RGB565(FFFFFF), RGBTRANS, CShapes_sel_right);
                    } else
                    {
                        x += BIOS::LCD::Draw( x, y, RGB565(000000), RGBTRANS, CShapes_sel_left);
                        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGB565(000000), item.strName);
                        x += BIOS::LCD::Draw( x, y, RGB565(000000), RGBTRANS, CShapes_sel_right);
                    }
                    break;
                case TItem::Selected:
                    if (HasFocus())
                    {
                        x += BIOS::LCD::Draw( x, y, RGB565(FFFFFF), RGBTRANS, CShapes_sel_left);
                        x += BIOS::LCD::Print(x, y, RGB565(000000), RGB565(FFFFFF), item.strName);
                        x += BIOS::LCD::Draw( x, y, RGB565(FFFFFF), RGBTRANS, CShapes_sel_right);
                    } else
                    {
                        x += BIOS::LCD::Draw( x, y, RGB565(808080), RGBTRANS, CShapes_sel_left);
                        x += BIOS::LCD::Print(x, y, RGB565(000000), RGB565(808080), item.strName);
                        x += BIOS::LCD::Draw( x, y, RGB565(808080), RGBTRANS, CShapes_sel_right);
                    }
                    break;
                default:
                    break;
            }
        }
    }
    
    virtual void OnKey(ui16 nKey)
    {
        if (nKey == BIOS::KEY::Left)
        {
            for (int i=mItem-1; i>=0; i--)
                if (GetItem(i).eState == TItem::Default || GetItem(i).eState == TItem::Radio)
                {
                    mItem = i;
                    Invalidate();
                    break;
                }
        }
        if (nKey == BIOS::KEY::Right)
        {
            for (int i=mItem+1; true; i++)
            {
                if (GetItem(i).eState == TItem::None)
                    break;
                if (GetItem(i).eState == TItem::Default || GetItem(i).eState == TItem::Radio)
                {
                    mItem = i;
                    Invalidate();
                    break;
                }
            }
        }
        if (nKey == BIOS::KEY::Enter)
        {
            OnItem(mItem);
        }
        CWnd::OnKey(nKey);
    }
    
    virtual void OnItem(int nItem)
    {
      if (GetItem(nItem).fCallback)
        GetItem(nItem).fCallback();
    }
};
