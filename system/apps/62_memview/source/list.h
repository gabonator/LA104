template <int N> class CString
{
public:
    char mData[N];
public:
    CString()
    {
        mData[0] = 0;
    }
    CString(char* str)
    {
        _ASSERT(strlen(str) < N-1);
        strcpy(mData, str);
    }
    CString(const char* str)
    {
        _ASSERT(strlen(str) < N-1);
        strcpy(mData, str);
    }
    void operator = (char* str)
    {
        _ASSERT(strlen(str) < N-1);
        strcpy(mData, str);
    }
    operator char* () const
    {
        return mData;
    }
    operator char* ()
    {
        return mData;
    }
};

class CListDialog : public CWnd
{
    constexpr static int Width = 250;
    constexpr static int Height = 140;
    
    bool mSelected{false};
    bool mCancelled{false};
    int mScroll{0};
    int mIndex{0};
    int mMaxLines{0};
public:
    CString<32> mElementsData[16];
    CArray<CString<32>> mElements;
    
public:
//    CListDialog()
//    {
//        mElements.Init(mElementsData, COUNT(mElementsData));
//    }
    
    int ModalShow(CWnd* pParent, const char* caption)
    {
        int lcdWidth = BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::ScreenWidth);
        int lcdHeight = BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::ScreenHeight);
        Create("list", CWnd::WsVisible, CRect(lcdWidth/2 - Width/2, lcdHeight/2 - Height/2, lcdWidth/2 + Width/2, lcdHeight/2 + Height/2), pParent);

        Layout::Render(m_rcClient) << Layout::Window(caption);

        mSelected = false;
        mCancelled = false;
//        mIndex = 0;
//        mScroll = 0;

        SetFocus();
        WindowMessage(CWnd::WmPaint);
        while (!mSelected && !mCancelled)
        {
            BIOS::KEY::EKey key = BIOS::KEY::GetKey();
            if (key != BIOS::KEY::EKey::None)
                WindowMessage(CWnd::WmKey, key);
            WindowMessage(CWnd::WmTick);
        }
        
        Destroy();
        pParent->SetFocus();
        return mSelected ? mIndex : -1;
    }
    
    void ListItems(const CRect& rcWindow, int index, int scroll)
    {
        
        int y = rcWindow.top;
        int i = 0;
        int first = scroll;
        int last = first;
        
        for (int i=0; i<mMaxLines; i++)
        {
            int x = rcWindow.left;
            if (i+scroll >= mElements.GetSize())
                break;
            if (i+scroll==index)
            {
                x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGB565(404040), CShapes_sel_left);
                x += BIOS::LCD::Printf(x, y, RGB565(000000), RGB565(ffffff), "%s", (char*)mElements[i+scroll]);
                x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGB565(404040), CShapes_sel_right);
            }
            else
            {
                x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGB565(404040), " %s", (char*)mElements[i+scroll]);
            }
            BIOS::LCD::Bar(x, y, rcWindow.right-4, y+14, RGB565(404040));
            y += 16;
        }
    }

    virtual void OnPaint() override
    {
        using namespace Layout;
        CRect rcList(m_rcClient);
        rcList.Deflate(8, 20, 8, 8);
        
        mMaxLines = (rcList.Height())/16;
        ListItems(rcList, mIndex, mScroll);
    }
    
    virtual void OnKey(int key) override
    {
        if (key == BIOS::KEY::Up && mIndex > 0)
        {
            mIndex--;
            if (mIndex < mScroll)
                mScroll = mIndex;
            Invalidate();
        }
        if (key == BIOS::KEY::Down && mIndex < mElements.GetSize()-1)
        {
            mIndex++;
            if (mIndex >= mScroll+mMaxLines)
                mScroll = mIndex - mMaxLines +1;
            Invalidate();
        }
        if (key == BIOS::KEY::Enter)
            mSelected = true;
        if (key == BIOS::KEY::Escape)
            mCancelled = true;
    }
};
