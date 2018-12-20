#include <library.h>
#include "../../os_host/source/framework/BufferedIo.h"
#include "shapes.h"
#include "../../os_host/source/framework/BufferedIo.h"
#include "../../os_host/source/framework/Serialize.h"
//#include "../../os_host/source/framework/Serialize.cpp"

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
    reader.Open(path);
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
            int c0 = LCD::GetPixel(bx+x, by+header.dwBiHeight-1-y);
            int c1 = InterpolateColor(c0, c, color[3]);
            LCD::PutPixel(bx+x, by+header.dwBiHeight-1-y, c1);
        }
        
    int f = 9;
    f++;

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

class CMenuMain : public CTopMenu
{
public:
    virtual TItem GetItem(int i)
    {
        switch (i)
        {
            case 0: return TItem{"LA104", TItem::Static};
            case 1: return TItem{"Analysers", TItem::Default};
            case 2: return TItem{"Default", TItem::Default};
            case 3: return TItem{"Games", TItem::Default};
            default: return TItem{nullptr, TItem::None};
        }
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

uint32_t icon[] ={
    0b01111111111111111111111111111110,
    0b10000000000000000000000000000001,
    0b10000111110001111111100111100001,
    0b10000100010001000000100100100001,
    0b10000100010001000000100100100001,
    0b10000100010001000000100100100001,
    0b10000100010001000000100100100001,
    0b10111100011111000000111100111101,//
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000111110001111111100111100001,
    0b10000100010001000000100100100001,
    0b10000100010001000000100100100001,
    0b10000100010001000000100100100001,
    0b10000100010001000000100100100001,
    0b10111100011111000000111100111101,//
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000111110001111111100111100001,
    0b10000100010001000000100100100001,
    0b10000100010001000000100100100001,
    0b10000100010001000000100100100001,
    0b10000100010001000000100100100001,
    0b10111100011111000000111100111101,//
    0b10000000000000000000000000000001,
    0b10000111110001111111100111100001,
    0b10000100010001000000100100100001,
    0b10000100010001000000100100100001,
    0b10000100010001000000100100100001,
    0b10111100011111000000111100111101,
    0b10000000000000000000000000000001,
    0b01111111111111111111111111111110,
};

class CApplication : public CWnd
{
    CMenuMain mMenu;
    // Tab1
    
    // Tab2
    
public:
    void Create()
    {
        CWnd::Create("Application", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
        mMenu.Create("MainMenu", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, 14), this);
        /*
        constexpr int base = 15;
        constexpr int width = 65;
        constexpr int space = 10;
        int _x = base;
        constexpr int _y = 22;
        
        mPlay.Create("Burst", CWnd::WsVisible, CRect(_x, _y, _x+width, _y+16), this);
        _x += space + width;
        mLoop.Create("Loop", CWnd::WsVisible, CRect(_x, _y, _x+width, _y+16), this);
        _x += space + width;
        mFollow.Create("Follow", CWnd::WsVisible, CRect(_x, _y, _x+width, _y+16), this);

     
         
         
         mSequencer.Create("Player", CWnd::WsVisible, CRect(0, 14+32, BIOS::LCD::Width, BIOS::LCD::Height), this);
         */
    }

    void DrawIcon(int bx, int by, uint32_t* buf, bool on = false)
    {
        CRect rcIcon(bx-4, by-4, bx+64+4, by+64+4);
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
                int dis = min(y-rcIcon.top, rcIcon.bottom-y);
                if (dis<8)
                {
                    sub.left += shift[dis];
                    sub.right -= shift[dis];
                }
                GUI::Background(sub, RGB565RGB(39, 101, 217), RGB565RGB(39, 101, 217));
            }
        }
        if (!on)
            DrawImage((char*)ROOT "folder.bmp", bx, by);
        else
            DrawImage((char*)ROOT "out.bmp", bx, by);
        if (0&&!on)
        for (int y =0; y<64; y++)
            for (int x=0; x<64; x++)
                if ((buf[y/2] & (1<<x/2)))
                {
                    LCD::PutPixel(bx+x, by+y, RGB565(ffffff));
                    LCD::PutPixel(bx+x+1, by+y, RGB565(b0b0b0));
                    LCD::PutPixel(bx+x, by+y+1, RGB565(b0b0b0));
                    LCD::PutPixel(bx+x+1, by+y+1, RGB565(b0b0b0));
                }
        if (on)
        {
        const char* msg1 ="Logical analysers";
        const char* msg2 ="and sequencers";
        if (on)
        BIOS::LCD::RoundRect(bx+32-strlen(msg1)*3-6, by+64+8-1, bx+32+strlen(msg1)*3+6, by+64+8*2+10, on ? RGB565RGB(39, 101, 217) : RGB565(808080));
        Print(bx+32-strlen(msg1)*3, by+64+8, msg1, RGB565(ffffff));
        Print(bx+32-strlen(msg2)*3, by+64+8+9, msg2, RGB565(ffffff));
        } else{
            const char* msg1 ="Logical";
            const char* msg2 ="analysers";
            if (on)
                BIOS::LCD::RoundRect(bx+32-strlen(msg1)*3-6, by+64+8-1, bx+32+strlen(msg1)*3+6, by+64+8*2+10, on ? RGB565RGB(39, 101, 217) : RGB565(808080));
            Print(bx+32-strlen(msg1)*3, by+64+8, msg1, RGB565(ffffff));
            Print(bx+32-strlen(msg2)*3, by+64+8+9, msg2, RGB565(ffffff));

        }
    }
    
    void Print(int bx, int by, const char* msg, uint16_t color)
    {
        while (*msg)
        {
            Printch(bx, by, *msg++, color);
            bx += 6;
        }
    }
    
    void Printch(int bx, int by, char ch, uint16_t c)
    {
        if (ch == ' ')
            return;
        for (int y=0; y<8; y++)
            for (int x=0; x<6; x++)
                if (getfont68(ch, x) & (128>>y))
                    LCD::PutPixel(bx+x, by+y, c);

    }
    
    virtual void OnPaint()
    {
        CRect rcTop(m_rcClient);
        rcTop.bottom = rcTop.top + 32;
        //GUI::Background(m_rcClient, RGB565(808080), RGB565(404040));
        GUI::Background(m_rcClient, RGB565(b0b0b0), RGB565(808080));
        
        DrawIcon(20, 30, icon);
        DrawIcon(20+90, 30, icon);
        DrawIcon(20+90*2, 30, icon, true);
        DrawIcon(20, 50+90, icon);
        DrawIcon(20+90, 50+90, icon);
        DrawIcon(20+90*2, 50+90, icon);
    }
    
    virtual void OnMessage(CWnd* pSender, ui16 code, ui32 data)
    {
     
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
