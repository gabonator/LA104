#include <library.h>
#include "../../os_host/source/framework/BufferedIo.h"
#include "shapes.h"
#include "mididevice.h"

uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];

void Nop()
{
}

class CMidiOutput : public CMidiDevice
{
public:
    void Panic()
    {
        for (int i=0; i<16; i++)
        {
            Send(0xb0 | i);
            Send(0x7b);
            Send(0x00);
            Send(0x0b);
        }
    }
};

class CMidiReader
{
    CBufferedReader mReader;
    
public:
    int mDivision{20};
    int mBytes{0};
    int mTickDurationUs{5000};
    int mBpm{120};
    uint16_t mChannelMask{(uint16_t)-1};

    CMidiOutput mOutput;
    
private:
    struct MThd
    {
        char chunk[4];
        uint32_t length;
        uint16_t format;
        uint16_t tracks;
        uint16_t division;
        
        bool isValid()
        {
            return memcmp(chunk, "MThd", 4) == 0 && length == 6 && (format == 1 || format == 0);
        }
        void fixEndian()
        {
            length = CMidiReader::fixEndian(length);
            format = CMidiReader::fixEndian(format);
            tracks = CMidiReader::fixEndian(tracks);
            division = CMidiReader::fixEndian(division);
        }
    } __attribute__((packed));
    
    struct MTrk
    {
        char chunk[4];
        uint32_t length;
        void fixEndian()
        {
            length = CMidiReader::fixEndian(length);
        }
        bool isValid()
        {
            return memcmp(chunk, "MTrk", 4) == 0 && length < 500000;
        }
    } __attribute__((packed));

private:
    static uint16_t fixEndian(uint16_t v)
    {
        return (v << 8) | (v >> 8);
    }
    
    static uint32_t fixEndian(uint32_t v)
    {
        return (v<<24) | ((v<<8) & 0x00ff0000) | ((v>>8) & 0x0000ff00) | (v>>24);
    }

    int getDeltaTime()
    {
        int time = 0;
        int quantity;
        while ((quantity = get()) & 0x80)
        {
            time |= quantity & 0x7f;
            time <<= 7;
        }
        time |= quantity;
        return time;
    }
    
    int get()
    {
        _ASSERT(mBytes > 0);
        mBytes--;
        
        uint8_t value;
        mReader >> value;
        return value;
    }
    
public:
    bool Open(char* file)
    {
        if (!mReader.Open(file))
            return false;
        
        MThd head;
        mReader >> CStream(&head, sizeof(head));
        
        head.fixEndian();
        if (!head.isValid())
        {
            BIOS::DBG::Print("CMidi: Invalid MThd\n");
            return false;
        }
        
        mDivision = head.division;
        
        MTrk track;
        mReader >> CStream(&track, sizeof(track));
        track.fixEndian();
        if (!track.isValid())
        {
            BIOS::DBG::Print("CMidi: Invalid MTrk\n");
            return false;
        }
        
        mBytes = track.length;
        return true;
    }
    
    bool Eof()
    {
        return mBytes <= 0;
    }
    
    int Wait()
    {
        return getDeltaTime();
    }
    
    void Event()
    {
        int event = get();
        int channel = event & 0x0f;

        switch (event)
        {
            case 0xff: // meta
            {
                // tempo & time signature
                int meta = get();
                int len = get();
                if (meta == 0x51) // tempo
                {
                    _ASSERT(len == 3);
                    int tempo = get();
                    tempo <<= 8;
                    tempo |= get();
                    tempo <<= 8;
                    tempo |= get();
                    
                    mBpm = 60000000UL / tempo;
                    mTickDurationUs = tempo / mDivision;
                } else
                {
                    // Skip meta
                    for (int i=0; i<len; i++)
                        get();
                }
                break;
            }
            case 0x80 ... 0x8f: // release
            case 0x90 ... 0x9f: // press
            case 0xa0 ... 0xaf: // aftertouch
            {
                int key = get();
                int velocity = get();
                
                if (mChannelMask & (1<<channel))
                {
                    mOutput.Send(event);
                    mOutput.Send(key);
                    mOutput.Send(velocity);
                }
                
                if ((event & 0xf0) == 0x80 || velocity == 0x00)
                    OnKeyRelease(channel, key);
                else if ((event & 0xf0) == 0x90)
                    OnKeyPress(channel, key);
                break;
            }
            case 0xb0 ... 0xbf: // controller
            {
                int cc = get();
                int value = get();
                
                if (mChannelMask & (1<<channel))
                {
                    mOutput.Send(event);
                    mOutput.Send(cc);
                    mOutput.Send(value);
                }
                
                switch (cc)
                {
                    case 0x07:
                        OnChannelVolume(channel, value);
                        break;
                    case 0x0a:
                        OnChannelPan(channel, value);
                        break;
                }
                break;
            }
            case 0xc0 ... 0xcf: // patch change
            {
                int instrumentId = get();
                
                if (mChannelMask & (1<<channel))
                {
                    mOutput.Send(event);
                    mOutput.Send(instrumentId); // instrument number
                }
                
                OnChannelPatch(channel, instrumentId);
                //prepareChannel(event & 0x0f);
                break;
            }
            case 0xe0 ... 0xef: // pitchbend
            {
                if (mChannelMask & (1<<channel))
                {
                    mOutput.Send(event);
                    mOutput.Send(get()); // lsb
                    mOutput.Send(get()); // msb
				} else
				{
                    get(); // lsb
                    get(); // msb
				}
                break;
            }
            default:
                _ASSERT(0);
        }
    }
    
    virtual void OnChannelPatch(int channel, int instrument) = 0;
    virtual void OnChannelVolume(int channel, int value) = 0;
    virtual void OnChannelPan(int channel, int value) = 0;
    virtual void OnKeyPress(int channel, int key) = 0;
    virtual void OnKeyRelease(int channel, int key) = 0;

    void Begin()
    {
        mOutput.Begin();
    }

    void End()
    {
        mOutput.End();
    }
};

class CBarVisualizer
{
public:
    void DrawChannels(int selected, bool focus)
    {
        for(int x=0; x<16; x++)
        {
            char strChannel[4];
            sprintf(strChannel, "%d", x+1);
            CRect rcBack(30 + x*18-2+2, 174+8, 30 + x*18-2+2 + 15, 174 + 8 + 14);
            if (focus)
                BIOS::LCD::Bar(rcBack, x==selected ? RGB565(ffffff) : RGB565(404040));
            else
                BIOS::LCD::Bar(rcBack, x==selected ? RGB565(b0b0b0) : RGB565(404040));
            
            if (strlen(strChannel) == 1)
                BIOS::LCD::Print(30 + x*18-2+5, 174+8, x==selected ? RGB565(000000) : RGB565(ffffff), RGBTRANS, strChannel);
            else
                BIOS::LCD::Print(30 + x*18-2, 174+8, x==selected ? RGB565(000000) : RGB565(ffffff), RGBTRANS, strChannel);
        }
    }
    
    void Hide()
    {
        BIOS::LCD::Bar(0, 32, BIOS::LCD::Width, 182, RGB565(404040));
    }

    void Show()
    {
        for(int x=0; x<16; x++)
        {
            for (int y=36; y<=72; y++)
                drawKey(x, y, RGB565(808080));
        }
        
        const uint8_t keyMap[13] = ".#.#].#.#.#]";
        
        for (int k=36; k<=72; k++)
        {
            int x = 6;
            int y = 178 - (k-36)*4 - 1;
            //CRect rc(x, y, x + 20, y+4);
            
            int ktype = keyMap[k%12];
            if (ktype == '.')
            {
                CRect rc1(x, y, x + 20, y+4);
                BIOS::LCD::Bar(rc1, RGB565(ffffff));
            }
            else if (ktype == ']')
            {
                CRect rc1(x, y+1, x + 20, y+4);
                BIOS::LCD::Bar(rc1, RGB565(ffffff));
                CRect rc2(x, y, x + 20, y+1);
                BIOS::LCD::Bar(rc2, RGB565(b0b0b0));
            }
            else
            {
                CRect rc1(x, y, x + 10, y+4);
                BIOS::LCD::Bar(rc1, RGB565(ffffff));
                CRect rc3(x, y+2, x + 10, y+3);
                BIOS::LCD::Bar(rc3, RGB565(b0b0b0));
                CRect rc2(x+10, y, x + 20, y+4);
                BIOS::LCD::Bar(rc2, RGB565(000000));
            }
        }
    }
    
    void drawKey(int channel, int key, int color)
    {
        if (key >= 48-12 && key <= 72) // C2..C5
        {
            int x = 30 + channel*18;
            int y = 178 - (key-36)*4;
            CRect rc(x, y, x + 15, y+2);
            BIOS::LCD::Bar(rc, color);
        }
    }
    
    void PressKey(int channel, int key)
    {
        drawKey(channel, key, RGB565(ffff00));
    }

    void PressMutedKey(int channel, int key)
    {
        drawKey(channel, key, RGB565(b0b000));
    }

    void ReleaseKey(int channel, int key)
    {
        drawKey(channel, key, RGB565(8080b0));
    }
    
    void PrepareChannel(int channel)
    {
        for (int y=36; y<=72; y++)
            drawKey(channel, y, RGB565(8080b0));
    }
};

class CKeyboardVisualizer
{
public:
    inline int ColorByChar(int c)
    {
        switch (c)
        {
            case '*':
                return RGB565(ff0000);
            case 'a' ... 'g':
                return RGB565(ffffff);
            case 'A' ... 'G':
                return RGB565(000000);
            default:
                return RGB565(d0d0d0);
        }
    }
    
    void DrawOctave(int x, int y, int selected, bool highlight)
    {
        char pattern1[] = "cccccCCCCCdddDDDDDeeeee|fffffFFFFFgggGGGGGaaaAAAAAbbbbb|";
        char pattern2[] = "ccccccc|ddddddd|eeeeeee|fffffff|ggggggg|aaaaaaa|bbbbbbb|";
        
        for (int i=0; pattern1[i]; i++)
        {
            char p1 = pattern1[i];
            char p2 = pattern2[i];
            
            if (selected != 0)
            {
                if (p1 == selected)
                {
                    if (highlight)
                        p1 = '*';
                } else
                {
                    p1 = 0;
                }
                if (p2 == selected)
                {
                    if (highlight)
                        p2 = '*';
                } else
                {
                    p2 = 0;
                }
                if (p1 == 0 && p2 == 0)
                    continue;
            }

            if (p1 == p2)
            {
                int color1 = ColorByChar(p1);
                CRect rc(x+i, y, x+i+1, y+40);
                BIOS::LCD::Bar(rc, color1);
            } else
            {
                if (p1)
                {
                    int color1 = ColorByChar(p1);
                    CRect rc1(x+i, y, x+i+1, y+20);
                    BIOS::LCD::Bar(rc1, color1);
                }
                if (p2)
                {
                    int color2 = ColorByChar(p2);
                    CRect rc2(x+i, y+20, x+i+1, y+40);
                    BIOS::LCD::Bar(rc2, color2);
                }
            }
        }
    }
    
    void PressKey(int channel, int key)
    {
        const static char keyTab[] = "cCdDefFgGaAb";
        int nOctave = key/12-2;
        int nKey = key%12;
        
        if (nOctave < 5)
            DrawOctave(22+nOctave*8*7, 100, keyTab[nKey], true);
    }
    
    void ReleaseKey(int channel, int key)
    {
        const static char keyTab[] = "cCdDefFgGaAb";
        int nOctave = key/12-2;
        int nKey = key%12;
        
        if (nOctave < 5)
            DrawOctave(22+nOctave*8*7, 100, keyTab[nKey], false);
    }

    void Show()
    {
        for (int i=0; i<5; i++)
            DrawOctave(22+i*8*7, 100, 0, false);
    }
    
    void Hide()
    {
        BIOS::LCD::Bar(0, 100, BIOS::LCD::Width, 100+40, RGB565(404040));
    }
};

class CGraphVisualizer
{
    uint8_t mKeys[128/8]; // TODO: char bits
    int mPos{0};
    static constexpr int mBaseY = 40;
    
public:
    CGraphVisualizer()
    {
        Reset();
    }
    
    void PressKey(int channel, int key)
    {
        if (key<128)
        {
            BIOS::LCD::PutPixel(mPos, mBaseY+key, RGB565(ff0000));
            mKeys[key/8] |= 1<<(key&7);
        }
    }
    
    void ReleaseKey(int channel, int key)
    {
        if (key<128)
        {
            BIOS::LCD::PutPixel(mPos, mBaseY+key, RGB565(808080));
            mKeys[key/8] &= ~(1<<(key&7));
        }
    }
    
    void Do()
    {
        CRect rcDraw(mPos, mBaseY, mPos+1, mBaseY+128);
        BIOS::LCD::BufferBegin(rcDraw);
        for (int i=0; i<128; i++)
        {
            if (mKeys[i/8] & (1<<(i&7)))
                BIOS::LCD::BufferWrite(RGB565(ffffff));
            else
                BIOS::LCD::BufferWrite(RGB565(808080));
        }
        mPos++;
        if (mPos >= BIOS::LCD::Width)
            mPos = 0;

        CRect rcDraw2(mPos, mBaseY, mPos+1, mBaseY+128);
        BIOS::LCD::BufferBegin(rcDraw2);
        for (int i=0; i<128; i++)
            BIOS::LCD::BufferWrite(RGB565(404040));
    }
    
    void Reset()
    {
        memset(mKeys, 0, sizeof(mKeys));
    }
    
    void Show()
    {
        BIOS::LCD::Bar(0, mBaseY, BIOS::LCD::Width, mBaseY+128, RGB565(408040));
    }
    
    void Hide()
    {
        BIOS::LCD::Bar(0, mBaseY, BIOS::LCD::Width, mBaseY+128, RGB565(404040));
    }
};

class CMidiPlayer : public CMidiReader
{
    CBarVisualizer mVisualizer;
    CKeyboardVisualizer mKeyboard;
    CGraphVisualizer mGraph;
    
    long mCurrentTime;
    long mCurrentTick;
    long mBaseTime;

    struct TChannel
    {
        uint8_t volume : 8;
        uint8_t pan : 8;
        uint8_t instrument : 8;
        
        uint8_t used : 1;
        uint8_t enabled : 1;
        uint8_t solo : 1;
        uint8_t dummy : 1;
        
        uint8_t swpos : 4;
    } mChannel[16];
    
    int mFileSize = 0;
    
    int mSelectedChannel{0};
    int mCursor{0};
    int mViewMode{0};
    int mBeatPhase{0};
    int mBeatTicks{128};
    
public:
    void Draw()
    {
        BIOS::LCD::Clear(RGB565(404040));
        DrawCaption();
        
        if (mViewMode == 1)
            mKeyboard.Show();
        if (mViewMode == 0)
            mVisualizer.Show();
        
        mVisualizer.DrawChannels(mSelectedChannel, mCursor == 0);
    }
    
    void DrawCaption()
    {
        CRect rc1(0, 0, BIOS::LCD::Width, 14);
        GUI::Background(rc1, RGB565(4040b0), RGB565(404040));

        int x = 0;
        x += BIOS::LCD::Print(x, 0, RGB565(000000), RGB565(b0b0b0), " MidiPlayer ");
        x += BIOS::LCD::Draw( x, 0, RGB565(b0b0b0), RGBTRANS, CShapes_sel_right);

        bool selected = mViewMode == 0;
        int color = selected ? RGB565(ffffff) : RGBTRANS;
        if (selected)
            x += BIOS::LCD::Draw( x, 0, color, RGBTRANS, CShapes_sel_left);
        x += BIOS::LCD::Print(x, 0, RGB565(000000), color, "Channels");
        if (selected)
            x += BIOS::LCD::Draw( x, 0, color, RGBTRANS, CShapes_sel_right);
        else
            x += 8;
        
        selected = mViewMode == 1;
        color = selected ? RGB565(ffffff) : RGBTRANS;
        if (selected)
            x += BIOS::LCD::Draw( x, 0, color, RGBTRANS, CShapes_sel_left);
        x += BIOS::LCD::Print(x, 0, RGB565(000000), color, "Keyboard");
        if (selected)
            x += BIOS::LCD::Draw( x, 0, color, RGBTRANS, CShapes_sel_right);
        else
            x += 8;

        selected = mViewMode == 2;
        color = selected ? RGB565(ffffff) : RGBTRANS;
        if (selected)
            x += BIOS::LCD::Draw( x, 0, color, RGBTRANS, CShapes_sel_left);
        x += BIOS::LCD::Print(x, 0, RGB565(000000), color, "Graph");
        if (selected)
            x += BIOS::LCD::Draw( x, 0, color, RGBTRANS, CShapes_sel_right);
    }
    
    virtual void OnChannelPatch(int channel, int instrument) override
    {
        if (mViewMode == 0)
            mVisualizer.PrepareChannel(channel);

        mChannel[channel].instrument = instrument;
        mChannel[channel].used = 1;
        mChannel[channel].enabled = 1;

        if (channel == mSelectedChannel)
            UpdateChannelInfo(channel);
    }
    
    virtual void OnChannelVolume(int channel, int value) override
    {
        mChannel[channel].volume = value;
        mChannel[channel].used = 1;
        if (channel == mSelectedChannel)
            UpdateChannelInfo(channel);
    }
    
    virtual void OnChannelPan(int channel, int value) override
    {
        mChannel[channel].pan = value;
        mChannel[channel].used = 1;
        if (channel == mSelectedChannel)
            UpdateChannelInfo(channel);
    }
    
    virtual void OnKeyPress(int channel, int key) override
    {
        if (mViewMode == 2)
        {
            if (channel == mSelectedChannel)
                mGraph.PressKey(channel, key);
        }
        if (mViewMode == 1)
        {
            if (channel == mSelectedChannel)
                mKeyboard.PressKey(channel, key);
        }
        if (mViewMode == 0)
        {
            if (CMidiPlayer::mChannelMask & (1<<channel))
                mVisualizer.PressKey(channel, key);
            else
                mVisualizer.PressMutedKey(channel, key);
        }
    }

    virtual void OnKeyRelease(int channel, int key) override
    {
        if (mViewMode == 2)
        {
            if (channel == mSelectedChannel)
                mGraph.ReleaseKey(channel, key);
        }
        if (mViewMode == 1)
        {
            if (channel == mSelectedChannel)
                mKeyboard.ReleaseKey(channel, key);
        }
        if (mViewMode == 0)
            mVisualizer.ReleaseKey(channel, key);
    }

    bool Do()
    {
        BIOS::KEY::EKey key = BIOS::KEY::GetKey();
        if (key == BIOS::KEY::Escape)
            return false;
        
        if (key != BIOS::KEY::None)
            HandleKey(key);

        if (Eof())
            return false;
        
        int wait = Wait();
        if (wait)
        {
            mCurrentTick += wait;
            mCurrentTime = mBaseTime + (mCurrentTick * mTickDurationUs)/1000;
            
            mBeatPhase += wait;
            if (mBeatPhase > mBeatTicks) // or loop?
            {
                mBeatPhase -= mBeatTicks;
                if (mViewMode == 2)
                    mGraph.Do();
            }
            
            UpdateProgress();

            while ((long)BIOS::SYS::GetTick() - mCurrentTime < 0)
                Nop();
        }
        Event();
        return true;
    }
    
    void HandleKey(BIOS::KEY::EKey key)
    {
		if (key == BIOS::KEY::F4)
		{
			mTickDurationUs = mTickDurationUs*2/3;
		}
		
		if (key == BIOS::KEY::F3)
		{
			mTickDurationUs = mTickDurationUs*3/2;
		}
		
        if (key == BIOS::KEY::Enter)
        {
            switch (mViewMode)
            {
                case 0:
                    mVisualizer.Hide();
                    break;
                case 1:
                    mKeyboard.Hide();
                    break;
                case 2:
                    mGraph.Hide();
                    break;
            }
            
            mViewMode = (mViewMode + 1) % 3;
            DrawCaption();
            
            switch (mViewMode)
            {
                case 0:
                    mVisualizer.Show();
                    for (int i=0; i<16; i++)
                        if (mChannel[i].used)
                            mVisualizer.PrepareChannel(i);
                    break;
                case 1:
                    mKeyboard.Show();
                    break;
                case 2:
                    mGraph.Show();
                    break;
            }
        }
        
        if (key == BIOS::KEY::Up && mCursor > 0)
        {
            mCursor--;
            UpdateChannelInfo(mSelectedChannel);
            mVisualizer.DrawChannels(mSelectedChannel, mCursor == 0);
        }
        
        if (key == BIOS::KEY::Down && mCursor < 2 && mChannel[mSelectedChannel].used)
        {
            mCursor++;
            UpdateChannelInfo(mSelectedChannel);
            mVisualizer.DrawChannels(mSelectedChannel, mCursor == 0);
        }
        
        if (mCursor == 0)
        {
            if (key == BIOS::KEY::Left && mSelectedChannel > 0)
            {
                mSelectedChannel--;
                UpdateChannelInfo(mSelectedChannel);
                mVisualizer.DrawChannels(mSelectedChannel, mCursor == 0);
                if (mViewMode == 1)
                    mKeyboard.Show();
                if (mViewMode == 2)
                    mGraph.Reset();
            }
            
            if (key == BIOS::KEY::Right && mSelectedChannel < 15)
            {
                mSelectedChannel++;
                UpdateChannelInfo(mSelectedChannel);
                mVisualizer.DrawChannels(mSelectedChannel, mCursor == 0);
                if (mViewMode == 1)
                    mKeyboard.Show();
                if (mViewMode == 2)
                    mGraph.Reset();
            }
        }
        
        if (mCursor == 1 && mChannel[mSelectedChannel].used)
        {
            if (key == BIOS::KEY::Left && mChannel[mSelectedChannel].swpos > 0)
            {
                mChannel[mSelectedChannel].swpos--;
                UpdateChannelInfo(mSelectedChannel);
                UpdateChannelMask();
                Panic();
            }
            if (key == BIOS::KEY::Right && mChannel[mSelectedChannel].swpos < 2)
            {
                mChannel[mSelectedChannel].swpos++;
                UpdateChannelInfo(mSelectedChannel);
                UpdateChannelMask();
                Panic();
            }
        }

        if (mCursor == 2 && mChannel[mSelectedChannel].used)
        {
            if (key == BIOS::KEY::Left && mChannel[mSelectedChannel].instrument > 0)
            {
                mChannel[mSelectedChannel].instrument--;
                UpdateChannelInfo(mSelectedChannel);
                CMidiPlayer::SetInstrument(mSelectedChannel, mChannel[mSelectedChannel].instrument);
            }
            if (key == BIOS::KEY::Right && mChannel[mSelectedChannel].instrument < 127)
            {
                mChannel[mSelectedChannel].instrument++;
                UpdateChannelInfo(mSelectedChannel);
                CMidiPlayer::SetInstrument(mSelectedChannel, mChannel[mSelectedChannel].instrument);
            }
        }
    }
    
    bool Open(char* file)
    {
        if (!CMidiReader::Open(file))
            return false;

        mBaseTime = BIOS::SYS::GetTick();
        mCurrentTick = 0;
        
        mBeatTicks = CMidiReader::mDivision/8; // ticks per 16th note
        
        mFileSize = CMidiReader::mBytes;

        memset(mChannel, 0, sizeof(mChannel));
        for (int i=0; i<16; i++)
            mChannel[i].swpos = 1;
        return true;
    }

    void Panic()
    {
        CMidiReader::mOutput.Panic();
    }

    void SetInstrument(int channel, int instrument)
    {
        CMidiReader::mOutput.Send(0xc0 | channel);
        CMidiReader::mOutput.Send(instrument);
    }

    const char* GetPatchName(int i)
    {
        if (i<0 || i>=128)
            return "Unknown";
        
        static const char* patches[] = {
            "Piano", "Chromatic", "Organ", "Guitar",
            "Bass", "Strings", "Ensemble", "Brass",
            "Reed", "Pipe", "Synth lead", "Synth pad",
            "Synth effect", "Etnic", "Percussive", "Sound effect"
        };
        
        return patches[i/8];
    }
    
    void UpdateProgress()
    {
        static long lastUpdated = -1;
        long now = BIOS::SYS::GetTick();
        
        if (now - lastUpdated < 500)
            return;
        
        lastUpdated = now;
        
        int relative = CMidiReader::mBytes * BIOS::LCD::Width / mFileSize;

        static int lastValue = BIOS::LCD::Width;
        if (abs(lastValue - relative) < 2)
            return;
        
        CRect rc(BIOS::LCD::Width-lastValue, BIOS::LCD::Height-4, BIOS::LCD::Width-relative, BIOS::LCD::Height);
        BIOS::LCD::Bar(rc, RGB565(ffff00));
        //rc.left = rc.right;
        //rc.right = BIOS::LCD::Width;
        //BIOS::LCD::Bar(rc, RGB565(404040));

        lastValue = relative;
    }
    
    void UpdateChannelInfo(int channel)
    {
        char strText[32];
        CRect rcInfo(0, 204, BIOS::LCD::Width, 232);
        BIOS::LCD::Bar(rcInfo, RGB565(404040));
        if (!mChannel[channel].used)
        {
            sprintf(strText, "Channel%d", channel+1);
            BIOS::LCD::Print(4, 204+14*0, RGB565(b0b0b0), RGBTRANS, strText);
        } else
        {
            sprintf(strText, "Channel%d:", channel+1);
            int x = 4, y = 204+14*0;
            int color = mCursor == 1 ? RGB565(ffffff) : RGB565(b0b0b0);
            x += BIOS::LCD::Print(x, y, color, RGBTRANS, strText);
            bool isOff = mChannel[channel].swpos == 0;
            bool isOn = mChannel[channel].swpos == 1;
            bool isSolo = mChannel[channel].swpos == 2;
            
            if (isOff)
            {
                x += BIOS::LCD::Draw( x, y, color, RGBTRANS, CShapes_sel_left);
                x += BIOS::LCD::Print(x, y, 0, color, "Off");
                x += BIOS::LCD::Draw( x, y, color, RGBTRANS, CShapes_sel_right);
            } else
                x += BIOS::LCD::Print(x, y, color, RGBTRANS, " Off ");

            if (isOn)
            {
                x += BIOS::LCD::Draw( x, y, color, RGBTRANS, CShapes_sel_left);
                x += BIOS::LCD::Print(x, y, RGBTRANS, color, "On");
                x += BIOS::LCD::Draw( x, y, color, RGBTRANS, CShapes_sel_right);
            } else
                x += BIOS::LCD::Print(x, y, color, RGBTRANS, " On ");

            if (isSolo)
            {
                x += BIOS::LCD::Draw( x, y, color, RGBTRANS, CShapes_sel_left);
                x += BIOS::LCD::Print(x, y, 0, color, "Solo");
                x += BIOS::LCD::Draw( x, y, color, RGBTRANS, CShapes_sel_right);
            } else
                x += BIOS::LCD::Print(x, y, color, RGBTRANS, " Solo ");
        }
        
        if (!mChannel[channel].used)
        {
            BIOS::LCD::Print(4, 204+14*1, RGB565(b0b0b0), RGBTRANS, "Instrument: Not set");
        } else
        {
            int x = 4, y = 204+14*1;
            int color = mCursor == 2 ? RGB565(ffffff) : RGB565(b0b0b0);

            x += BIOS::LCD::Print(x, y, color, RGBTRANS, "Instrument:");
            if (mCursor == 2)
            {
                sprintf(strText, "%03d", mChannel[channel].instrument+1);
                x += BIOS::LCD::Draw( x, y, color, RGBTRANS, CShapes_sel_left);
                x += BIOS::LCD::Print(x, y, 0, color, strText);
                x += BIOS::LCD::Draw( x, y, color, RGBTRANS, CShapes_sel_right);
            } else
            {
                sprintf(strText, " %03d ", mChannel[channel].instrument+1);
                x += BIOS::LCD::Print(x, y, color, RGBTRANS, strText);
            }
            if (channel == 9)
                strcpy(strText, "Drums");
            else
                strcpy(strText, GetPatchName(mChannel[channel].instrument)); //nende!!
            x += BIOS::LCD::Print(x, y, color, RGBTRANS, strText);
        }

        if (mChannel[channel].used)
        {
            sprintf(strText, "Vol: %d", mChannel[channel].volume);
            BIOS::LCD::Print(230, 204+14*0, RGB565(b0b0b0), RGBTRANS, strText);
            sprintf(strText, "Pan: %d", mChannel[channel].pan);
            BIOS::LCD::Print(230, 204+14*1, RGB565(b0b0b0), RGBTRANS, strText);
        }
    }
    
    void UpdateChannelMask()
    {
        if (mChannel[mSelectedChannel].swpos == 2) // solo
        {
            for (int i=0; i<16; i++)
                if (mSelectedChannel != i && mChannel[i].swpos == 2)
                    mChannel[i].swpos = 1;
        }
        
        // Any channel solo?
        for (int i=0; i<16; i++)
            if (mChannel[i].swpos == 2)
            {
                CMidiPlayer::mChannelMask = 1 << i;
                return;
            }
        
        CMidiPlayer::mChannelMask = 0;
        for (int i=0; i<16; i++)
            if (mChannel[i].swpos == 1)
                CMidiPlayer::mChannelMask |= 1 << i;
    }
};

int ListFiles(char* path, const CRect& rcWindow, int index, int scroll)
{
	if (BIOS::FAT::OpenDir(path) != BIOS::FAT::EResult::EOk)
		return 0;
	
	int y = rcWindow.top + 20;
	int i = 0;
	int first = scroll;
	int last = first;
	
	BIOS::FAT::TFindFile file;
	while (BIOS::FAT::FindNext(&file) == BIOS::FAT::EResult::EOk)
	{
		if (strstr(file.strName, ".MID") != nullptr)
		{
			if (scroll > 0)
			{
				scroll--;
				i++;
				continue;
			}
			if (y+16 < rcWindow.bottom)
			{
				last++;
				if (i==index)
					BIOS::LCD::Printf(rcWindow.left+4, y, RGB565(ffffff), RGB565(000000), " %s ", file.strName);
				else
					BIOS::LCD::Printf(rcWindow.left+4, y, RGB565(000000), RGB565(b0b0b0), " %s       ", file.strName);
			}
			y += 16;
			i++;
		}
	}
	
	int total = i;
	
	int top = rcWindow.top + 20;
	int bottom = rcWindow.bottom - 6;
	BIOS::LCD::Bar(rcWindow.right-10, top, rcWindow.right-6, bottom, RGB565(d0d0d0));
    if (total > 0)
    {
        BIOS::LCD::Bar(rcWindow.right-10, top + first * (bottom - top) / total, rcWindow.right-6, top + last * (bottom - top) / total, RGB565(808080));
    }
	return i;
}

void AppendFile(char* path, int index)
{
	if (BIOS::FAT::OpenDir(path) != BIOS::FAT::EResult::EOk)
		return;
	
	BIOS::FAT::TFindFile file;
	int i = 0;
	while (BIOS::FAT::FindNext(&file) == BIOS::FAT::EResult::EOk)
	{
		if (strstr(file.strName, ".MID") != nullptr)
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

char* GetFileToPlay()
{
	char* fileName = BIOS::OS::GetArgument();

    if (strstr(fileName, " "))
	{
		fileName = strstr(fileName, " ") + 1;
	}
	else
	{
		CRect rcWindow(40, 40, BIOS::LCD::Width-40, BIOS::LCD::Height-40);
		GUI::Window(rcWindow, RGB565(ffffff));
		const char* title = "Select file to play";
		BIOS::LCD::Print(BIOS::LCD::Width/2 - strlen(title)*4, rcWindow.top+2, RGB565(000000), RGBTRANS, title);
		
		_ASSERT(fileName);
		char* strPath = fileName;
		char* last = strrchr(strPath, '/');
		if (last)
			*last = 0;
		else
			strcpy(fileName, "");

		int maxLines = (rcWindow.Height()-20)/16;
		int scroll = 0;
		int index = 0;
		int totalFiles = ListFiles(strPath, rcWindow, index, 0);
		BIOS::KEY::EKey key;
		while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
		{
			if (key == BIOS::KEY::Up && index > 0)
			{
				index--;
				if (index < scroll)
					scroll = index;
				ListFiles(strPath, rcWindow, index, scroll);
			}
			if (key == BIOS::KEY::Down && index < totalFiles-1)
			{
				index++;
				if (index >= scroll+maxLines)
					scroll = index - maxLines +1;
				ListFiles(strPath, rcWindow, index, scroll);
			}
			if (key == BIOS::KEY::Enter)
			{
				AppendFile(strPath, index);
				return strPath;
			}
		}
	}
	return fileName;
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
#if defined(__APPLE__) || defined(WIN32)
	BIOS::FAT::Init();
	//BIOS::OS::SetArgument((char*)"/APPS/SYNTH/MIDI/PLAYER.ELF");
    BIOS::OS::SetArgument((char*)"/MIDI/PLAYER.ELF");
#endif
    _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
    BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);

    BIOS::LCD::Clear(RGB565(404040));
    
    CMidiPlayer midi;
    char strDisplay[40];
    bool loaded{false};
	
    char* fileName = GetFileToPlay();
	
    if (!fileName || strlen(fileName) == 0)
    {
        strcpy(strDisplay, "No midi loaded, use F4 in manager");
    } else
    {
        if (!midi.Open(fileName))
        {
            strcpy(strDisplay, "Failed to load midi file");
        } else
        {
            loaded = true;
            if (strlen(fileName)>=40)
            {
                strcpy(strDisplay, "...");
                strcat(strDisplay, fileName + strlen(fileName)-40+4);
            } else
            {
                strcpy(strDisplay, fileName);
            }
        }
    }

    midi.Begin();
    midi.Draw();

    CRect rcDisplay(0, 16, BIOS::LCD::Width, 16+16);
    BIOS::LCD::Bar(rcDisplay, RGB565(404040));
    BIOS::LCD::Print( (BIOS::LCD::Width-(int)strlen(strDisplay)*8)/2, 16, RGB565(ffffff), RGBTRANS, strDisplay);
    
    if (loaded)
    {
        while (midi.Do());
        midi.Panic();

        strcpy(strDisplay, "Finished playing");
        BIOS::LCD::Bar(rcDisplay, RGB565(404040));
        BIOS::LCD::Print( (BIOS::LCD::Width-(int)strlen(strDisplay)*8)/2, 16, RGB565(ffffff), RGBTRANS, strDisplay);
    }

    midi.End();

    Nop();
    BIOS::FAT::SetSharedBuffer(nullptr);

    BIOS::SYS::DelayMs(1500);
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

extern "C" void __cxa_pure_virtual(void)
{
  _ASSERT(!"Pure virtual call");
}
