#include <library.h>
#include "../../os_host/source/framework/BufferedIo.h"
#include "shapes.h"
#include <gui/MessageBox.h>

using namespace BIOS;

void _HandleAssertion(const char* file, int line, const char* cond)
{
    //BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

#define BLINK(a) ((BIOS::SYS::GetTick() / a)&1)

#include "PCF8574.h"

class CPort
{
public:
    enum {
        PcfAddress1 = 0x38,
        PcfAddress2 = 0x39
    };
    
private:
    PCF8574 mPCF1{PcfAddress1};
    PCF8574 mPCF2{PcfAddress2};

    uint8_t mDataWrite[2] {0xff, 0xff};
    uint8_t mDataRead[2] {0xff, 0xff};
    
    uint32_t* mGpioStatus{nullptr};
    
public:
    enum EMode {Input, Output};
    
public:
    bool Init()
    {
        mGpioStatus = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::GpioStatus);
        _ASSERT(mGpioStatus);
        *mGpioStatus = 0;

        Wire.begin();
        mPCF1.begin();
        mPCF2.begin();

        //return BLINK(5000);
        if (*mGpioStatus == 0)
            return true;
        *mGpioStatus = 0;
        return false;
    }
    
    void Deinit()
    {
        Wire.end();
    }

    bool Sync()
    {
        mDataRead[0] = mPCF1.read8();
        mDataRead[1] = mPCF2.read8();
        mPCF1.write8(mDataWrite[0]);
        mPCF2.write8(mDataWrite[1]);
        
        //return BLINK(5000);
        if (*mGpioStatus == 0)
            return true;
        *mGpioStatus = 0;
        return false;
    }
    
    bool Read(int pin)
    {
        int port = pin / 8;
        int bit = pin % 8;
        
        return !!(mDataRead[port] & (1<<bit));
    }
    
    void Mode(int pin, EMode mode)
    {
        if (mode == Input)
            Write(pin, true); // disable NPN, 100uA weak pull up
        else
            Write(pin, false); // short to ground
    }
    
    void Write(int pin, bool level)
    {
        int port = pin / 8;
        int bit = pin % 8;
        
        if (level)
            mDataWrite[port] |= 1<<bit; // pull up
        else
            mDataWrite[port] &= ~(1<<bit); // strong
    }
};

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
            case 0: return TItem{"Sequencer", TItem::Static};
            case 1: return TItem{"Editor", TItem::Default};
//            case 2: return TItem{"Settings", TItem::Default};
//            case 3: return TItem{"File", TItem::Default};
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

static constexpr uint16_t mPalette[] = {
    RGB565(ffff00), RGB565(00ffff), RGB565(40ff40), RGB565(4040ff),
    RGB565(ff40ff), RGB565(ff4040), RGB565(ffb000), RGB565(00ffb0),
    RGB565(ff00b0)
};


class CSequencer
{
protected:
    struct TChannelInfo
    {
        enum EDirection {Disabled, Input, Output};
        
        TChannelInfo() = default;
        
        TChannelInfo(uint32_t channel, uint32_t sequence, char* _name, uint16_t color, EDirection direction) :
        channel(channel), sequence(sequence), color(color), direction(direction)
        {
            if (_name)
                strcpy(name, _name);
            else
                strcpy(name, "");
            
            colorBlend = InterpolateColor(color, RGB565(404040), 128);
        }
        
        uint32_t channel;
        uint32_t sequence;
        char name[4];
        uint_fast16_t color;
        uint_fast16_t colorBlend;
        EDirection direction;
    } mChannel[16];

    uint32_t mMask {0b11111111111111110000};
    CPort mPort;
    bool mOnline{false};
    
public:
    CSequencer()
    {
        mChannel[0] = {0, 0b01010101010101010000, (char*)"A1", mPalette[0], TChannelInfo::Output};
        mChannel[1] = {1, 0b00110011001100110000, (char*)"A2", mPalette[1], TChannelInfo::Output};
        mChannel[2] = {2, 0b00001111000011110000, (char*)"A3", mPalette[2], TChannelInfo::Output};
        mChannel[3] = {3, 0b00000000111111110000, (char*)"A4", mPalette[3], TChannelInfo::Output};
        
        mChannel[4] = {8, 0b00000000000000000000, (char*)"B1", mPalette[4], TChannelInfo::Input};
        mChannel[5] = {9, 0b00000000000000000000, (char*)"B2", mPalette[5], TChannelInfo::Input};
        mChannel[6] = {10, 0b00000000000000000000, (char*)"B3", mPalette[6], TChannelInfo::Input};
        mChannel[7] = {11, 0b00000000000000000000, (char*)"B4", mPalette[7], TChannelInfo::Input};
        mChannel[8] = {12, 0b00000000000000000000, (char*)"B5", mPalette[6], TChannelInfo::Input};
        mChannel[9] = {13, 0b00000000000000000000, (char*)"B6", mPalette[7], TChannelInfo::Input};
        mChannel[10] = {14, 0b00000000000000000000, (char*)"B7", mPalette[6], TChannelInfo::Input};

        for (int i=11; i<COUNT(mChannel); i++)
            mChannel[i] = {0, 0b00000000000000000000, nullptr, mPalette[i%COUNT(mPalette)], TChannelInfo::Disabled};
    }
    
    bool Init()
    {
        mOnline = mPort.Init();
        return mOnline;
    }
    
    void Deinit()
    {
        mPort.Deinit();
    }
    
    bool Write(int index)
    {
        if (!mOnline)
            return false;
        
        for (int i=0; i<COUNT(mChannel); i++)
        {
            TChannelInfo& channel = mChannel[i];
            if (channel.direction == TChannelInfo::Output)
            {
                mPort.Write(channel.channel, channel.sequence & (1<<(19-index)));
            }
        }
        
        mOnline = mPort.Sync();
        return mOnline;
    }
    
    bool Read()
    {
        if (!mOnline)
            return false;

        mOnline = mPort.Sync();
        for (int i=0; i<COUNT(mChannel); i++)
        {
            TChannelInfo& channel = mChannel[i];
            if (channel.direction == TChannelInfo::Input)
            {
                //TODO: current value/reading?
                channel.sequence <<= 1;
                channel.sequence |= mPort.Read(channel.channel);
            }
        }
        return mOnline;
    }
    
    bool IsOnline()
    {
        return mOnline;
    }
};

class CSequencerGui : public CSequencer, public CWnd
{
    int mCursorX{-1};
    int mCursorY{-1};
    int mScrollY{0};
    int mPlayX{0};
    int mLastDrawnStep{-1};
    bool mLoop{false};
    bool mPlay{false};
    bool mFollow{false};
    bool mFollowing{false};
    bool mNotified{false};

public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
        if (!CSequencer::Init())
            NotifyOffline();
    }
    
    void Destroy()
    {
        CSequencer::Deinit();
    }

    void DrawStep(int x, bool highlight)
    {
        if (x==-1)
            return;
        
        int _y = m_rcClient.top;
        int _x = 32+x*16;
        
        if (mCursorY == -1 && mCursorX == -1)
            mCursorX = 0;
        
        if (highlight)
            LCD::Bar(_x, _y-2, _x+16, _y+6, RGB565(ffff00));
        else if (mCursorY == -1 && mCursorX == x && GetFocus() == this)
            LCD::Bar(_x, _y-2, _x+16, _y+6, RGB565(ffffff));
        else
            LCD::Bar(_x, _y-2, _x+16, _y+6, RGB565(404040));
        
        bool enabled = mMask & (1 << (19-x));
        uint_fast16_t color = enabled ? RGB565(b0b0b0) : RGB565(505050);
        LCD::Bar(_x+2, _y, _x+14, _y+1, color);
        LCD::Bar(_x+1, _y+1, _x+15, _y+3, color);
        LCD::Bar(_x+2, _y+3, _x+14, _y+4, color);
    }
    
    void DrawRange()
    {
        LCD::Bar(0, m_rcClient.top, LCD::Width, m_rcClient.top+8, RGB565(404040));
        for (int i=0; i<18; i++)
            DrawStep(i, false);
    }

    void DrawAddChannel(int row, char* id)
    {
        int y = m_rcClient.top+row*20+8;
        const uint_fast16_t& color = mChannel[mScrollY+row].color;
        const uint_fast16_t& colorBlend = mChannel[mScrollY+row].colorBlend;
        
        if (mCursorY == row)
            LCD::Bar(0, y, 32, y+18, RGB565(ffffff));
        else
            LCD::Bar(0, y, 32, y+18, color);
        LCD::Bar(0, y+18, LCD::Width, y+20, colorBlend);
        LCD::Print(1, y+3, RGB565(000000), RGBTRANS, "Add");
        LCD::Bar(32, y, LCD::Width, y+20, RGB565(404040));
    }

    void DrawBit(int row, int x, int value)
    {
        int y = m_rcClient.top+row*20+8;
        const uint_fast16_t& color = mChannel[mScrollY+row].color;
        const uint_fast16_t& colorBlend = mChannel[mScrollY+row].colorBlend;

        LCD::BufferBegin(CRect(32+x, y, 33+x, y+20));
        if (value == 1)
        {
            uint_fast16_t buffer[20] = {RGB565(404040), RGB565(404040), color, colorBlend, colorBlend,
                colorBlend, colorBlend, colorBlend, colorBlend, colorBlend,
                colorBlend, colorBlend, colorBlend, colorBlend, colorBlend,
                colorBlend, colorBlend, colorBlend, colorBlend, colorBlend,
            };
            for (int i=COUNT(buffer)-1; i>=0; i--)
                LCD::BufferWrite(buffer[i]);
        }
        if (value == 0)
        {
            uint_fast16_t buffer[20] = {RGB565(404040), RGB565(404040), RGB565(404040), RGB565(404040), RGB565(404040),
                RGB565(404040), RGB565(404040), RGB565(404040), RGB565(404040), RGB565(404040),
                RGB565(404040), RGB565(404040), RGB565(404040), RGB565(404040), RGB565(404040),
                RGB565(404040), RGB565(404040), color, colorBlend, colorBlend
            };
            for (int i=COUNT(buffer)-1; i>=0; i--)
                LCD::BufferWrite(buffer[i]);
        }
        if (value == 2)
        {
            uint_fast16_t buffer[20] = {RGB565(404040), RGB565(404040), color, color, color,
                color, color, color, color, color,
                color, color, color, color, color,
                color, color, color, colorBlend, colorBlend,
            };
            for (int i=COUNT(buffer)-1; i>=0; i--)
                LCD::BufferWrite(buffer[i]);
        }
    }

    void DrawChannel(int row, char* id, uint8_t* sequence, int length)
    {
        int y = m_rcClient.top+row*20+8;
        const uint_fast16_t& color = mChannel[mScrollY+row].color;
        const uint_fast16_t& colorBlend = mChannel[mScrollY+row].colorBlend;

        bool highlight = row == mCursorY && (mCursorX == -1 || mChannel[mScrollY+row].direction == TChannelInfo::Input);
        
        LCD::Bar(0, y+18, LCD::Width, y+20, colorBlend);
        if (highlight)
            LCD::Bar(0, y, 32, y+18, RGB565(ffffff));
        else
            LCD::Bar(0, y, 32, y+18, color);

        LCD::Print(1, y+3, RGB565(000000), color, id);

        static const char shape_output[] =
        "\x11"
        " .... ..  .  ... "
        "  ..  ..  . ..  ."
        "  ..  ..  . ..  ."
        "  ..  ..  . ..  ."
        "  ..   ...   ... ";

        static const char shape_input[] =
        "\x11"
        "   ..   .  ....  "
        "   ..  ..   ..   "
        "   .. . .   ..   "
        "   ...  .   ..   "
        "   ..   .  ....  ";

        BIOS::LCD::Draw( 32-6, y, colorBlend, RGBTRANS, sequence ? shape_output : shape_input);
        
        if (!sequence || length <= 0)
        {
            //if (!mFollow)
                LCD::Bar(32, y, LCD::Width, y+18, RGB565(404040));
            return;
        }
        
        int prev = sequence[0];
        for (int i=0; i<length; i++)
        {
            int _x = 32 + i*16;
            int cur = sequence[i] & 1;
            int sel = mCursorY == row && i == mCursorX;
            
            if (sel)
            {
                LCD::Bar(_x, y, _x+16, y+2, RGB565(404040));
                LCD::Bar(_x, y+2, _x+16, y+20, RGB565(ffffff));
            } else
            {
                if (cur)
                    LCD::Bar(_x, y, _x+16, y+2, RGB565(404040));
                else
                    LCD::Bar(_x, y, _x+16, y+17, RGB565(404040));
            }
            if (cur)
            {
                LCD::Bar(_x, y+2, _x+16, y+3, color);
                if (sel)
                {
                    LCD::Bar(_x, y+3, _x+16, y+18, RGB565(ffffff));
                    LCD::Bar(_x, y+18, _x+16, y+20, colorBlend);
                } else {
                    LCD::Bar(_x, y+3, _x+16, y+20, colorBlend);
                }
                LCD::Print(_x+4, y+3, colorBlend - RGB565(101010), sel ? RGB565(ffffff) : colorBlend, "1");
            }
            else
            {
                LCD::Bar(_x, y+17, _x+16, y+18, color);
                LCD::Bar(_x, y+18, _x+16, y+20, colorBlend);
                LCD::Print(_x+4, y+3, RGB565(505050), sel ? RGB565(ffffff) : RGB565(404040), "0");
            }
            if (prev != cur)
            {
                LCD::Bar(_x, y+2, _x+1, y+18, color);
                prev = cur;
            }
        }
    }
    
    virtual void OnPaint() override
    {
//        int32_t t0 = SYS::GetTick();
        DrawRange();
        for (int i=0; i<9; i++)
        {
            TChannelInfo& channel = mChannel[mScrollY+i];
            
            // TODO: remove
            uint8_t sequence[20];
            for (int j=0; j<20; j++)
                sequence[j] = (channel.sequence >> (20-j-1)) & 1;
            
            switch (channel.direction)
            {
                case TChannelInfo::Disabled:
                    DrawAddChannel(i, (char*)"Add");
                    break;
                case TChannelInfo::Input:
                    DrawChannel(i, channel.name, nullptr, 0);
                    break;
                case TChannelInfo::Output:
                    DrawChannel(i, channel.name, sequence, COUNT(sequence)-2);
                    break;
            }
        }
//        int32_t t1 = SYS::GetTick();
//        char msg[32];
//        sprintf(msg, "took %d ms", t1-t0);
//        LCD::Print(160, LCD::Height-20, RGB565(ffffff), RGB565(000000), msg);
    }
    
    virtual void OnKey(int nKey) override
    {
        if (nKey == BIOS::KEY::Left && mCursorX > -1)
        {
            mCursorX--;
            Invalidate();
            if (mFollow)
                StartFollow(mCursorX);
        }
        if (nKey == BIOS::KEY::Right && mCursorX < 17)
        {
            mCursorX++;
            Invalidate();
            if (mFollow)
                StartFollow(mCursorX);
        }
        if (nKey == BIOS::KEY::Up && mCursorY == -1)
        {
            CWnd::OnKey(nKey);
            return;
        }
        if (nKey == BIOS::KEY::Up && mCursorY > -1)
        {
            if (mCursorY == 0 && mScrollY > 0)
                mScrollY--;
            else
                mCursorY--;
            Invalidate();
        }
        if (nKey == BIOS::KEY::Down && mCursorY + mScrollY < 15)
        {
            if (mCursorY < 8)
                mCursorY++;
            else
                mScrollY++;
            Invalidate();
        }
        if (nKey == BIOS::KEY::F1)
        {
            if (mCursorY >= 0 && mCursorX >= 0 && mCursorX < 20 && mChannel[mCursorY+mScrollY].direction == TChannelInfo::Output)
            {
                mChannel[mCursorY+mScrollY].sequence ^= 1<<(19-mCursorX);
                Invalidate();
                if (mFollow)
                    StartFollow(mCursorX);
            }
            if (mCursorY == -1 && mCursorX >= 0 && mCursorX < 20)
            {
                mMask ^= 1<<(19-mCursorX);
                Invalidate();
            }
            if (mCursorX == -1)
                SendMessage(GetParent(), 3, 0);
        }
    }

    int FindNextStep(int step)
    {
        for (int i=step/16+1; i<18; i++)
            if (IsStepEnabled(i))
                return i*16;
        
        return -1;
    }
    
    void CheckOnline()
    {
        if (mNotified)
        {
            if (CSequencer::Init())
            {
                mNotified = false;
                SendMessage(GetParent(), 2, 0);
            } else
            {
                SendMessage(GetParent(), 1, 0);
            }
        }
    }
    
    void NotifyOffline()
    {
        if (mNotified)
            return;
        mNotified = true;
        SendMessage(GetParent(), 1, 0);
    }

    virtual void OnTimer() override
    {
        if (mPaused)
            return;
        
        if (mPlayX == -1)
        {
            // find first step;
            mPlayX = FindNextStep(-16);
            
            if (mPlayX == -1)
            {
                mPlay = false;
                mLoop = false;
                mFollow = false;
                mFollowing = false;
                KillTimer();
            }
        }
        
        if (!Read())
            NotifyOffline();
        for (int i=0; i<9; i++)
        {
            TChannelInfo& channel = mChannel[mScrollY+i];
            if (channel.direction == TChannelInfo::Input)
            {
                switch (channel.sequence & 3)
                {
                    case 0b00: DrawBit(i, mPlayX, 0); break;
                    case 0b01: DrawBit(i, mPlayX, 2); break;
                    case 0b10: DrawBit(i, mPlayX, 2); break;
                    case 0b11: DrawBit(i, mPlayX, 1); break;
                }
            }
        }
        
        if ((mPlayX & 15) == 0)
        {
            int step = mPlayX / 16;
            
            if (!Write(step))
                NotifyOffline();
            
            DrawStep(mLastDrawnStep, false);
            DrawStep(step, true);
            mLastDrawnStep = step;
        }
        mPlayX++;
        
        if ((mPlayX & 15) == 0)
        {
            mPlayX = FindNextStep(mPlayX-1);
        } else if ((mPlayX & 15) == 15)
        {
            if (mFollow)
            {
                KillTimer();
                mFollowing = false;
            }
        }
        
        if (mPlayX == -1)
        {
            DrawStep(mLastDrawnStep, false);
            mLastDrawnStep = -1;
            if (mLoop)
            {
                mPlayX = -1;
            } else
            {
                KillTimer();
                mFollowing = false;
                mPlay = false;
                SendMessage(GetParent(), 0, 0);
            }
        }
    }
    
    bool IsStepEnabled(int step)
    {
        return mMask & (1 << (19-step));
    }

    void Play()
    {
        if (mPlay)
            return;
        CheckOnline();
        if (mFollow)
        {
            mFollow = false;
            if (mFollowing)
                KillTimer();
        }
        if (mLoop)
        {
            mLoop = false;
            KillTimer();
        }
        mPlayX = -1;
        SetTimer(10);
        SendMessage(GetParent(), 0, 0);
    }
    
    void Loop()
    {
        if (mLoop)
        {
            mLoop = false;
            KillTimer();
            return;
        }
        Play();
        mLoop = true;
    }
    
    bool Looping()
    {
        return mLoop;
    }
    
    void Follow()
    {
        if (mFollow)
        {
            KillTimer();
            SendMessage(GetParent(), 0, 0);
            mFollow = false;
            return;
        }
        
        if (mPlay || mLoop)
        {
            mPlay = false;
            mLoop = false;
            KillTimer();
            SendMessage(GetParent(), 0, 0);
        }
        CheckOnline();
        mFollow = true;
    }
    
    bool Following()
    {
        return mFollow;
    }
    
    void StartFollow(int index)
    {
        if (index < 0)
            return;
        
        mPlayX = index*16;
        if (!mFollowing)
        {
            SetTimer(10);
            mFollowing = true;
        }
    }
    
    // 620 ms
    // no print 530ms
    // no sequence 424ms
    // only sequence no background 265
    
    bool mPaused{false};
    void Pause(bool p = true)
    {
        mPaused = p;
    }
    
    bool IsPaused()
    {
        return mPaused;
    }
};

class CButton : public CWnd
{
public:
    virtual void OnPaint() override
    {
        if (GetFocus() == this)
        {
            LCD::RoundRect(m_rcClient, RGB565(ffffff));
            LCD::Print(m_rcClient.left+4, m_rcClient.top+1, RGB565(000000), RGBTRANS, m_pszId);
        } else
        {
            LCD::RoundRect(m_rcClient, RGB565(b0b0b0));
            LCD::Print(m_rcClient.left+4, m_rcClient.top+1, RGB565(ffffff), RGBTRANS, m_pszId);
        }
    }
    
    virtual void OnKey(int nKey) override
    {
        if (nKey == BIOS::KEY::F1)
        {
            SendMessage(GetParent(), 0, 0);
            return;
        }
        if (nKey == BIOS::KEY::Left)
        {
            CWnd::OnKey(KEY::Up);
            return;
        }
        if (nKey == BIOS::KEY::Right)
        {
            CWnd::OnKey(KEY::Down);
            return;
        }
        CWnd::OnKey(nKey);
    }

};

class CSelect : public CButton
{
};

class CApplication : public CWnd
{
    CMenuMain mMenu;
    CWndMessageBox mMessageBox;

    // Tab1
    CButton mPlay;
    CButton mLoop;
    CButton mFollow;
    CSequencerGui mSequencer;
    
    // Tab2
    // TODO: not implemented
    
public:
    void Create()
    {
        CWnd::Create("Application", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
        mMenu.Create("MainMenu", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, 14), this);
        
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
        
        SetTimer(100);
    }

    virtual void OnPaint() override
    {
        CRect rcTop(m_rcClient);
        rcTop.bottom = rcTop.top + 32;
        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
    }
    
    virtual void OnTimer() override
    {
        if (mSequencer.IsPaused())
        {
            if (!mMessageBox.IsVisible())
                mSequencer.Pause(false);
        }
    }
    
    virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override
    {
        if (pSender == &mPlay)
            mSequencer.Play();

        if (pSender == &mLoop)
            mSequencer.Loop();

        if (pSender == &mFollow)
            mSequencer.Follow();
        
        if (pSender == &mSequencer && code == 3)
        {
            this->SetFocus();
            mMessageBox.Show(this, "Not implemented", "Check source code", RGB565(FFFF00));
        } else if (pSender == &mSequencer && code == 2)
        {
            this->SetFocus();
            mMessageBox.Show(this, "Online mode", "Connection recovered!", RGB565(00FF00));
            mSequencer.Pause();
        } else if (pSender == &mSequencer && code == 1)
        {
            this->SetFocus();
            mMessageBox.Show(this, "Offline mode", "PCF8574 expanders not found!", RGB565(FFFF00));
            mSequencer.Pause();
        }
        else if (pSender == &mFollow || pSender == &mLoop || pSender == &mSequencer)
        {
            if (mSequencer.Following())
                mFollow.m_pszId = "\xfb" "Follow";
            else
                mFollow.m_pszId = "Follow";
            mFollow.Invalidate();

            if (mSequencer.Looping())
                mLoop.m_pszId = "\xfb" "Loop";
            else
                mLoop.m_pszId = "Loop";
            mLoop.Invalidate();
        }
    }

    void Destroy()
    {
        mSequencer.Destroy();
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
