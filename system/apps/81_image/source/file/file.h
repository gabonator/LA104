class CFileFilter
{
public:
    virtual bool ShowFile(BIOS::FAT::TFindFile* file) { return true; }
};

class CFileFilterSuffix : public CFileFilter
{
    const char* mSuffix;

public:
    CFileFilterSuffix(const char* suffix) : mSuffix(suffix)
    {
    }
    
    virtual bool ShowFile(BIOS::FAT::TFindFile* file)
    {
        return strstr(file->strName, mSuffix) != nullptr;
    }
};

class CFileDialog : public CWnd
{
    constexpr static int Width = 200;
    constexpr static int Height = 140;
    
    bool mRunning{false};
    char mPath[64];
    char mFilename[64];
    int mScroll{0};
    int mIndex{0};
    int mTotalFiles{0};
    int mMaxLines{0};
    CFileFilter* mFilter{nullptr};
    
public:
    bool ModalShow(CWnd* pParent, const char* caption, CFileFilter* pFilter)
    {
        Create("file", CWnd::WsVisible, CRect(LCD::Width/2 - Width/2, LCD::Height/2 - Height/2, LCD::Width/2 + Width/2, LCD::Height/2 + Height/2), pParent);

        Layout::Render(m_rcClient) << Layout::Window(caption);

        mFilter = pFilter;
        mRunning = true;
        GetCurrentPath(mPath);
        strcpy(mFilename, "");
        mIndex = 0;
        mScroll = 0;

        SetFocus();
        WindowMessage(CWnd::WmPaint);
        while (mRunning)
        {
            BIOS::KEY::EKey key = BIOS::KEY::GetKey();
            if (key != BIOS::KEY::EKey::None)
                WindowMessage(CWnd::WmKey, key);
            WindowMessage(CWnd::WmTick);
        }
        
        Destroy();
        return strlen(mFilename) > 0;
    }
    
    void GetCurrentPath(char* path)
    {
        // Full application path
        strcpy(path, BIOS::OS::GetArgument());

        // Strip app name
        char* last = strrchr(path, '/');
        if (last)
            *last = 0;
        else
            strcpy(path, "");
    }
    
    char* GetFilename()
    {
        return mFilename;
    }
    
    void AppendFile(char* path, int index)
    {
        if (BIOS::FAT::OpenDir(path) != BIOS::FAT::EResult::EOk)
            return;
        
        BIOS::FAT::TFindFile file;
        int i = 0;
        while (BIOS::FAT::FindNext(&file) == BIOS::FAT::EResult::EOk)
        {
            if (mFilter->ShowFile(&file))
            {
                if (i++==index)
                {
                    if (strlen(path) > 0 && path[strlen(path)-1] != '/')
                        strcat(path, "/");
                    strcat(path, file.strName);
                    return;
                }
            }
        }
    }

    int ListFiles(char* path, const CRect& rcWindow, int index, int scroll)
    {
        if (BIOS::FAT::OpenDir(path) != BIOS::FAT::EResult::EOk)
            return 0;
        
        int y = rcWindow.top;
        int i = 0;
        int first = scroll;
        int last = first;
        
        BIOS::FAT::TFindFile file;
        while (BIOS::FAT::FindNext(&file) == BIOS::FAT::EResult::EOk)
        {
            if (mFilter->ShowFile(&file))
            {
                if (scroll > 0)
                {
                    scroll--;
                    i++;
                    continue;
                }
                if (y+16 <= rcWindow.bottom)
                {
                    last++;
                    int x = rcWindow.left;
                    if (file.nAtrib & BIOS::FAT::EAttribute::EDirectory)
                    {
                        if (i==index)
                            x += BIOS::LCD::Printf(x, y, RGB565(000000), RGB565(ffffff), "<%s>", file.strName);
                        else
                            x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGB565(404040), "<%s>", file.strName);
                    } else
                    {
                        if (i==index)
                            x += BIOS::LCD::Printf(x, y, RGB565(404040), RGB565(ffffff), " %s ", file.strName);
                        else
                            x += BIOS::LCD::Printf(x, y, RGB565(b0b0b0), RGB565(404040), " %s ", file.strName);
                    }
                    BIOS::LCD::Bar(x, y, rcWindow.right-4, y+14, RGB565(404040));
                }
                y += 16;
                i++;
            }
        }
        
        int total = i;
        
        int top = rcWindow.top;
        int bottom = rcWindow.bottom - 2;
        BIOS::LCD::Bar(rcWindow.right-4, top, rcWindow.right, bottom, RGB565(606060));
        if (total > 0)
        {
            BIOS::LCD::Bar(rcWindow.right-4, top + first * (bottom - top) / total, rcWindow.right, top + last * (bottom - top) / total, RGB565(d0d0d0));
        }
        return i;
    }

    
    //
    virtual void OnPaint() override
    {
        using namespace Layout;
        CRect rcFiles(m_rcClient);
        rcFiles.Deflate(8, 20, 8, 8);
        //BIOS::LCD::Bar(rcFiles, RGB565(4040b0));
        
        mMaxLines = (rcFiles.Height())/16;
        mTotalFiles = ListFiles(mPath, rcFiles, mIndex, mScroll);
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
        if (key == BIOS::KEY::Down && mIndex < mTotalFiles-1)
        {
            mIndex++;
            if (mIndex >= mScroll+mMaxLines)
                mScroll = mIndex - mMaxLines +1;
            Invalidate();
        }
        if (key == BIOS::KEY::Enter)
        {
            strcpy(mFilename, mPath);
            AppendFile(mFilename, mIndex);
            mRunning = false;
        }

        //if (key == BIOS::KEY::Enter)
//            mRunning = false;
        if (key == BIOS::KEY::Escape)
            mRunning = false;
    }
};
