#include <library.h>
#include "../../os_host/source/framework/BufferedIo.h"
#include "../../os_host/source/framework/BufferedIo.h"
#include "../../os_host/source/framework/Serialize.h"

using namespace BIOS;

#include "shapes.h"
#include "json.h"
#include "font.h"
#include "bitmap.h"
#include "menu.h"

uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];

#ifndef __APPLE__
__attribute__((__section__(".persistent")))
#endif
char mPersistentConfig[256];

void SetCookie(char* cookie)
{
    strcpy(mPersistentConfig, "GABs"); 
    strcpy(mPersistentConfig+4, cookie); 
}

char* GetCookie()
{
#ifdef __APPLE__
    return (char*) "";
#endif
    if (mPersistentConfig[0] != 'G' || mPersistentConfig[1] != 'A' || mPersistentConfig[2] != 'B' || mPersistentConfig[3] != 's')
        return nullptr;
    return mPersistentConfig+4;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

class CTokenizer
{
  char* mString;

public:
  CTokenizer(char* str)
  {
    mString = str;
  }

    CTokenizer(const char* str)
    {
      mString = (char*)str;
    }

  bool GetToken(char* out, int maxLength)
  {
    for (int i=0; i<maxLength-1; i++)
    {
      out[i] = mString[i];
      if (mString[i] == 0)
      {
        mString += i;
        return i > 0;
      }
      if (mString[i] == ' ')
      {
        out[i] = 0;
        mString += i;
        mString++;
        return true;
      }
    }
    _ASSERT(0);
    return false;
  }
};

class CDirInfo
{
    char mFileName[16];
    char mShortName[32];
    char mIconName[16];
    char mExecName[64];
    static char* mRoot;
    int mOrder;

public:
    CDirInfo()
    {
    }
    
    CDirInfo(char* path, char* name)
    {
        char temp[256];
        char* fullPath = temp;
        char* buffer = temp;

        strcpy(mShortName, "");
        strcpy(mIconName, "");
        strcpy(mExecName, "");
        mOrder = 0;

        mRoot = path;
        strcpy(mFileName, name);
        
        bool hasIndex = false;
        CBufferedReader reader;
        {
            if (strstr(name, ".LNK") == nullptr)
            {
                strcpy(fullPath, path);
                strcat(fullPath, "/");
                strcat(fullPath, name);
                strcat(fullPath, "/index.lnk");
                hasIndex = reader.Open(fullPath);
            } else
            {
                strcpy(fullPath, path);
                strcat(fullPath, "/");
                strcat(fullPath, name);
                hasIndex = reader.Open(fullPath);
            }
        }
        if (hasIndex)
        {
            reader >> buffer;
            CJson json(buffer);
            if (json.Verify())
            {
                json["icon"].ToString(mIconName, 16);
                if (strlen(mIconName) == 0)
                {
                    strcpy(mIconName, name);
                    strcat(mIconName, ".bmp");
                }
                json["execute"].ToString(mExecName, 64);
                CJson jsonDesc = json["description"];
                if (jsonDesc)
                {
                    CJson jsonDescShort = jsonDesc["long"];
                    if (jsonDescShort)
                        jsonDescShort.ToString(mShortName, 32);
                    else
                        jsonDesc.ToString(mShortName, 32);
                } else
                {
                    strcpy(mShortName, name);
                }
                mOrder = json["order"].GetNumber();
            } else
            {
                strcpy(mShortName, name);
            }
        } else
        {
            strcpy(mShortName, name);
        }
        if (strlen(mIconName) == 0)
        {
            strcpy(mIconName, name);
            strcat(mIconName, ".bmp");
        }
    }
    
    const char* GetRoot() const
    {
        return mRoot;
    }
    const char* GetShortName() const
    {
        return mShortName;
    }
    const char* GetFileName() const
    {
        return mFileName;
    }
    const char* GetIconName() const
    {
        return mIconName;
    }
    const char* GetExecutable() const
    {
        return mExecName;
    }
    int GetOrder() const
    {
        return mOrder;
    }
};

char* CDirInfo::mRoot = {0};

class CBrowser : public CWnd, public CSmallFont, public CMenuEnumerator
{
    CDirInfo mItemsStorage[16];
    CArray<CDirInfo> mItems;
    int mCursor{0};
    int mScroll{0};
    
    typedef char TFolderName[16];
    TFolderName mFolderStackStorage[8];
    CArray<TFolderName> mFolderStack;
#ifdef __APPLE__
    char mCurrentDir[512];
#else
    char mCurrentDir[64];
#endif
    bool mRunning{true};

public:
    void Create(const char *pszId, ui16 dwFlags, const CRect &rc, CWnd *pParent)
    {
        mFolderStack.Init(mFolderStackStorage, COUNT(mFolderStackStorage));
        mItems.Init(mItemsStorage, COUNT(mItemsStorage));

        CWnd::Create(pszId, dwFlags, rc, pParent);

        if (!Load())
            LoadItems();
        DrawIcons();
    }
    
    virtual CTopMenu::TItem GetItem(int i) override
    {
        if (i==0)
            return CTopMenu::TItem{"LA104 ", CTopMenu::TItem::EState::Default};
        
        i--;
        if (i < mFolderStack.GetSize())
            return CTopMenu::TItem{mFolderStackStorage[i], CTopMenu::TItem::EState::Default};

        return CTopMenu::TItem{nullptr, CTopMenu::TItem::EState::None};
    }

    void PopTo(int i)
    {
        if (i == mFolderStack.GetSize())
            return;
        
        mFolderStack.SetSize(i);
        SendMessage(GetParent(), 0, mFolderStack.GetSize());
        LoadItems();
        DrawIcons();
    }
    
    void DrawIcon(int bx, int by, CDirInfo& info, bool on, int row)
    {        
        CRect rcIcon(bx-4, by-4, bx+64+4, by+64+4);
        char imgSrc[128];
        if (strstr(info.GetFileName(), ".LNK"))
        {
            strcpy(imgSrc, info.GetRoot());
            strcat(imgSrc, "/");
            strcat(imgSrc, info.GetIconName());
        } else
        {
            strcpy(imgSrc, info.GetRoot());
            strcat(imgSrc, "/");
            strcat(imgSrc, info.GetFileName());
            strcat(imgSrc, "/");
            strcat(imgSrc, info.GetIconName());
        }

        FixPath(imgSrc);
    
        char* suffix = imgSrc + strlen(imgSrc) - 4; // .BMP
        _ASSERT(suffix[0] == '.');
        strcpy(suffix, row == 0 ? (on ? ".TM1" : ".TM0") : (on ? ".TM3" : ".TM2"));
        
        if (!LoadImage(imgSrc, rcIcon))
        {
            if (on)
            {
                /*
                 . . .
                 . . . . .
                 . . . . . .
                 . . . . . . .
                 . . . . . . .
                 . . . . . . . .
                 . . . . . . . .
                 . . . . . . . .
                 */
                int shift[8] = {5, 3, 2, 1, 1, 0, 0, 0};
                for (int y=rcIcon.top; y <rcIcon.bottom; y++)
                {
                    CRect sub(rcIcon);
                    sub.top = y;
                    sub.bottom = y+1;
                    int dis = min(y-rcIcon.top, rcIcon.bottom-1-y);
                    if (dis<8)
                    {
                        sub.left += shift[dis];
                        sub.right -= shift[dis];
                    }
                    GUI::Background(sub, RGB565RGB(39, 101, 217), RGB565RGB(39, 101, 217));
                }
            } else
            {
                GUI::Background(rcIcon, RGB565(b0b0b0), RGB565(808080));
            }
            
            if (info.GetIconName()[0])
            {
                strcpy(suffix, ".BMP");
                DrawImage(imgSrc, bx, by);
            }
            strcpy(suffix, row == 0 ? (on ? ".TM1" : ".TM0") : (on ? ".TM3" : ".TM2"));
            SaveImage(imgSrc, rcIcon);
        }
        
        char line0[16];
        char line1[16];
        char line2[16];
        int lines = FitText((char*)info.GetShortName(), 15, line0, line1, line2);
        
        if (on)
        {
            LCD::RoundRect(bx+32-strlen(line0)*3-2, by+64+10-3, bx+32+strlen(line0)*3+2, by+64+10+8+2, RGB565RGB(39, 101, 217));
            if (line1[0])
                LCD::RoundRect(bx+32-strlen(line1)*3-2, by+64+10+8-2, bx+32+strlen(line1)*3+2, by+64+10+8+8+2, RGB565RGB(39, 101, 217));
            if (line2[0])
                LCD::RoundRect(bx+32-strlen(line2)*3-2, by+64+10+16-2, bx+32+strlen(line2)*3+2, by+64+10+8+16+2, RGB565RGB(39, 101, 217));
        } else
        {
            int maxChars = (int)max(max(strlen(line0), strlen(line1)), strlen(line2));
            CRect back(bx+32-maxChars*3-2, by+64+10-3, bx+32+maxChars*3+2, by+64+10+2+8*lines);
            GUI::Background(back, RGB565(b0b0b0), RGB565(808080));
        }
        
        Print(bx+32-strlen(line0)*3, by+64+10, line0, RGB565(ffffff));
        if (line1[0])
            Print(bx+32-strlen(line1)*3, by+64+10+8, line1, RGB565(ffffff));
        if (line2[0])
            Print(bx+32-strlen(line2)*3, by+64+10+16, line2, RGB565(ffffff));
    }
    
    virtual void OnPaint() override
    {
        if (HasFocus())
        {
            DrawCursor(mCursor, true);
        }
    }
    
    void LoadItems()
    {
        mItems.RemoveAll();
        
        strcpy(mCurrentDir, "");
        for (int i=0; i<mFolderStack.GetSize(); i++)
        {
            strcat(mCurrentDir, "/");
            strcat(mCurrentDir, mFolderStack[i]);
        }

        FAT::EResult eOpen = FAT::OpenDir(mCurrentDir);
        if (eOpen == FAT::EResult::EOk)
        {
            FAT::TFindFile file;
            while (FAT::FindNext(&file) == FAT::EResult::EOk)
            {
                if (file.nAtrib & FAT::EAttribute::EHidden)
                    continue;
                
                if (file.strName[0] == '.' || !(file.nAtrib & FAT::EAttribute::EDirectory))
                    continue;
                
                mItems.Add(CDirInfo(mCurrentDir, file.strName));
            }
        }

        eOpen = FAT::OpenDir(mCurrentDir);
        if (eOpen == FAT::EResult::EOk)
        {
            FAT::TFindFile file;
            while (FAT::FindNext(&file) == FAT::EResult::EOk)
            {
                if (file.nAtrib & FAT::EAttribute::EHidden)
                    continue;
                if (file.strName[0] == '.' || (file.nAtrib & FAT::EAttribute::EDirectory))
                    continue;
                if (strstr(file.strName, ".LNK") == nullptr || strstr(file.strName, "INDEX") != nullptr)
                    continue;

                mItems.Add(CDirInfo(mCurrentDir, file.strName));
            }
        }
        
        SortItems();
    }
    
    void SortItems()
    {
        mItems.Sort([](CDirInfo& a, CDirInfo& b) -> int {
            int order = a.GetOrder() - b.GetOrder();
            if (order != 0)
                return order;
            return strcmp(b.GetShortName(), a.GetShortName());
        });
    }
    
    void DrawIcons()
    {
        GUI::Background(m_rcClient, RGB565(b0b0b0), RGB565(808080));
        int index = mScroll;
        for (int y=0; y<2; y++)
            for (int x=0; x<3; x++)
            {
                if (index < mItems.GetSize())
                {
                    CDirInfo& item = mItems[index];
                    DrawIcon(15+98*x, 30+104*y, item, HasFocus() && mCursor == index, y);
                }
                index++;
            }
    }
    
    void DrawCursor(int pos, bool select)
    {
        int index = mScroll;
        for (int y=0; y<2; y++)
            for (int x=0; x<3; x++)
            {
                if (index < mItems.GetSize() && index == pos)
                {
                    CDirInfo& item = mItems[index];
                    DrawIcon(15+98*x, 30+104*y, item, select, y);
                }
                index++;
            }
    }
    
    void OnKey(int nKey) override
    {
        static int lastKey = BIOS::KEY::None;

        int newCursor = mCursor;
        if (nKey == BIOS::KEY::Escape)
        {
            if (mFolderStack.GetSize() == 0)
            {
                if (lastKey == nKey)
                    mRunning = false;
                lastKey = nKey;
            }
            else
            {
                PopTo(mFolderStack.GetSize()-1);
                lastKey = BIOS::KEY::None;
            }
            return;
        }
        lastKey = nKey;

        if (nKey == BIOS::KEY::Left)
            newCursor--;
        if (nKey == BIOS::KEY::Right)
            newCursor++;
        if (nKey == BIOS::KEY::Up)
            newCursor -= 3;
        if (nKey == BIOS::KEY::Down)
            newCursor += 3;

        if (nKey == BIOS::KEY::F1 && mItems.GetSize() > 0)
        {
            if (mItems[mCursor].GetExecutable()[0])
            {
                Save();
                Execute(mItems[mCursor].GetRoot(), mItems[mCursor].GetExecutable());
                return;
            }

            mFolderStack.SetSize(mFolderStack.GetSize()+1);
            strcpy(mFolderStack.GetLast(), mItems[mCursor].GetFileName());
            mScroll = 0;
            mCursor = 0;
            SendMessage(GetParent(), 0, mFolderStack.GetSize());
            LoadItems();
            DrawIcons();
            return;
        }

        if (nKey == BIOS::KEY::F3)
        {
            if (mFolderStack.GetSize() > 0)
            {
                char* lastItem = mFolderStack.RemoveLast();
                LoadItems();
                mCursor = 0;
                
                for (int i=0; i<mItems.GetSize(); i++)
                    if (strcmp(mItems[i].GetFileName(), lastItem) == 0)
                    {
                        mCursor = i;
                        break;
                    }
                
                mScroll = (mCursor / 6)*6;
                SendMessage(GetParent(), 0, mFolderStack.GetSize());
                DrawIcons();
            } else
            {
                if (mCursor != 0)
                {
                    if (mScroll != 0)
                    {
                        mScroll = 0;
                        mCursor = 0;
                        DrawIcons();
                    } else {
                        DrawCursor(mCursor, false);
                        mCursor = 0;
                        DrawCursor(mCursor, true);
                    }
                }
            }
            return;
        }

        if (mItems.GetSize() == 0)
        {
            CWnd::OnKey(BIOS::KEY::Up);
            return;
        }
        
        if (newCursor < 0)
        {
            DrawCursor(mCursor, false);
            CWnd::OnKey(BIOS::KEY::Up);
            return;
        }
        
        
        newCursor = min(max(0, newCursor), mItems.GetSize()-1);
        
        if (newCursor != mCursor)
        {
            if (newCursor < mScroll)
            {
                mCursor = newCursor;
                mScroll -= 6;
                DrawIcons();
            } else
            if (newCursor >= mScroll + 6)
            {
                mCursor = newCursor;
                mScroll += 6;
                DrawIcons();
            } else
            {
                DrawCursor(mCursor, false);
                mCursor = newCursor;
                DrawCursor(mCursor, true);
            }
        }
    }

    void FixPath(char* fixedPath)
    {
        char *parent = nullptr;
        while ((parent = strstr(fixedPath, "/../")) != nullptr)
        {
           char* end = parent+3;
           parent--;
           while (*parent != '/' && parent > fixedPath)
               parent--;

           // remove parent..end
           int charsToRemove = end-parent;
           int chars = strlen(end)+1;
           for (int i=0; i<chars; i++)
               parent[i] = parent[i+charsToRemove];
        }
    }

    void Execute(const char* root, const char* file)
    {
        char fileToken[32];
        char fixedPath[64] = "";
        CTokenizer tokenizer(file);
        while (tokenizer.GetToken(fileToken, 32))
        {
          if (fixedPath[0])
            strcat(fixedPath, " ");

          // switch or file as attribute
          if (fileToken[0] != '-')
          {
            strcat(fixedPath, root);
            strcat(fixedPath, "/");
            strcat(fixedPath, fileToken);
          } else
          {
            strcat(fixedPath, fileToken);
          }
        }
        FixPath(fixedPath);
        //BIOS::DBG::Print("[%s]\n", fixedPath);
        BIOS::OS::SetArgument(fixedPath);
    }

    void Save()
    { 
        char buffer[128];
        strcpy(buffer, "");
        for (int i=0; i<mFolderStack.GetSize(); i++)
        {
            strcat(buffer, mFolderStack[i]);
            strcat(buffer, "/");
        }
        if (mCursor < mItems.GetSize())
        {
          strcat(buffer, mItems[mCursor].GetFileName());
        }

        SetCookie(buffer);
    }

    bool Load()
    {
        char* buffer = GetCookie();
        if (!GetCookie())
            return false;

        mFolderStack.SetSize(0);
        char element[16];
        char *pelement = element;
        while (1)
        {
            *pelement = *buffer;
            if (*pelement == '/' || *pelement == 0)
            {
                *pelement = 0;

                mFolderStack.SetSize(mFolderStack.GetSize()+1);
                strcpy(mFolderStack.GetLast(), element);
                if (*buffer == 0)
                  break;
                pelement = element;
                buffer++;
                continue;
            }
            pelement++;
            buffer++;
        }

        if (mFolderStack.GetSize() > 0)
        {
            char* current = mFolderStack.RemoveLast();

            LoadItems();
            for (int i=0; i<mItems.GetSize(); i++)
                if (strcmp(mItems[i].GetFileName(), current) == 0)
                {
                    mCursor = i;
                    mScroll = (mCursor / 6)*6;

                    SendMessage(GetParent(), 0, mFolderStack.GetSize());
                    return true;
                }
        }
        return false;
    }
    bool IsRunning()
    {
        return mRunning;
    }
};

class CApplication : public CWnd
{
    CMenuMain mMenu;
    CBrowser mBrowser;
    uint8_t mFileSystemBuffer[BIOS::FAT::SectorSize];
    
public:
    void Create()
    {
		BIOS::FAT::SetSharedBuffer(mFileSystemBuffer);
		
        CWnd::Create("Application", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
        mMenu.Create("MainMenu", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, 14), this);
        mMenu.SetEnumerator(&mBrowser);

        mBrowser.Create("Browser", CWnd::WsVisible, CRect(0, 14, BIOS::LCD::Width, BIOS::LCD::Height), this);
        mBrowser.SetFocus();
    }

    virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override
    {
        if (pSender == &mBrowser)
        {
            mMenu.Select(data);
            mMenu.Invalidate();
        }
        if (pSender == &mMenu)
        {
            mBrowser.PopTo(data);
        }
    }

    void Destroy()
    {
		BIOS::FAT::SetSharedBuffer(nullptr);
    }

    bool IsRunning()
    {
        return mBrowser.IsRunning();
    }
};

CApplication app;

#ifndef EMSCRIPTEN
#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
#ifdef __APPLE__
    BIOS::FAT::Init();
    //BIOS::OS::SetArgument((char*)"RFTOOL/RFTOOL.ELF");
#endif
    _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
    BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);

    app.Create();
    app.WindowMessage( CWnd::WmPaint );

    BIOS::KEY::EKey key;
    while (app.IsRunning())
    {
        key = BIOS::KEY::GetKey();
        if (key != BIOS::KEY::None)
            app.WindowMessage(CWnd::WmKey, key);
        app.WindowMessage(CWnd::WmTick);

        if (BIOS::OS::HasArgument()) // was internally set
        {
            BIOS::OS::SetArgument(BIOS::OS::GetArgument()); // hack, keep flag set
            break;
        }
    }
    
    app.Destroy();
    BIOS::FAT::SetSharedBuffer(nullptr);

    return 0;
}

#else
void mainInit(void)
{    
    BIOS::FAT::Init();
    app.Create();
    app.WindowMessage( CWnd::WmPaint );
}

bool mainLoop(void)
{
    BIOS::KEY::EKey key;

    if ((key = BIOS::KEY::GetKey()) == BIOS::KEY::Escape)
        return false;

    if (key != BIOS::KEY::None)
        app.WindowMessage(CWnd::WmKey, key);
    app.WindowMessage(CWnd::WmTick);

    if (BIOS::OS::HasArgument()) // was internally set
    {
        BIOS::OS::SetArgument(BIOS::OS::GetArgument()); // hack, keep flag set
        return false;
    }
    return true;
}

void mainFinish()
{
    app.Destroy();
}

#endif
