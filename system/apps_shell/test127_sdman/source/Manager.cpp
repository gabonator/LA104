#include "Manager.h"
#include "platform.h"
#ifndef __APPLE__
#include "spi.h"
#include "sd.h"
#else
class CSpi {};
class CSd { public:
    FILE* f;
    CSd(CSpi&) {
        f = fopen("/Users/gabrielvalky/Documents/git/LA104/system/apps_shell/test127_sdman/image/test2.img", "r");
    }
    int GetSectors()
    {
        return 50*1024*1024/512;
    }
    bool readSector(uint8_t* buffer, uint64_t sector)
    {
        fseek(f, sector*512, 0);
        fread(buffer, 512, 1, f);
        return true;
    }
    const char* GetCardType()
    {
        return "SD";
    }
    bool init()
    {
        return true;
    }
};
#endif
#include "fat.h"

CString m_arrPathData[5];
CString m_arrItemsData[64];
CString m_arrDetailsData[20];

uint8_t mSharedSector[512];
CSpi mSpi;
CSd mSd(mSpi);
CFat mFat(mSd, mSharedSector);
CFat32 mFat32(mSd, mSharedSector);

class CLayoutFat32 : public CLayout
{
    CArray<CString> m_arrPath;
    CArray<CString> m_arrItems;

public:
    void Reset(char* id)
    {
        m_arrPath.RemoveAll();
        m_arrPath.Add(id);
    }
    
    CLayoutFat32()
    {
        m_arrPath.Init(m_arrPathData, COUNT(m_arrPathData));
        m_arrItems.Init(m_arrItemsData, COUNT(m_arrItemsData));
    }
    virtual const CArray<CString>& GetPath() override
    {
        return m_arrPath;
    }
    virtual const CArray<CString>& GetElements() override
    {
        m_arrItems.RemoveAll();
        static CArray<CString>* _m_arrItems;
        _m_arrItems = &m_arrItems;

        ForEachFile([](direntry_t& entry, char* name)
        {
            if (_m_arrItems->GetSize() >= _m_arrItems->GetMaxSize()-1)
                return false;
            _m_arrItems->Add(name);
            return true;
        });
        m_arrItems.Sort([](CString& a, CString& b)
        {
            bool folderA = a[32] == 'F';
            bool folderB = b[32] == 'F';
            if (folderA != folderB)
                return folderA - folderB;
            return strcmp(b, a);
            
        });
        return m_arrItems;
    }
    void ForEachFile(bool (*process)(direntry_t&, char* name))
    {
        char longName[40];
        static char* _longName;
        longName[0] = 0;
        _longName = longName;
        static bool (*_process)(direntry_t&, char* name);
        _process = process;

        mFat32.ListFiles([](direntry_t& entry)
        {
            char msg[42];

            if (entry.IsLfn())
            {
                if (entry.IsLfnLast())
                {
                    entry.GetName(_longName);
                    return true;
                }
                char temp[16];
                entry.GetName(temp);
                int ofs = strlen(temp);
                for (int i=38; i>=ofs; i--)
                    _longName[i] = _longName[i-ofs];
                for (int i=0; i<ofs; i++)
                    _longName[i] = temp[i];
                _longName[39] = 0;
                return true;
            }
            if (!entry.IsFile() && !entry.IsDirectory())
                return true;
            if (_longName[0])
            {
                strcpy(msg, _longName);
                _longName[0] = 0;
            } else {
                entry.GetName(msg);
                if (strcmp(msg, ".") == 0 || strcmp(msg, "..") == 0)
                    return true;
            }
            if (strlen(msg) < 16)
            {
                int i;
                for (i=strlen(msg); i<16; i++)
                    msg[i] = ' ';
                msg[i++] = '|';
                msg[i++] = 0;
                // 5bits, 4bits, 7 bits; 5 bits, 6 bits, 5 bits
                sprintf(msg+i-1, "%02d/%02d/%02d|%02d:%02d",
                        entry.CrtDate & 31, (entry.CrtDate >> 5) & 15, (1980+(entry.CrtDate >> 9)) % 100,
                        entry.CrtTime >> 11, (entry.CrtTime >> 5) & 63);
            }
            msg[31] = 0;
            int i=strlen(msg);
            for (; i<31; i++)
                msg[i] = ' ';
            msg[i++] = '|';
            msg[i++] = 0;
            if (entry.IsDirectory())
                strcat(msg, "Folder");
            else
            {
                char temp[8];
                if (entry.FileSize <= 999999)
                    sprintf(temp, "%d", entry.FileSize);
                else if (entry.FileSize/1024 <= 9999)
                    sprintf(temp, "%d K", entry.FileSize/1024); // 9999 K
                else
                    sprintf(temp, "%d M", entry.FileSize/1024/1024); // 9999 M
                for (int i=strlen(temp); i<6; i++)
                    strcat(msg, " ");
                strcat(msg, temp);
            }
            return _process(entry, msg);
        });
    }
    
    virtual bool HasDetails() override
    {
        return false;
    }
    [[noreturn]]  virtual const CArray<CString>& GetDetails(int row) override
    {
        _ASSERT(0);
        while(1);
    }
    virtual void DrawElement(const CRect& rc, int index, bool focus) override
    {
        bool showDelim1 = true;
        bool showDelim2 = true;
        bool showDelim3 = true;
        if (index < 0)
            GUI::Background(rc, RGB565(101010), RGB565(404040));
        
        if (index >= 0)
        {
            char temp[40];
            strcpy(temp, m_arrItems[index]);
            if ((showDelim1 = (temp[16] == '|')))
                temp[16] = 0;
            if ((showDelim2 = (temp[25] == '|')))
                temp[25] = focus ? ' ' : 0;
            if ((showDelim3 = (temp[31] == '|')))
                temp[31] = 0;
            bool folder = temp[32] == 'F';
            if (focus)
            {
                BIOS::LCD::Bar(rc, RGB565(ffffff));
                BIOS::LCD::Print(rc.left+4, rc.top, RGB565(000000), RGB565(ffffff), temp);
                if (showDelim1)
                    BIOS::LCD::Print(rc.left+4+17*8, rc.top, RGB565(d0d0d0), RGB565(ffffff), temp+17);
                if (showDelim3)
                    BIOS::LCD::Print(rc.left+4+32*8, rc.top, RGB565(000000), RGB565(ffffff), temp+32);
            } else {
                GUI::Background(rc, RGB565(101010), RGB565(404040));
                BIOS::LCD::Print(rc.left+4, rc.top, folder ? RGB565(ffffff) : RGB565(b0b0b0), RGBTRANS, temp);
                if (showDelim1)
                    BIOS::LCD::Print(rc.left+4+17*8, rc.top, RGB565(404040), RGBTRANS, temp+17);
                if (showDelim2)
                    BIOS::LCD::Print(rc.left+4+26*8, rc.top, RGB565(404040), RGBTRANS, temp+26);
                if (showDelim3)
                    BIOS::LCD::Print(rc.left+4+32*8, rc.top, folder ? RGB565(ffffff) : RGB565(b0b0b0), RGBTRANS, temp+32);
            }
        }
        CRect rcLine(rc.left+255, rc.top, rc.left+256, rc.bottom);
        int clrLine = focus ? RGB565(d0d0d0) : RGB565(404040);
        if (showDelim3)
            BIOS::LCD::Bar(rcLine, clrLine);
        if (showDelim2)
        {
            rcLine.Offset(-8*6, 0);
            BIOS::LCD::Bar(rcLine, clrLine);
        }
        if (showDelim1)
        {
            rcLine.Offset(-8*9, 0);
            BIOS::LCD::Bar(rcLine, clrLine);
        }
    }

    virtual CLayout& Enter(int row) override
    {
        static const char* _element;
        _element = m_arrItems[row];
        static int _target;
        static char shortName[12];
        static char* _shortName = shortName;

        _target = -1;
        ForEachFile([](direntry_t& entry, char* name)
        {
            if (entry.IsDirectory() && strcmp(name, _element) == 0)
            {
                _target = (entry.FstClusHI<<16)|entry.FstClusLO;
                entry.GetName(_shortName);
                return false;
            }
            return true;
        });
        _ASSERT(_target != -1)
        if (_target != -1)
        {
            //mFat32.ChangeDir(_target);
            m_arrPath.Add(_shortName);
            UpdateDir();
        }
        return *this;
    }
    virtual CLayout& Leave(int& row) override
    {
        if (m_arrPath.GetSize() > 1)
            m_arrPath.RemoveLast();
        UpdateDir();
        return *this;
    }
    void UpdateDir()
    {
        mFat32.ChangeDir(2); // root
        for (int i=1; i<m_arrPath.GetSize(); i++)
        {
            static const char* _match;
            static int _target;
            _match = m_arrPath[i];
            _target = -1;
            
            mFat32.ListFiles([](direntry_t& entry)
            {
                char name[16];
                if (entry.IsDirectory() && !entry.IsLfn())
                {
                    entry.GetName(name);
                    if (strcmp(name, _match) == 0)
                    {
                        _target = (entry.FstClusHI<<16)|entry.FstClusLO;
                        return false;
                    }
                }
                return true;
            });
            _ASSERT(_target != -1);
            mFat32.ChangeDir(_target);
        }
    }
    virtual void DrawHeading(const CRect& rcRect) override
    {
        GUI::Background(rcRect, RGB565(303030), RGB565(303030));
        BIOS::LCD::Print( rcRect.left+4, rcRect.top+1, RGB565(ffff00), RGBTRANS, "     Name          Date   Time   Size");
        
        CRect rcLine(rcRect.left+255, rcRect.top, rcRect.left+256, rcRect.bottom);
        int clrLine = RGB565(404040);
        BIOS::LCD::Bar(rcLine, clrLine);
        rcLine.Offset(-8*6, 0);
        BIOS::LCD::Bar(rcLine, clrLine);
        rcLine.Offset(-8*9, 0);
        BIOS::LCD::Bar(rcLine, clrLine);
    }
};

CLayoutFat32 mLayoutFat32;

class CLayoutDetect : public CLayout
{
    CArray<CString> m_arrPath;
    CArray<CString> m_arrItems;
    CArray<CString> m_arrDetails;
    bool mGood{false};
    bool mHasMainDetails{false};
    int mType{0};
public:
    CLayoutDetect()
    {
        m_arrPath.Init(m_arrPathData, COUNT(m_arrPathData));
        m_arrItems.Init(m_arrItemsData, COUNT(m_arrItemsData));
        m_arrDetails.Init(m_arrDetailsData, COUNT(m_arrDetailsData));
    }
    virtual const CArray<CString>& GetPath() override
    {
        m_arrPath.RemoveAll();
        return m_arrPath;
    }
    virtual const CArray<CString>& GetElements() override
    {
        Populate();
        m_arrItems.RemoveAll();
        if (mGood)
        {
            m_arrItems.Add(mSd.GetCardType());
            for (int i=0; i<4; i++)
            {
               char tmp[40];
               CFat::partition_t partition;
               mFat.GetPartition(i, partition);
               if (partition.Type)
               {
                   sprintf(tmp, "Part%d: %s", i, partition.GetType());
                   m_arrItems.Add(tmp);
               }
            }
        } else
        {
            m_arrItems.Add("No card");
        }
        return m_arrItems;
    }
    virtual bool HasDetails() override
    {
        return true;
    }
    virtual const CArray<CString>& GetDetails(int row) override
    {
        if (row == 0)
        {
            if (!mHasMainDetails)
                Populate();
            return m_arrDetails;
        }
        // reload MBR
        m_arrDetails.RemoveAll();
        mFat.CheckMbr();
        
        m_arrDetails.RemoveAll();
        mHasMainDetails = false;
        CFat::partition_t partition;
        mFat.GetPartition(row-1, partition);

        char msg[40];
        sprintf(msg, "Partition %d", row-1);
        m_arrDetails.Add(msg);
        m_arrDetails.Add("");
        sprintf(msg, "Type: %s (0x%02x)", partition.GetType(), partition.Type);
        m_arrDetails.Add(msg);
        sprintf(msg, "Range: %d - %d", partition.FirstSectorOffset, partition.FirstSectorOffset+partition.NumSectors);
        m_arrDetails.Add(msg);

        sprintf(msg, "Size: ");
        partition.FormatSize(msg+strlen(msg));
        m_arrDetails.Add(msg);
        switch (partition.State)
        {
          case 0x00:
            sprintf(msg, "Boot: No");
            break;
          case 0x80:
            sprintf(msg, "Boot: Yes");
            break;
          default:
            sprintf(msg, "Boot: Invalid (0x%02x)", partition.State);
        }
        m_arrDetails.Add(msg);

        PopulatePartition(partition);
        return m_arrDetails;
    }
    virtual CLayout& Enter(int row) override
    {
        if (row == 0)
            return *this;

        CFat::partition_t partition;
        mFat.CheckMbr();
        mFat.GetPartition(row-1, partition);
        switch (partition.Type)
        {
            case CFat::PartitionType::Fat16:
            case CFat::PartitionType::Fat32:
            {
                char id[8];
                sprintf(id, "SD%d:", row-1);
                mFat32.SetRange(partition.FirstSectorOffset, partition.NumSectors);
                mLayoutFat32.Reset(id);
                return mLayoutFat32;
            }
            default:
                return *this; // no support yet
        }
    }
    virtual CLayout& Leave(int& row) override
    {
        return *this;
    }
    void Populate()
    {
        mHasMainDetails = true;

        m_arrDetails.RemoveAll();
        static CArray<CString>* _m_arrDetails;
        _m_arrDetails = &m_arrDetails;
        static bool _anyMsg;
        _anyMsg = false;
        Platform::redirect = [](const char* msg)
        {
           // assert(strlen(msg) < sizeof(CString)-1);
            _m_arrDetails->Add(msg);
            _anyMsg = true;
        };
        
        if (mSd.init())
        {
            char msg[40];
            sprintf(msg, "Card detected: %s", mSd.GetCardType());
            m_arrDetails.InsertAt(0, msg);
            sprintf(msg, "Sectors: %d", mSd.GetSectors());
            m_arrDetails.InsertAt(1, msg);
            //int sizeGb10 = mSd.GetSectors()*10/2/1000/1000;
            CFat::partition_t temp;
            temp.NumSectors = mSd.GetSectors();
            sprintf(msg, "Capacity: "); //%d.%d GB", sizeGb10/10, sizeGb10%10);
            temp.FormatSize(msg+strlen(msg));
            m_arrDetails.InsertAt(2, msg);
            m_arrDetails.InsertAt(3, "");
            if (mFat.CheckMbr())
                mGood = true;
            else
                m_arrDetails.InsertAt(4, "No Master boot record!");
        } else {
            m_arrDetails.InsertAt(0, "No card detected!");
            m_arrDetails.InsertAt(1, "");
            if (_anyMsg)
                m_arrDetails.InsertAt(2, "----------------------");
            if (_anyMsg)
                m_arrDetails.Add("----------------------");
            m_arrDetails.Add((char*)"");
            m_arrDetails.Add((char*)"Check your wiring:");
            m_arrDetails.Add((char*)"  P1: MOSI");
            m_arrDetails.Add((char*)"  P2: MISO");
            m_arrDetails.Add((char*)"  P3: SCK");
            m_arrDetails.Add((char*)"  P4: CS");
            mGood = false;
        }
        Platform::redirect = nullptr;
    }
    void PopulatePartition(CFat::partition_t& partition)
    {
        static CArray<CString>* _m_arrDetails;
        _m_arrDetails = &m_arrDetails;
        Platform::redirect = [](const char* msg)
        {
            _m_arrDetails->Add(msg);
        };

        if (partition.Type == CFat::PartitionType::Fat32 ||
            partition.Type == CFat::PartitionType::Fat16)
        {
            mFat32.SetRange(partition.FirstSectorOffset, partition.NumSectors);
            mFat32.Check();
        }
        
        Platform::redirect = nullptr;
    }
    virtual void DrawElement(const CRect& _rc, int index, bool focus) override
    {
        CRect rc(_rc.left, _rc.top, _rc.left + 120, _rc.bottom);
        CRect rcLine(3, rc.top, 4, rc.bottom);
        rcLine.Offset(13*8+4, 0);
        if (index < 0)
        {
            GUI::Background(rc, RGB565(101010), RGB565(404040));
            BIOS::LCD::Bar(rcLine, RGB565(404040));
            return;
        }
        if (focus)
        {
            BIOS::LCD::Bar(rc, RGB565(ffffff));
            BIOS::LCD::Print(rc.left+4, rc.top, RGB565(000000), RGB565(ffffff), m_arrItems[index]);
        } else {
            GUI::Background(rc, RGB565(101010), RGB565(404040));
            BIOS::LCD::Print(rc.left+4, rc.top, RGB565(ffffff), RGBTRANS, m_arrItems[index]);
        }
        BIOS::LCD::Bar(rcLine, RGB565(404040));
    }
    virtual void DrawHeading(const CRect& rcRect) override
    {
        GUI::Background(rcRect, RGB565(303030), RGB565(303030));
        BIOS::LCD::Print( rcRect.left+4, rcRect.top+1, RGB565(ffff00), RGBTRANS, "     Name             Details");
        CRect rcLine(3, rcRect.top, 4, rcRect.bottom);
        rcLine.Offset(13*8+4, 0);
        BIOS::LCD::Bar(rcLine, RGB565(404040));
    }
    bool Changed()
    {
        Platform::redirect = [](const char* msg) {};
        bool nowGood = mSd.init();
        Platform::redirect = nullptr;
        return nowGood != mGood;
    }
};

CLayoutDetect mLayoutDetect;
//CLayout* pLayout = &mLayoutDetect;
//const CArray<CString>* mpItems = nullptr;

const char CShapes_sel_left[] =
"\x0e"
"              "
"              "
"              "
"              "
"  ..........  "
" ............ "
".............."
"..............";

const char CShapes_sel_right[] =
"\x0e"
".............."
".............."
" ............ "
"  ..........  "
"              "
"              "
"              "
"              ";

const char CShapes_sel_right_inv[] =
"\x0e"
"              "
".            ."
"..          .."
"..............";

int _min(int a, int b)
{
    return a<b?a:b;
}


CWndManager::CWndManager()
{
}
void CWndManager::Create(CWnd *pParent, ui16 dwFlags)
{
    CWnd::Create("Manager", dwFlags, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), pParent);
    SetTimer(250);
    mPosition.Init(mPositionData, COUNT(mPositionData));
    mPosition.Add(Position{0, 0});
    Select(mLayoutDetect);
}

void CWndManager::Select(CLayout& layout)
{
    mpLayout = &layout;
    mpItems = &mpLayout->GetElements();
}

void CWndManager::OnPaint()
{
    CRect topBar(m_rcClient.left, m_rcClient.top, m_rcClient.left+100, m_rcClient.top + 14);
    GUI::Background(topBar, RGB565(4040b0), RGB565(404040));
    topBar.right = m_rcClient.right;
    int x = 8;
    x += BIOS::LCD::Print(x, topBar.top, RGB565(ffffff), RGBTRANS, "SD Browser ");
    x += BIOS::LCD::Draw(x, 0, RGB565(404040), RGBTRANS, CShapes_sel_right_inv);
    topBar.left = x;
    GUI::Background(topBar, RGB565(404040), RGB565(404040));

    x += 8;
    /*
    auto insert = [](char* str, const char* ins)
    {
        int l = strlen(ins);
        for (int i=strlen(str); i >= 0; i--)
            str[i+l] = str[i];
        memcpy(str, ins, l);
    };
    char tempPath[40] = {0};
    int avail = BIOS::LCD::Width - 8;
    for (int i=path.GetSize()-1; i >= 0; i--)
    {
        const CString& e = path[i];
        if ((int)strlen(e)*8+16 <= avail)
        {
            avail -= strlen(e)*8+16;
            if (i==path.GetSize()-1)
            {
                insert(tempPath, "}");
                insert(tempPath, e);
                insert(tempPath, "{");
            } else {
                insert(tempPath, ">");
                insert(tempPath, e);
                insert(tempPath, "<");
            }
            continue;
        }
    }
    
    bool last = false;
    for (int i=0; tempPath[i]; i++)
    {
        char c = tempPath[i];
        if (c == '{')
        {
            x += BIOS::LCD::Draw( x, 0, RGB565(606060), RGBTRANS, CShapes_sel_left);
            last = true;
        }
        else if (c == '}')
        {
            x += BIOS::LCD::Draw( x, 0, RGB565(606060), RGBTRANS, CShapes_sel_right);
            last = false;
        }
        else if (c == '<')
            x += BIOS::LCD::Draw( x, 0, RGB565(606060), RGBTRANS, CShapes_sel_left);
        else if (c == '>')
            x += BIOS::LCD::Draw( x, 0, RGB565(606060), RGBTRANS, CShapes_sel_right);
        else
            x += BIOS::LCD::Print( x, 0, last ? RGB565(ffffff) : RGB565(b0b0b0), RGB565(606060), c);
    }
*/
    const CArray<CString>& path = mpLayout->GetPath();
    for (int i=0; i<path.GetSize(); i++)
    {
        x += BIOS::LCD::Draw( x, 0, RGB565(606060), RGBTRANS, CShapes_sel_left);
        x += BIOS::LCD::Print(x, 0, i==path.GetSize()-1 ? RGB565(ffffff) : RGB565(b0b0b0), RGB565(606060), path[i]);
        x += BIOS::LCD::Draw( x, 0, RGB565(606060), RGBTRANS, CShapes_sel_right);
    }
    int y = 14;
    CRect rcHeading(0, y, BIOS::LCD::Width, y+16);
    mpLayout->DrawHeading(rcHeading);
    y += 14;
    _ASSERT(mpItems);
    const CArray<CString>& elements = *mpItems;
    //const CArray<CString>& elements = mpLayout->GetElements();
    for (int i = 0; i < MaxLines; i++, y+=14)
        DrawLine(i, i==mPosition.GetLast().mIndex, i >= elements.GetSize());
    if (mpLayout->HasDetails())
        DrawDetails();
    
    DrawProgress();
}
void CWndManager::OnTimer()
{
    if (mpLayout == &mLayoutDetect)
    {
        if (mLayoutDetect.Changed())
            Invalidate();
    }
}
void CWndManager::OnMessage(CWnd* pSender, int code, uintptr_t data)
{
    
}
void CWndManager::OnKey(int nKey)
{
    Position& cur = mPosition.GetLast();
    if ( nKey == BIOS::KEY::Down )
    {
        if (cur.mOffset + cur.mIndex + 1 < mpItems->GetSize() )
        {
            if (cur.mIndex + 1 >= MaxLines)
            {
                cur.mOffset++;
                Invalidate();
            }
            else
            {
                DrawLine( cur.mIndex, false );
                cur.mIndex++;
                DrawLine( cur.mIndex, true );
                if (mpLayout->HasDetails())
                    DrawDetails();
            }
        }
    }
    if ( nKey == BIOS::KEY::Up )
    {
        if (cur.mOffset + cur.mIndex > 0)
        {
            if (cur.mIndex == 0)
            {
                cur.mOffset--;
                Invalidate();
            }
            else
            {
                DrawLine( mPosition.GetLast().mIndex, false );
                cur.mIndex--;
                DrawLine( mPosition.GetLast().mIndex, true );
                if (mpLayout->HasDetails())
                    DrawDetails();
            }
        } else
        {
            nKey = BIOS::KEY::Escape;
        }
    }
    if ( nKey == BIOS::KEY::Enter )
    {
        Select(mpLayout->Enter(mPosition.GetLast().mIndex));
        cur.mIndex = 0;
        cur.mOffset = 0;
        Invalidate();
    }
    if ( nKey == BIOS::KEY::Escape )
    {
        int row = 0;
        Select(mpLayout->Leave(row));
        cur.mIndex = row;
        if (cur.mIndex > MaxLines)
        {
            cur.mOffset = cur.mIndex - (MaxLines-1);
            cur.mIndex = MaxLines-1;
        }

        Invalidate();
    }

}

void CWndManager::DrawProgress()
{
    // update scroll pos
    CRect rcProgress(BIOS::LCD::Width-8, m_rcClient.top+14+16, BIOS::LCD::Width, m_rcClient.top +16+ (1+MaxLines)*14);
    GUI::Background(rcProgress, RGB565(101010), RGB565(404040));
//    BIOS::LCD::Bar( rcProgress, RGB565(0000b0) );
    int nPercentTop = 0;
    int nPercentBottom = 1024;
    ui16 clr = RGB565(808080);
    int entries = mpItems->GetSize();
    int nScroll = mPosition.GetLast().mOffset;
    if ( entries > MaxLines )
    {
        nPercentTop = nScroll * 1024 / entries;
        nPercentBottom = (nScroll + MaxLines) * 1024 / entries;
        clr = RGB565(ffffff);
    }
    CRect rcRange;
    rcRange.left = rcProgress.left + 2;
    rcRange.right = rcProgress.right - 2;
    rcRange.top = rcProgress.top + rcProgress.Height() * nPercentTop / 1024;
    rcRange.bottom = rcProgress.top + rcProgress.Height() * nPercentBottom / 1024;
    BIOS::LCD::Bar( rcRange, clr );
}

void CWndManager::DrawLine(int i, bool bSelected, bool empty)
{
    int y = i*14+16+14;
    CRect rcBack( 0, y, m_rcClient.right-8, y+14);
    mpLayout->DrawElement(rcBack, empty ? -1 : i + mPosition.GetLast().mOffset, bSelected);
}

void CWndManager::DrawDetails()
{
    int y = 30;
    int i = 0;
    const CArray<CString>& details = mpLayout->GetDetails(mPosition.GetLast().mIndex);
    for (int j=0; j<details.GetSize(); j++)
    {
        CRect rcRect(112, y, BIOS::LCD::Width-8, y+14);
        GUI::Background(rcRect, RGB565(202020), RGB565(404040));
        if (details[j][0])
            BIOS::LCD::Print(116, y, RGB565(b0b0b0), RGBTRANS, details[j]);
        y+= 14;
        i++;
    }
    for ( ; i < MaxLines; i++)
    {
        CRect rcRect(112, y, BIOS::LCD::Width-8, y+14);
        GUI::Background(rcRect, RGB565(202020), RGB565(404040));
        y+= 14;
    }
}

