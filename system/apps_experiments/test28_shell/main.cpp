#include <library.h>
#include "../../os_host/source/framework/BufferedIo.h"
#include "shapes.h"
#include "../../os_host/source/framework/BufferedIo.h"
#include "../../os_host/source/framework/Serialize.h"
//#include "../../os_host/source/framework/Serialize.cpp"

#include "json.h"

#ifdef __APPLE__
#define ROOT "../../../../system/apps_experiments/test28_shell/"
#else
#define ROOT ""
#endif

#pragma pack(push)
#pragma pack(2)
struct BmpHdr
{
    uint16_t wBfType;
    uint32_t dwBfSize;
    uint16_t wBfReserved1;
    uint16_t wBfReserved2;
    uint32_t dwBfOffset;
    
    uint32_t dwBiSize;
    uint32_t dwBiWidth;
    uint32_t dwBiHeight;
    uint16_t wBiPlanes;
    uint16_t wBiBitDepth;
    uint32_t dwBiCompression;
    uint32_t dwBiSizeImage;
    uint32_t dwBiXPels;
    uint32_t dwBiYPels;
    
    uint32_t dwBiClrUsed;
    uint32_t dwBiClrImportant;
};
#pragma pack(pop)

using namespace BIOS;

void _HandleAssertion(const char* file, int line, const char* cond)
{
    //BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}
uint8_t getfont68(char c, int i);
uint_fast16_t InterpolateColor( uint_fast16_t clrA, uint_fast16_t clrB, uint_fast8_t nLevel );

void DrawImage(char* path, int bx, int by)
{
    CBufferedReader2 reader;
    if (!reader.Open(path))
        return;
    
    BmpHdr header;
    reader >> CStream(&header, sizeof(header));
    reader.Seek(header.dwBfOffset);
    
    for (int y=0; y<(int)header.dwBiHeight; y++)
        for (int x=0; x<(int)header.dwBiWidth; x++)
        {
            uint8_t color[4];
            reader >> CStream(color, 4);
            if (!color[3])
                continue;
            int c = RGB565RGB(color[2], color[1], color[0]);
            if (color[3] > 250)
            {
                LCD::PutPixel(bx+x, by+header.dwBiHeight-1-y, c);
                continue;
            }
            int c0 = LCD::GetPixel(bx+x, by+header.dwBiHeight-1-y);
            int c1 = InterpolateColor(c0, c, color[3]);
            LCD::PutPixel(bx+x, by+header.dwBiHeight-1-y, c1);
        }
}

void SaveImage(char* path, CRect rc)
{
#ifdef __APPLE__
    return;
#endif

    CBufferedWriter2 writer;
    //LCD::BufferBegin(rc); // TODO: not working
    
    writer.Open(path);
#ifdef __APPLE__
    for (int y=rc.bottom-1; y>=rc.top; y--)
        for (int x=rc.left; x<rc.right; x++)
#else
    for (int x=rc.left; x<rc.right; x++)
        for (int y=rc.bottom-1; y>=rc.top; y--)
#endif
            writer << (uint16_t)LCD::GetPixel(x, y);
    
    writer.Close();
}

bool LoadImage(char* path, CRect rc)
{
#ifdef __APPLE__
    return false;
#endif
    CBufferedReader2 reader;
    if (!reader.Open(path))
        return false;
    
    int offset = 0;
    int fileOffset = 0;
    int pixelCount = BIOS::FAT::SectorSize/2;
    uint16_t* pixelData = (uint16_t*)BIOS::FAT::GetSharedBuffer();

    LCD::BufferBegin(rc);
    for (int y=rc.top; y<rc.bottom; y++)
        for (int x=rc.left; x<rc.right; x++)
        {
            LCD::BufferWrite(pixelData[offset++]);
            if (offset >= pixelCount)
            {
                fileOffset += BIOS::FAT::SectorSize;
                reader.Seek(fileOffset);
                offset = 0;
            }
        }

    return true;
}

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
                        x += BIOS::LCD::Draw( x, y, RGB565RGB(39, 101, 217), RGBTRANS, CShapes_sel_left);
                        x += BIOS::LCD::Print(x, y, RGB565(ffffff), RGB565RGB(39, 101, 217), item.strName);
                        x += BIOS::LCD::Draw( x, y, RGB565RGB(39, 101, 217), RGBTRANS, CShapes_sel_right);
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
    
    void Select(int n)
    {
        mSelected = n;
        mItem = n;
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

uint_fast16_t InterpolateColor( uint_fast16_t clrA, uint_fast16_t clrB, uint_fast8_t nLevel )
{
    int br = Get565R(clrB)*nLevel;
    int bg = Get565G(clrB)*nLevel;
    int bb = Get565B(clrB)*nLevel;
    nLevel = 255-nLevel;
    int ar = Get565R(clrA)*nLevel;
    int ag = Get565G(clrA)*nLevel;
    int ab = Get565B(clrA)*nLevel;
    ar = (ar+br) / 256;
    ag = (ag+bg) / 256;
    ab = (ab+bb) / 256;
    return RGB565RGB(ar, ag, ab);
}

class CDirInfo
{
    char mFileName[16];
    char mShortName[32];
    char mIconName[16];
    static char* mRoot;
public:
    CDirInfo()
    {
    }
    
    CDirInfo(char* path, char* name)
    {
        strcpy(mShortName, "");
        strcpy(mIconName, "");

        mRoot = path;
        strcpy(mFileName, name);
        
        bool hasIndex = false;
        CBufferedReader2 reader;
        {
            char fullPath[256];
            strcpy(fullPath, path);
            strcat(fullPath, "/");
            strcat(fullPath, name);
            strcat(fullPath, "/index.jsn");
            hasIndex = reader.Open(fullPath);
        }
        if (hasIndex)
        {
            char buffer[256];
            reader >> buffer;
            CJson json(buffer);
            if (json.Verify())
            {
                json["icon"].ToString(mIconName, 16);
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
            } else
            {
//                _ASSERT(0);
                strcpy(mShortName, name);
            }
        } else
        {
            strcpy(mIconName, name);
            strcat(mIconName, ".bmp");
            strcpy(mShortName, name);
        }

    }
    
    char* GetRoot()
    {
        return mRoot;
    }
    char* GetShortName()
    {
        return mShortName;
    }
    char* GetFileName()
    {
        return mFileName;
    }
    char* GetIconName()
    {
        return mIconName;
    }
};

char* CDirInfo::mRoot = nullptr;

class CSmallFont
{
public:
    void Print(int bx, int by, const char* msg, uint16_t color)
    {
        while (*msg)
        {
            Printch(bx, by, *msg++, color);
            bx += 6;
        }
    }
    
    int FitText(char* text, int maxChars, char* line0, char* line1, char* line2)
    {
        auto TakeLine = [](char* text, int maxLength, char* output) -> int
        {
            int lastSpacer = 0;
            
            int len = (int)strlen(text);
            
            if (len < maxLength)
            {
                // fits whole text
                memcpy(output, text, len);
                output[len] = 0;
                return len;
            } else
            {
                len = maxLength;
            }
            
            for (int i=0; i<len; i++)
                if (text[i] == ' ')
                    lastSpacer = i;
            
            if (lastSpacer == 0)
            {
                // cut long word
                memcpy(output, text, maxLength);
                output[maxLength] = 0;
                return maxLength;
            }
            
            memcpy(output, text, lastSpacer);
            output[lastSpacer] = 0;
            return lastSpacer+1;
        };
        
        auto AppendDots = [](char* text, int length)
        {
            if ((int)strlen(text) < length-3)
            {
                strcat(text, "...");
                return;
            }
            strcpy(text+length-3, "...");
        };
        
        line0[0] = line1[0] = line2[0] = 0;
        
        text += TakeLine(text, maxChars, line0);
        if (strlen(text) == 0)
            return 1;
        
        text += TakeLine(text, maxChars, line1);
        if (strlen(text) == 0)
            return 2;
        
        text += TakeLine(text, maxChars, line2);
        if (strlen(text) > 0)
            AppendDots(line2, maxChars);
        return 3;
    }

private:
    void Printch(int bx, int by, char ch, uint16_t c)
    {
        if (ch == ' ')
            return;
        for (int y=0; y<8; y++)
            for (int x=0; x<6; x++)
                if (getfont68(ch, x) & (128>>y))
                    LCD::PutPixel(bx+x, by+y, c);
        
    }
};

class CBrowser : public CWnd, public CSmallFont, public CMenuEnumerator
{
    CDirInfo mItemsStorage[16];
    CArray<CDirInfo> mItems;
    int mCursor{0};
    int mScroll{0};
    
    typedef char TFolderName[16];
    TFolderName mFolderStackStorage[8];
    CArray<TFolderName> mFolderStack;
    //char mPath[64];

public:
    void Create(const char *pszId, ui16 dwFlags, const CRect &rc, CWnd *pParent)
    {
        mFolderStack.Init(mFolderStackStorage, COUNT(mFolderStackStorage));
        mItems.Init(mItemsStorage, COUNT(mItemsStorage));

        CWnd::Create(pszId, dwFlags, rc, pParent);
        LoadItems();
        DrawIcons();
    }
    
    virtual CTopMenu::TItem GetItem(int i)
    {
        if (i==0)
            return CTopMenu::TItem{"LA104 apps", CTopMenu::TItem::EState::Default};
        
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
        LoadItems();
        DrawIcons();
        SendMessage(GetParent(), 0, mFolderStack.GetSize());
    }
    
    void DrawIcon(int bx, int by, CDirInfo& info, bool on = false)
    {
        CRect rcIcon(bx-4, by-4, bx+64+4, by+64+4);
        char imgSrc[128];
        strcpy(imgSrc, info.GetRoot());
        for (int i=0; i<mFolderStack.GetSize(); i++)
        {
            strcat(imgSrc, "/");
            strcat(imgSrc, mFolderStack[i]);
        }
        strcat(imgSrc, "/");
        strcat(imgSrc, info.GetFileName());
        strcat(imgSrc, "/");
        char* pimgSrcFile = imgSrc + strlen(imgSrc);
        strcpy(pimgSrcFile, on ? "folder1.tmp" : "folder0.tmp");
        
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
                strcpy(pimgSrcFile, info.GetIconName());
                DrawImage(imgSrc, bx, by);
            }
            strcpy(pimgSrcFile, on ? "folder1.tmp" : "folder0.tmp");
            SaveImage(imgSrc, rcIcon);
        }
        
        char line0[16];
        char line1[16];
        char line2[16];
        int lines = FitText(info.GetShortName(), 15, line0, line1, line2);
        
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
    
    virtual void OnPaint()
    {
        if (HasFocus())
        {
            DrawCursor(mCursor, true);
        }
    }
    
    void LoadItems()
    {
        mItems.RemoveAll();
        
#ifdef __APPLE__
        static char* rootPath = (char*)"/Users/gabrielvalky/Documents/git/LA104/system/bin/apps";
        char fullPath[256];
#else
        static char* rootPath = (char*)"APPS";
        char fullPath[64];
#endif
        
        strcpy(fullPath, rootPath);
        for (int i=0; i<mFolderStack.GetSize(); i++)
        {
            strcat(fullPath, "/");
            strcat(fullPath, mFolderStack[i]);
        }
        
        FAT::EResult eOpen = FAT::OpenDir(fullPath);
        if (eOpen == FAT::EResult::EOk)
        {
            FAT::TFindFile file;
            while (FAT::FindNext(&file) == FAT::EResult::EOk)
            {
                if (file.strName[0] == '.' || !(file.nAtrib & FAT::EAttribute::EDirectory))
                    continue;
                
                mItems.Add(CDirInfo(rootPath, file.strName));
            }
        }
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
                    DrawIcon(15+90*x, 30+100*y, item, HasFocus() && mCursor == index);
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
                    DrawIcon(15+90*x, 30+100*y, item, select);
                }
                index++;
            }
    }
    
    void OnKey(ui16 nKey)
    {
        int newCursor = mCursor;
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
            mFolderStack.SetSize(mFolderStack.GetSize()+1);
            strcpy(mFolderStack.GetLast(), mItems[mCursor].GetFileName());
            mScroll = 0;
            mCursor = 0;
            LoadItems();
            DrawIcons();
            SendMessage(GetParent(), 0, mFolderStack.GetSize());
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
                DrawIcons();
                SendMessage(GetParent(), 0, mFolderStack.GetSize());
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
};

class CApplication : public CWnd
{
    CMenuMain mMenu;
    CBrowser mBrowser;
    
public:
    void Create()
    {
        CWnd::Create("Application", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
        mMenu.Create("MainMenu", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, 14), this);
        mBrowser.Create("Browser", CWnd::WsVisible, CRect(0, 14, BIOS::LCD::Width, BIOS::LCD::Height), this);
        mMenu.SetEnumerator(&mBrowser);
    }

    virtual void OnMessage(CWnd* pSender, ui16 code, ui32 data)
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
    }
};

CApplication app;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{    
    app.Create();
    app.SetFocus();
    app.WindowMessage( CWnd::WmPaint );
    
    BIOS::KEY::EKey key;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
    {
        if (key != BIOS::KEY::None)
            app.WindowMessage(CWnd::WmKey, key);
        app.WindowMessage(CWnd::WmTick);
    }
    
    app.Destroy();
    return 0;
}


uint8_t CHAR6x8[] = {
    0x00, 0x00, 0xFA, 0xFA, 0x00, 0x00,    //!
    0x00, 0x00, 0xE0, 0x00, 0xE0, 0x00,    //"
    0x00, 0x28, 0xFE, 0x28, 0xFE, 0x28,    //#
    //  0x00, 0x24, 0x74, 0xD6, 0xD6, 0x4C,    //$
    0x00, 0x0C, 0x06, 0x0E, 0x18, 0x30,    //$
    0x00, 0x66, 0x6C, 0x18, 0x36, 0x66,    //%
    0x00, 0x4C, 0xFE, 0x92, 0x9E, 0x6A,    //&
    0x00, 0x00, 0x20, 0xE0, 0xC0, 0x00,    // '
    0x00, 0x00, 0x38, 0x7C, 0xC6, 0x82,    //(
    0x00, 0x00, 0x82, 0xC6, 0x7C, 0x38,    //)
    0x00, 0x54, 0x7C, 0x38, 0x7C, 0x54,    //*
    0x00, 0x10, 0x10, 0x7C, 0x10, 0x10,    //+
    0x00, 0x00, 0x01, 0x07, 0x06, 0x00,    //,
    0x00, 0x10, 0x10, 0x10, 0x10, 0x10,    //-
    0x00, 0x00, 0x00, 0x06, 0x06, 0x00,    //.
    0x00, 0x06, 0x0C, 0x18, 0x30, 0x60,    // /
    0x00, 0x7C, 0xFE, 0x82, 0xFE, 0x7C,    //0
    0x00, 0x00, 0x42, 0xFE, 0xFE, 0x02,    //1
    0x00, 0x46, 0xCE, 0x9A, 0xF2, 0x66,    //2
    0x00, 0x44, 0xC6, 0x92, 0xFE, 0x6C,    //3
    0x00, 0x18, 0x28, 0x48, 0xFE, 0xFE,    //4
    0x00, 0xE2, 0xA2, 0xA2, 0xBE, 0x9C,    //5
    0x00, 0x7C, 0xFE, 0xA2, 0xBE, 0x1C,    //6
    0x00, 0x80, 0x8E, 0xBE, 0xF0, 0xC0,    //7
    0x00, 0x6C, 0xFE, 0x92, 0xFE, 0x6C,    //8
    0x00, 0x70, 0xFA, 0x8A, 0xFE, 0x7C,    //9
    0x00, 0x00, 0x00, 0x24, 0x24, 0x00,    //:
    0x00, 0x00, 0x01, 0x27, 0x26, 0x00,    //;
    //  0x00, 0x10, 0x38, 0x6C, 0xC6, 0x82,    //<
    0x00, 0x18, 0x3C, 0x7E, 0xFF, 0x00,    //
    0x00, 0x28, 0x28, 0x28, 0x28, 0x28,    //=
    //  0x00, 0x82, 0xC6, 0x6C, 0x38, 0x10,    //>
    0x00, 0xFF, 0x7E, 0x3C, 0x18, 0x00,    //
    0x00, 0x40, 0xCA, 0x9A, 0xF0, 0x60,    //?
    0x00, 0x7C, 0xFE, 0x82, 0xB2, 0x74,    //@
    0x00, 0x3E, 0x7E, 0xC8, 0x7E, 0x3E,    //A
    0x00, 0xFE, 0xFE, 0x92, 0xFE, 0x6C,    //B
    0x00, 0x7C, 0xFE, 0x82, 0xC6, 0x44,    //C
    0x00, 0xFE, 0xFE, 0x82, 0xFE, 0x7C,    //D
    0x00, 0xFE, 0xFE, 0x92, 0x92, 0x92,    //E
    0x00, 0xFE, 0xFE, 0x90, 0x90, 0x90,    //F
    0x00, 0x7C, 0xFE, 0x82, 0x92, 0x5C,    //G
    0x00, 0xFE, 0xFE, 0x10, 0xFE, 0xFE,    //H
    0x00, 0x82, 0xFE, 0xFE, 0x82, 0x00,    //I
    0x00, 0x04, 0x82, 0xFE, 0xFC, 0x80,    //J
    0x00, 0xFE, 0xFE, 0x38, 0xEE, 0xC6,    //K
    0x00, 0xFE, 0xFE, 0x02, 0x02, 0x02,    //L
    0x00, 0xFE, 0x70, 0x3C, 0x70, 0xFE,    //M
    0x00, 0xFE, 0xFE, 0x70, 0x38, 0xFE,    //N
    0x00, 0x7C, 0xFE, 0x82, 0xFE, 0x7C,    //O
    0x00, 0xFE, 0xFE, 0x88, 0x88, 0x70,    //P
    0x00, 0x78, 0xFC, 0x86, 0xFE, 0x7A,    //Q
    0x00, 0xFE, 0xFE, 0x98, 0xFC, 0x66,    //R
    0x00, 0x64, 0xF6, 0x92, 0xDE, 0x4C,    //S
    0x80, 0x80, 0xFE, 0xFE, 0x80, 0x80,    //T
    0x00, 0xFC, 0xFE, 0x02, 0xFE, 0xFC,    //U
    0x00, 0xF8, 0xFC, 0x06, 0xFC, 0xF8,    //V
    0x00, 0xFE, 0x1C, 0x78, 0x1C, 0xFE,    //W
    0x00, 0xC6, 0xEE, 0x38, 0xEE, 0xC6,    //X
    0x00, 0xE0, 0xF2, 0x1E, 0xF2, 0xE0,    //Y
    0x00, 0xC6, 0x8E, 0x9A, 0xF2, 0xE6,    //Z
    //  0x00, 0x00, 0xFE, 0x82, 0x82, 0x00,    //[
    0xFF, 0x83, 0x01, 0x7D, 0x01, 0x83,    //0
    0x00, 0x60, 0x30, 0x18, 0x0C, 0x06,    //  "\"
    //  0x00, 0x00, 0x82, 0x82, 0xFE, 0x00,    //]
    0xFF, 0xFF, 0xBD, 0x01, 0x01, 0xFD,    //1
    //  0x00, 0x30, 0x60, 0xC0, 0x60, 0x30,    //^
    0x00, 0x00, 0x2A, 0x2C, 0x28, 0x38,    //^
    0x00, 0x02, 0x02, 0x02, 0x02, 0x02,    //_
    //  0x00, 0x40, 0x70, 0x30, 0x00, 0x00,    //`
    0x28, 0x68, 0xA8, 0x28, 0x00, 0x00,    //`
    0x00, 0x04, 0x2E, 0x2A, 0x2A, 0x1E,    //a
    0x00, 0x7E, 0x7E, 0x12, 0x1E, 0x0C,    //b
    0x00, 0x1C, 0x3E, 0x22, 0x22, 0x22,    //c
    0x00, 0x0C, 0x1E, 0x12, 0x7E, 0x7E,    //d
    0x00, 0x1C, 0x3E, 0x2A, 0x3A, 0x18,    //e
    0x00, 0x10, 0x3E, 0x7E, 0x50, 0x50,    //f
    0x00, 0x1A, 0x3D, 0x25, 0x3F, 0x1E,    //g
    0x00, 0x7E, 0x7E, 0x10, 0x1E, 0x0E,    //h
    0x00, 0x00, 0x12, 0x5E, 0x5E, 0x02,    //i
    0x00, 0x00, 0x01, 0x01, 0x5F, 0x5E,    //j
    0x00, 0x7E, 0x7E, 0x08, 0x1C, 0x36,    //k
    0x00, 0xFC, 0xFE, 0x02, 0x02, 0x00,    //l
    0x00, 0x3E, 0x30, 0x1E, 0x30, 0x3E,    //m
    0x00, 0x3E, 0x3E, 0x20, 0x3E, 0x1E,    //n
    0x00, 0x1C, 0x3E, 0x22, 0x3E, 0x1C,    //o
    0x00, 0x3F, 0x3F, 0x24, 0x3C, 0x18,    //p
    0x00, 0x18, 0x3C, 0x24, 0x3F, 0x3F,    //q
    0x00, 0x3E, 0x3E, 0x10, 0x20, 0x20,    //r
    0x00, 0x14, 0x3A, 0x2A, 0x2E, 0x04,    //s
    0x00, 0x20, 0x7C, 0x7E, 0x22, 0x02,    //t
    0x00, 0x3C, 0x3E, 0x02, 0x3E, 0x3E,    //u
    0x00, 0x38, 0x3C, 0x06, 0x3C, 0x38,    //v
    0x00, 0x3E, 0x0C, 0x18, 0x0C, 0x3E,    //w
    0x00, 0x22, 0x36, 0x1C, 0x36, 0x22,    //x
    0x00, 0x39, 0x3D, 0x05, 0x3F, 0x3E,    //y
    0x00, 0x32, 0x26, 0x2E, 0x3A, 0x36,    //z
    0x09, 0x19, 0xFF, 0x98, 0x88, 0x00,    //
    0x00, 0x00, 0x00, 0xFE, 0x00, 0x00,    //|
    //  0x00, 0x00, 0x2A, 0x2C, 0x28, 0x38,    //{
    //  0x28, 0x68, 0xA8, 0x28, 0x00, 0x00,    //|
    0x90, 0x98, 0xFF, 0x19, 0x11, 0x00,    //
    0x00, 0x30, 0x60, 0x30, 0x18, 0x30,    //~
};
uint8_t getfont68(char c, int i) { return CHAR6x8[(c-'!')*6+i];}
