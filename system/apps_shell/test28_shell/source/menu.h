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
    
    virtual void OnPaint() override
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
                        if (i==0)
                            x += BIOS::LCD::Print(x, y, RGB565(000000), RGB565RGB(39, 101, 217), " ");
                        else
                            x += BIOS::LCD::Draw( x, y, RGB565RGB(39, 101, 217), RGBTRANS, CShapes_sel_left);
                        x += BIOS::LCD::Print(x, y, RGB565(ffffff), RGB565RGB(39, 101, 217), item.strName);
                        x += BIOS::LCD::Draw( x, y, RGB565RGB(39, 101, 217), RGBTRANS, CShapes_sel_right);
                    } else
                    {
                        if (i==0)
                            x += BIOS::LCD::Print(x, y, RGB565(000000), RGB565(b0b0b0), " ");
                        else
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
    
    void Select(int n)
    {
        mSelected = n;
        mItem = n;
    }
    
    virtual void OnKey(int nKey) override
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
                {
                    nKey = BIOS::KEY::Down; // jump to next window
                    break;
                }
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

class CMenuEnumerator
{
public:
    virtual CTopMenu::TItem GetItem(int i) = 0;
};

class CMenuMain : public CTopMenu
{
    CMenuEnumerator* mEnumerator{nullptr};
    
public:
    void SetEnumerator(CMenuEnumerator* enumerator)
    {
        mEnumerator = enumerator;
    }
    
    virtual TItem GetItem(int i)
    {
        if (mEnumerator)
            return mEnumerator->GetItem(i);
        
        return TItem{nullptr, TItem::None};
    }
};
