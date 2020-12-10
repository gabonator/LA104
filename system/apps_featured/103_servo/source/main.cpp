#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include <math.h>

#include "knob.h"
#include "knob2.h"
#include "pca9685/Adafruit_PWMServoDriver.h"

uint16_t drawBuffer[32*32];

CRect InvalidRect(0, 0, 0, 0);


class CPwm
{
    bool mOnline{false};
    Adafruit_PWMServoDriver mPwmDriver;
    
public:
    bool begin()
    {
        mOnline = false;
        if (!mPwmDriver.begin())
        {
            //BIOS::DBG::Print("Failed to initialize driver!\n");
            //BIOS::SYS::DelayMs(15000);
            return false;
        }

        mPwmDriver.setOscillatorFrequency(27000000);
        mPwmDriver.setPWMFreq(50);  // Analog servos run at ~50 Hz updates
        mOnline = true;
        return true;
    }
    
    int rawValue(int level)
    {
        // 150..600
        // 110..500
        #define SERVOMIN  480 // This is the 'minimum' pulse length count (out of 4096)
        #define SERVOMAX  95 // This is the 'maximum' pulse length count (out of 4096)

        return SERVOMIN + (SERVOMAX-SERVOMIN)*level/180;
    }
    
    void write(int n, int level)
    {
        mPwmDriver.setPWM(n, 0, rawValue(level));
    }
    
    bool isOnline()
    {
        return mOnline;
    }
};

CPwm mPwm;

namespace Utils
{
    uint_fast16_t InterpolateColor( uint_fast16_t clrA, uint_fast16_t clrB, uint_fast8_t nLevel )
    {
        //nLevel = min(nLevel, 255);
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
}

enum EEasing {
    Linear,
    CubicInOut,
    Last = CubicInOut
};

class CDataPoint
{
public:
    CDataPoint()
    {
    }
    
    CDataPoint(int Uid) : mUid{Uid}
    {
    }
    
    int mUid{0};
    int mData[8] {0, 0, 0, 0, 0, 0, 0, 0};
    int mDuration{1000};
    EEasing mEasing{CubicInOut};
    
    /*
    void operator = (const CDataPoint& p)
    {
        memcpy(*this, &p, sizeof(CDataPoint));
    }*/
    bool operator == (const CDataPoint& p)
    {
        if (mUid != p.mUid)
            return false;
        for (int i=0; i<COUNT(mData); i++)
            if (mData[i] != p.mData[i])
                return false;
        return (mDuration == p.mDuration) && (mEasing == p.mEasing);
    }
    bool operator != (const CDataPoint& p)
    {
        return !operator ==(p);
    }
    const char* GetEasingName()
    {
        switch (mEasing)
        {
            case Linear: return "Linear";
            case CubicInOut: return "CubicInOut";
            default:
                return "Unknown";
        }
       return 0;
    }
};

CDataPoint mDataPointsData[16];
CArray<CDataPoint> mDataPoints{mDataPointsData, COUNT(mDataPointsData)};

class CListItem
{
public:
    uint32_t mRedrawMask{0};
    CRect mRect;
    void SetNeedsRedraw(int i)
    {
        if (i==-1)
        {
            int count = GetSubitemCount();
            for (int i=-1; i<count; i++)
                mRedrawMask |= 1<<(i+1);
            return;
        }
        mRedrawMask |= 1<<(i+1);
    }
    bool NeedsRedraw(int i)
    {
        int test = 1<<(i+1);
        if (!(mRedrawMask & test))
            return false;
        mRedrawMask &= ~test;
        return true;
    }
    //virtual CRect GetSize() = 0;
    //virtual CRect Paint(const CRect& rect, int focus) = 0;
    virtual int GetSubitemCount()
    {
        return 1;
    }
    virtual CListItem* Execute()
    {
        return nullptr;
    }
//    virtual ~CListItem()
//    {
//    }
    virtual CDataPoint* GetData()
    {
        return nullptr;
    }
    virtual void SetData(CDataPoint *p)
    {
    }

    virtual CRect GetSubitemRect(int i) = 0;
    virtual void PaintSubitem(const CRect& rect, int index, bool focus) = 0;
    
    virtual void OnKey(int subfocus, BIOS::KEY::EKey key) { _ASSERT(0); }
};

class CDialConfig : public CListItem
{
    enum {
        Max = 6,
        Width = Max*24 + (6-1)*8+2*0-24
    };
    
    CDataPoint* mDataPoint{nullptr};

public:
    virtual void SetData(CDataPoint* pData) override
    {
        mDataPoint = pData;
    }

    virtual CRect GetSubitemRect(int i) override
    {
        if (i<Max)
            return CRect(0, i*14, Width-16, i*14+14);
        else if (i==Max)
            return CRect(0, Max*14, 80, Max*14+16);
        else
            return CRect(80, Max*14, Width-16, Max*14+16);
        _ASSERT(0);
        return CRect();
    }
    
    virtual void PaintSubitem(const CRect& rect, int index, bool focus) override
    {
        if (index < Max)
        {
            BIOS::LCD::Bar(rect, focus ? RGB565(ffffff) : RGB565(202020));

            CRect rcSlider(rect.left, rect.top+1, rect.right-60, rect.top+14-1);
            int sliderWidth = rcSlider.Width();
            DrawSlider(rcSlider, mDataPoint->mData[index]*sliderWidth/180, focus);
            if (focus)
                BIOS::LCD::Printf(rcSlider.right+8, rect.top, RGB565(000000), RGB565(ffffff), "%d\xf8", mDataPoint->mData[index]);
            else
                BIOS::LCD::Printf(rcSlider.right+8, rect.top, RGB565(d0d0d0), RGB565(202020 ), "%d\xf8", mDataPoint->mData[index]);
        } else
        if (index == Max)
        {
            if (focus)
                BIOS::LCD::Print(rect.left, rect.top, RGB565(000000), RGB565(ffffff), "<Insert>");
            else
                BIOS::LCD::Print(rect.left, rect.top, RGB565(ffffff), RGB565(202020), "<Insert>");
        }
        else if (index == Max+1)
        {
            if (focus)
                BIOS::LCD::Print(rect.left, rect.top, RGB565(000000), RGB565(ffffff), "<Delete>");
            else
                BIOS::LCD::Print(rect.left, rect.top, RGB565(ffffff), RGB565(202020), "<Delete>");
        }
    }

    virtual int GetSubitemCount() override
    {
        return Max+2;
    }
    
    virtual CDataPoint* GetData() override
    {
        return mDataPoint;
    }

    virtual void OnKey(int subfocus, BIOS::KEY::EKey key) override
    {
        if (subfocus < Max)
        {
            if (key == BIOS::KEY::Right)
                mDataPoint->mData[subfocus] = min(mDataPoint->mData[subfocus] + 3, 180);
            if (key == BIOS::KEY::Left)
                mDataPoint->mData[subfocus] = max(mDataPoint->mData[subfocus] - 3, 0);
        }
    }

private:
    void DrawSlider(const CRect& rc, int pos, bool focus)
    {
        //BIOS::LCD::Bar(rc, RGB565(404040));
        
        int mid = (rc.top + rc.bottom)/2;
        CRect rcBar(rc.left+2, mid-2, rc.right-2, mid+2);
        //if (focus)
        //    BIOS::LCD::Bar(rcBar, RGB565(ffffff));
        //else
            BIOS::LCD::Rectangle(rcBar, RGB565(b0b0b0));
        
        int x = rc.left + pos;
        if (x < rc.left+2)
            x = rc.left+2;
        if (x >= rc.right-2)
            x = rc.right-2;
        
        CRect rcTick(x-2, rc.top, x+2, rc.bottom);
        BIOS::LCD::Bar(rcTick, /*focus ? RGB565(ffffff) :*/ RGB565(b0b0b0));
    }
};

CDialConfig mCommonDialConfig;

constexpr static int mCircleData[80*3] = { // TODO: uint8_t
  -7, 0, 255, -6, -1, 18, -7, -1, 236, -6, -2, 74, -7, -2, 180, -7,
  -3, 82, -6, -3, 172, -5, -4, 65, -6, -4, 189, -5, -4, 25, -5, -5,
  229, -4, -5, 65, -4, -6, 189, -3, -7, 82, -3, -6, 172, -2, -6, 74,
  -2, -7, 180, -1, -6, 18, -1, -7, 236, 0, -8, 0, 0, -7, 255, 1,
  -6, 18, 1, -7, 236, 2, -6, 74, 2, -7, 180, 3, -7, 82, 3, -6,
  172, 4, -5, 65, 4, -6, 189, 4, -5, 25, 5, -5, 229, 5, -4, 65,
  6, -4, 189, 7, -3, 82, 6, -3, 172, 6, -2, 74, 7, -2, 180, 6,
  -1, 18, 7, -1, 236, 8, 0, 0, 7, 0, 255, 6, 1, 18, 7, 1,
  236, 6, 2, 74, 7, 2, 180, 7, 3, 82, 6, 3, 172, 5, 4, 65,
  6, 4, 189, 5, 4, 25, 5, 5, 229, 4, 5, 65, 4, 6, 189, 3,
  7, 82, 3, 6, 172, 2, 6, 74, 2, 7, 180, 1, 6, 18, 1, 7,
  236, 0, 8, 0, 0, 7, 255, -1, 6, 18, -1, 7, 236, -2, 6, 74,
  -2, 7, 180, -3, 7, 82, -3, 6, 172, -4, 5, 65, -4, 6, 189, -4,
  5, 25, -5, 5, 229, -5, 4, 65, -6, 4, 189, -7, 3, 82, -6, 3,
  172, -6, 2, 74, -7, 2, 180, -6, 1, 18, -7, 1, 236, -8, 0, 0,
};



class CItemDials : public CListItem
{
public:
    enum {
        Padding = 0,
        KnobSize = knob_width,
        Spacing = 8,
        Max = 6,
        Width = Max*KnobSize + (Max-1)*Spacing+2*Padding
    };
    
    CDataPoint* mDataPoint{nullptr};
//    CDataPoint mLast;
//    bool mLastFocus{false};
//    int mLastY{-1};

public:
    virtual void SetData(CDataPoint* pData) override
    {
        mDataPoint = pData;
//        mLast.mUid = -1;
    }

    virtual CRect GetSubitemRect(int i) override
    {
        return CRect(0, 0, Width, KnobSize + 2*Padding);
    }
    
    virtual void PaintSubitem(const CRect& rect, int index, bool focus) override
    {
//        if (mLastFocus != focus)
//        {
//            mLastFocus = focus;
//            mLast.mUid = -1;
//        }
//        if (mLastY != rect.top)
//        {
//            mLastY = rect.top;
//            mLast.mUid = -1;
//        }
//
//        if (mLast.mUid == -1)
//            BIOS::LCD::Bar(rect, focus ? RGB565(ffffff) : RGB565(202020));

        int x = rect.left + Padding;
        int y = rect.top + Padding;
        
        for (int i=0; i<Max; i++)
        {
//            if (mLast.mUid != mDataPoint->mUid || mLast.mData[i] != mDataPoint->mData[i])
                DrawKnob(x, y, mDataPoint->mData[i], focus);
            
            if (i>0)
                BIOS::LCD::Bar(x-Spacing, rect.top, x, rect.bottom, focus ? RGB565(ffffff) : RGB565(202020));
            x += KnobSize + Spacing;
        }
//        mLast = *mDataPoint;
    }
    
    virtual CListItem* Execute() override
    {
        mCommonDialConfig.SetData(mDataPoint);
        return &mCommonDialConfig;
    }

    virtual CDataPoint* GetData() override
    {
        return mDataPoint;
    }

private:
    void DrawDot(int x, int y, int clr)
    {
        auto pixel = [](int x, int y, int c, int a)
        {
            drawBuffer[x+y*knob_width] = Utils::InterpolateColor(drawBuffer[x+y*knob_width], c, a);
        };

        pixel(x-1, y-1, clr, 128);
        pixel(x-1, y+0, clr, 255);
        pixel(x-1, y+1, clr, 128);
                
        pixel(x+0, y-1, clr, 255);
        pixel(x+0, y+0, clr, 255);
        pixel(x+0, y+1, clr, 255);
        
        pixel(x+1, y-1, clr, 128);
        pixel(x+1, y+0, clr, 255);
        pixel(x+1, y+1, clr, 128);
    }
    
    void DrawRange(int mn, int mx, int color, bool dot = false)
    {
        constexpr int guard = 10;
        mn = max(0, min(mn, 180));
        mx = max(mn, min(mx, 180));
        int left = guard + (80-2*guard)*mn/180;
        int right = guard + (80-2*guard)*mx/180;
        int i;
        for (i=left; i<right; i++)
        {
            int x = mCircleData[i*3];
            int y = mCircleData[i*3+1];
            int a = mCircleData[i*3+2];
            
            x = x + knob_width/2-1;
            y = y + knob_height/2;

            drawBuffer[x+y*knob_width] = Utils::InterpolateColor(drawBuffer[x+y*knob_width], color, a);
        }
        
        if (dot)
        {
            i &= ~1;
            int x = mCircleData[i*3];
            int y = mCircleData[i*3+1];

            x = x + knob_width/2-1;
            y = y + knob_height/2;

            DrawDot(x, y, RGB565(ffffff));
        }
    }
    
    void DrawKnob(int x, int y, int a, bool focus)
    {
        memcpy(drawBuffer, !focus ? knob_data : knob_data2, sizeof(knob_data));
        
        DrawRange(0, 180, RGB565(000000));
        DrawRange(0, a, RGB565(00ff00), true);
        
        BIOS::LCD::BufferBegin(CRect(x, y, x+knob_width, y+knob_height));
        BIOS::LCD::BufferWrite((uint16_t*)drawBuffer, knob_width*knob_height);
        BIOS::LCD::BufferEnd();
    }
};

class CItemFrameConfig : public CListItem
{
    CDataPoint* mDataPoint;

public:
    virtual void SetData(CDataPoint* pData) override
    {
        mDataPoint = pData;
    }

    virtual CRect GetSubitemRect(int i) override
    {
        return CRect(0, i*16, CItemDials::Width-16, i*16+16);
    }
    
    virtual void PaintSubitem(const CRect& rect, int index, bool focus) override
    {
        BIOS::LCD::Bar(rect, RGB565(202020));
        if (index == 0)
        {
            if (focus)
                BIOS::LCD::Printf(rect.left, rect.top, RGB565(000000), RGB565(ffffff), "Duration: %.1f s", mDataPoint->mDuration*0.001f);
            else
                BIOS::LCD::Printf(rect.left, rect.top, RGB565(ffffff), RGB565(202020), "Duration: %.1f s", mDataPoint->mDuration*0.001f);
        } else
        if (index == 1)
        {
            if (focus)
                BIOS::LCD::Printf(rect.left, rect.top, RGB565(000000), RGB565(ffffff), "Easing: <%s>", mDataPoint->GetEasingName());
            else
                BIOS::LCD::Printf(rect.left, rect.top, RGB565(ffffff), RGB565(202020), "Easing: <%s>", mDataPoint->GetEasingName());
        }
    }
    
    virtual void OnKey(int subfocus, BIOS::KEY::EKey key) override
    {
        if (subfocus == 0 && key == BIOS::KEY::Right)
            mDataPoint->mDuration += 100;
        if (subfocus == 0 && key == BIOS::KEY::Left && mDataPoint->mDuration >= 200)
            mDataPoint->mDuration -= 100;
        if (subfocus == 1 && key == BIOS::KEY::Right && mDataPoint->mEasing < EEasing::Last)
            mDataPoint->mEasing = (EEasing)(mDataPoint->mEasing + 1);
        if (subfocus == 1 && key == BIOS::KEY::Left && mDataPoint->mEasing > 0)
            mDataPoint->mEasing = (EEasing)(mDataPoint->mEasing - 1);
    }

    virtual int GetSubitemCount() override
    {
        return 2;
    }
};

CItemFrameConfig mCommonItemFrameConfig;

class CItemFrameInfo : public CListItem
{
    CDataPoint* mDataPoint{nullptr};
    
public:    
    virtual void SetData(CDataPoint* pData) override
    {
        mDataPoint = pData;
    }

    virtual CRect GetSubitemRect(int i) override
    {
        return CRect(0, 0, CItemDials::Width, 16);
    }
    
    virtual void PaintSubitem(const CRect& rect, int index, bool focus) override
    {
        BIOS::LCD::Bar(rect, focus ? RGB565(ffffff) : RGB565(202020));
        _ASSERT(mDataPoint->mDuration >= 10 && mDataPoint->mDuration <= 5000);
        if (focus)
            BIOS::LCD::Printf(rect.left+16, rect.top+1, RGB565(000000), RGB565(ffffff), "%.1f s", mDataPoint->mDuration*0.001f);
        else
            BIOS::LCD::Printf(rect.left+16, rect.top+1, RGB565(606060), RGB565(202020), "%.1f s", mDataPoint->mDuration*0.001f);
        DrawCurve(rect.right-16-24, rect.top);
    }

    virtual CListItem* Execute() override
    {
        mCommonItemFrameConfig.SetData(mDataPoint);
        return &mCommonItemFrameConfig;
    }

private:
    // https://github.com/warrenm/AHEasing/blob/master/AHEasing/easing.c
    float CubicEaseInOut(float p)
    {
        if(p < 0.5)
        {
            return 4 * p * p * p;
        }
        else
        {
            float f = ((2 * p) - 2);
            return 0.5 * f * f * f + 1;
        }
    }
    
    int EasingFunction(int x)
    {
        // 0..1024
        switch (mDataPoint->mEasing)
        {
            case Linear:
                return x;
            case CubicInOut:
                return CubicEaseInOut(x/1024.0f)*1024.0f;
            default:
                _ASSERT(0);
                return x;
        }
        return 0;
    }
    
    void DrawCurve(int _x, int _y)
    {
        uint16_t* line = drawBuffer;
        for (int i=0; i<24; i++)
        {
            int x = i*1024/24;
            int y = EasingFunction(x)*11*256/1024;

            for (int j=0; j<14; j++)
                line[j] = RGB565(202020);
            
            int decy = y / 256;
            int fray = y & 255;
            line[decy+1] = Utils::InterpolateColor(line[decy+1], RGB565(00ff00), 255-fray);
            line[decy+2] = Utils::InterpolateColor(line[decy+2], RGB565(00ff00), fray);

            if (i==0 || i==23)
            {
                for (int j=0; j<14; j++)
                    line[j] = RGB565(606060);
            } else {
                line[0] = RGB565(606060);
                line[13] = RGB565(606060);
            }

            CRect rcLine(_x+i, _y+1, _x+i+1, _y+15);
            BIOS::LCD::BufferBegin(rcLine);
            BIOS::LCD::BufferWrite(line, 14);
            BIOS::LCD::BufferEnd();
        }
    }
};

class CInterpolator
{
    CDataPoint mLastData{-1};
    CDataPoint mTargetData{-1};
    int32_t mAnimationStart{0};

public:
    void SetData(CDataPoint* p)
    {
        if (!p)
            return;
        if (mAnimationStart == 0 && (mLastData.mUid == p->mUid || mLastData.mUid == -1 ))
        {
            mAnimationStart = 0;
            // direct change
            if (mLastData != *p)
            {
                Apply(p);
                mLastData = *p;
            }
        } else
        {
            if (mAnimationStart == 0)
            {
                // Run animation
                mAnimationStart = BIOS::SYS::GetTick();
                mTargetData = *p;
            } else
            {
                if (p->mUid != mTargetData.mUid)
                {
                    // Animovali sme zo skutocneho mLastData do mTargetData,
                    // teraz sa vsak zaradil novy point
                    CDataPoint temp;
                    int f = Interpolate(temp);
                    memcpy(mLastData.mData, temp.mData, sizeof(temp.mData));
                    mLastData.mUid = mTargetData.mUid;
                    //mLastData = mTargetData;
                    // TODO: dopocitat
                    
                    int compensation = 0;//f*1500/1024;
                    mAnimationStart = BIOS::SYS::GetTick() - compensation;
                    mTargetData = *p;
                }
            }
        }
    }
    
    void Cancel()
    {
        if (mAnimationStart == 0)
        {
            return;
        }
        mLastData.mUid = -1;
        mTargetData.mUid = -1;
        mAnimationStart = 0;
    }
    
    void Tick()
    {
        if (mAnimationStart == 0)
            return;

        int32_t now = BIOS::SYS::GetTick();
        
        if (now >= mAnimationStart + mLastData.mDuration)
        {
            mLastData = mTargetData;
            mAnimationStart = 0;
            mTargetData.mUid = -1;
            Apply(&mLastData, true);
        }
        
        // interpolate
        CDataPoint temp;
        Interpolate(temp);
        Apply(&temp);
    }

    int Interpolate(CDataPoint& temp)
    {
        int32_t now = BIOS::SYS::GetTick();
        int f = (now - mAnimationStart)*1024/mLastData.mDuration;
        f = min(max(0, f), 1024);
        
        for (int i=0; i<COUNT(mLastData.mData); i++)
        {
            int a = mLastData.mData[i];
            int b = mTargetData.mData[i];
            temp.mData[i] = a + (b-a)*f/1024;
        }
        return f;
    }
    
    void Apply(CDataPoint* p, bool forceRedraw = false)
    {
        EVERY(100)
        {
            forceRedraw = true;
        }

        uint32_t l0 = BIOS::SYS::GetTick();
        if (mPwm.isOnline())
        {
            for (int i=0; i<COUNT(p->mData); i++)
            {
                mPwm.write(i, p->mData[i]);
            }
        }
        uint32_t l1 = BIOS::SYS::GetTick();

        if (forceRedraw)
        {
            int value[COUNT(CDataPoint::mData)];
            for (int i=0; i<COUNT(value); i++)
                value[i] = mPwm.rawValue(p->mData[i]);
            
            char message[32];
            if (mPwm.isOnline())
            {
                sprintf(message, "%d ms: %d, %d, %d, %d, %d, %d", (int)(l1-l0),
                        value[0], value[1], value[2], value[3], value[4], value[5]);
            } else {
                sprintf(message, "Offline: %d, %d, %d, %d, %d, %d",
                        value[0], value[1], value[2], value[3], value[4], value[5]);
                
                
                if (!mPwm.isOnline())
                {
                    EVERY(10000)
                    {
                        sprintf(message, "Reconnect...");
                        mPwm.begin();
                    }
                }

            }
            
            APP::Status(message);
        }
    }
    
    void GetIndices(int& idA, int& idB, int& factor)
    {
        idA = mLastData.mUid;
        idB = mTargetData.mUid;
        if (idB != -1)
        {
            int32_t now = BIOS::SYS::GetTick();
            factor = (now - mAnimationStart)*1024/mLastData.mDuration;
            factor = min(max(0, factor), 1024);
        } else
        {
            factor = -1;
        }
    }
    
    bool AnimationRunning()
    {
        return mAnimationStart != 0;
    }
};

class CList : public CWnd, CInterpolator
{
    int mFocus{0};
    int mSubFocus{0};
    bool mChild{false};
    bool mPlaying{false};
    
public:
    CListItem* mItemsData[32];
    CArray<CListItem*> mItems{mItemsData, COUNT(mItemsData)};
    CItemDials mBufferItemDials[8];
    CItemFrameInfo mBufferFrameInfo[8];
    
    CList()
    {
        for(int i=0; i<5; i++)
        {
            CDataPoint p(i);
            p.mData[i] = 180;
            mDataPoints.Add(p);
            mBufferItemDials[i].SetNeedsRedraw(-1);
            mBufferFrameInfo[i].SetNeedsRedraw(-1);
            mBufferItemDials[i].SetData(&mDataPoints[i]);
            mBufferFrameInfo[i].SetData(&mDataPoints[i]);
            mItems.Add(&mBufferItemDials[i]);
            mItems.Add(&mBufferFrameInfo[i]);
        }
    }
    
    void Draw(CRect& rc)
    {
        CRect remain(rc);
        remain.left = 24;
        // TODO: refresh only necessary
        CRect positionA = InvalidRect;
        CRect positionB = InvalidRect;
        int idA = -1, idB = -1, factor = -1;
        CInterpolator::GetIndices(idA, idB, factor);
        
        for (int i=0; i<mItems.GetSize(); i++)
        {
            CListItem* item = mItems[i];
            bool isChild = item->GetSubitemCount() > 1;
            int bottom = remain.top;
            bool needsRedraw = item->NeedsRedraw(-1);
            item->mRect = InvalidRect;
            for (int j=0; j<item->GetSubitemCount(); j++)
            {
                CRect required = item->GetSubitemRect(j);
                
                CRect placement(required);
                placement.Offset(remain.left, remain.top);
                item->mRect |= placement;
                
                // cursor
                if (!isChild)
                {
                    CDataPoint* pData = mItems[i]->GetData();
                    if (pData && pData->mUid == idA && !positionA.IsValid())
                        positionA = placement;
                    if (pData && pData->mUid == idB && !positionB.IsValid())
                        positionB = placement;
                }

                bottom = max(bottom, placement.bottom);
                if (isChild)
                    placement.Offset(16, 0);
                
                if (placement.bottom > remain.bottom)
                    break;

                if (item->NeedsRedraw(j))
                    item->PaintSubitem(placement, j, (mFocus == i) ? (mSubFocus == j) : false);
            }

            if (isChild && item->NeedsRedraw(-1) && remain.Height() > 0)
            {
                BIOS::LCD::Bar(CRect(remain.left, remain.top, remain.left+16, bottom), RGB565(202020));
            }
            remain.top = bottom;
        }

        if (remain.Height() > 0)
            BIOS::LCD::Bar(remain, RGB565(202020));
        
        CDataPoint* pData = mItems[mFocus]->GetData();
        if (pData)
        {
            SetData(pData);
        }
        
        int y1 = -1, y2 = -1;
        if (positionA.IsValid())
        {
            y1 = y2 = positionA.Center().y;
        }
        if (positionB.IsValid())
        {
            y2 = positionB.Center().y;
        }
        if (y1 != y2)
        {
            y1 = y1 + (y2-y1)*factor/1024;
        }
        // interpolateA, positionB
        if (y1 != -1)
        {
            /*
            if (mAnimationShift != 0)
            {
                if (y1 >= mChildRect.top)
                    y1 += mChildRect.Height();
            }*/
            static int y0 = -1;
            if (y0 != y1)
            {
                /*
                CRect rcButton0(0, y0-10, 16, y0+8);
                if (rcButton0.top > 16 && rcButton0.bottom < BIOS::LCD::Height-16)
                    BIOS::LCD::RoundRect(rcButton0, RGB565(202020));
                
                CRect rcButton1(0, y1-10, 16, y1+8);
                if (rcButton1.top > 16 && rcButton1.bottom < BIOS::LCD::Height-16)
                    BIOS::LCD::RoundRect(rcButton1, RGB565(ffffff));
*/
                if (y0 != -1 && abs(y0-y1) > 3 && y0-8+16<BIOS::LCD::Height)
                    BIOS::LCD::Print(4, y0-8, RGB565(00ff00), RGB565(202020), " ");
                y0 = y1;
                if (y1-8+16<BIOS::LCD::Height)
                    BIOS::LCD::Print(4, y1-8, RGB565(00ff00), RGB565(202020), "\x10");
            }
        }
    }
    
    int mChildDrawIndex;
    CRect mChildRect;
    CRect mAnimationRect;
    int mAnimationShift{0};
    
    void OnTick()
    {
        EVERY(20)
        {
            CInterpolator::Tick();
            
            if (mAnimationShift > 0)
            {
                int shift = min(8, mAnimationShift);
                mAnimationShift -= shift;
                mAnimationRect.Offset(0, shift);
                mAnimationRect.top = min(mAnimationRect.top, BIOS::LCD::Height-16);
                mAnimationRect.bottom = min(mAnimationRect.bottom, BIOS::LCD::Height-16);
                mChildRect.bottom += shift;
                
                // move mAnimationRect 8 pixels down
                for (int i=mAnimationRect.bottom+shift-1; i>=mAnimationRect.top; i--)
                {
                    if (i>=BIOS::LCD::Height-16)
                        continue;
                    BIOS::LCD::BufferBegin(CRect(mAnimationRect.left, i-shift, mAnimationRect.right, i+1-shift));
                    BIOS::LCD::BufferRead((uint16_t*)drawBuffer, BIOS::LCD::Width);
                    BIOS::LCD::BufferEnd();

                    BIOS::LCD::BufferBegin(CRect(mAnimationRect.left, i, mAnimationRect.right, i+1));
                    BIOS::LCD::BufferWrite((uint16_t*)drawBuffer, BIOS::LCD::Width);
                    BIOS::LCD::BufferEnd();

                }
                BIOS::LCD::Bar(mAnimationRect.left, mAnimationRect.top-shift, mAnimationRect.right, min(mAnimationRect.top, BIOS::LCD::Height-16), RGB565(202020));
                
                // finish
                if (mAnimationShift == 0)
                {
                    mChildRect.Invalidate();
                    
                    CListItem* pChild = mItems[mFocus]->Execute();
                    _ASSERT(pChild);

                    mItems[mFocus]->SetNeedsRedraw(mSubFocus);
                    
                    mItems.InsertAt(mFocus+1, pChild);
                    mFocus++;
                    mSubFocus = 0;
                    mChild = true;

                    mItems[mFocus]->SetNeedsRedraw(-1);
                } else {
                    CListItem* item = mItems[mFocus]->Execute();
                    _ASSERT(item);
                    int top = mChildRect.top;
                    for (int j=0; j<item->GetSubitemCount(); j++)
                    {
                        CRect required = item->GetSubitemRect(j);
                        required.Offset(mChildRect.left, top);
                        if (required.bottom > BIOS::LCD::Height-16)
                            break;
                        if (required.bottom > mChildRect.bottom)
                            break;
                        if (j >= mChildDrawIndex)
                        {
                            item->PaintSubitem(required, j, false);
                            mChildDrawIndex = j+1;
                        }
                    }
                }
            }
        }
        
        if (mPlaying)
        {
            if (!AnimationRunning())
            {
                // select next row;
                mItems[mFocus]->SetNeedsRedraw(mSubFocus);
                mFocus = (mFocus+1) % mItems.GetSize();
                mSubFocus = 0;
                mItems[mFocus]->SetNeedsRedraw(mSubFocus);
            }
            
        }
    }
    
    virtual void OnKey(int key) override
    {
        if (key != BIOS::KEY::None)
        {
            if (mPlaying)
            {
                mPlaying = false;
                return;
            }
        }
        if (mAnimationShift != 0)
            return;
        
        if (key==BIOS::KEY::Enter)
        {
            if (mItems[mFocus]->GetSubitemCount() == 1)
            {
                // probably selectable
                CListItem* pChild = mItems[mFocus]->Execute();
                if (pChild)
                {
                    mChildDrawIndex = 0;
                    mChildRect = CRect(24+16, mItems[mFocus]->mRect.bottom, BIOS::LCD::Width, mItems[mFocus]->mRect.bottom);
                    mAnimationRect = CRect(24, mItems[mFocus]->mRect.bottom, BIOS::LCD::Width, mItems[mItems.GetSize()-1]->mRect.bottom);
                    mAnimationShift = pChild->GetSubitemRect(pChild->GetSubitemCount()-1).bottom;
                }
            }
        }
        if (key==BIOS::KEY::Right && mItems[mFocus]->GetSubitemCount() > 1)
        {
            mItems[mFocus]->OnKey(mSubFocus, (BIOS::KEY::EKey)key);

            CDataPoint* p = mItems[mFocus]->GetData();
            if (p)
            {
                CInterpolator::Cancel();
                SetData(p);
                for (int i=0; i<mItems.GetSize(); i++)
                    if (mItems[i]->GetData() == p)
                        mItems[i]->SetNeedsRedraw(-1);
            } else
            {
                mItems[mFocus-1]->SetNeedsRedraw(-1);
                mItems[mFocus]->SetNeedsRedraw(mSubFocus);
            }
        }
        if (key==BIOS::KEY::Left && mItems[mFocus]->GetSubitemCount() > 1)
        {
            mItems[mFocus]->OnKey(mSubFocus, (BIOS::KEY::EKey)key);

            CDataPoint* p = mItems[mFocus]->GetData();
            if (p)
            {
                CInterpolator::Cancel();
                SetData(p);
                for (int i=0; i<mItems.GetSize(); i++)
                    if (mItems[i]->GetData() == p)
                        mItems[i]->SetNeedsRedraw(-1);
            } else
            {
                mItems[mFocus-1]->SetNeedsRedraw(-1);
                mItems[mFocus]->SetNeedsRedraw(mSubFocus);
            }
        }
        if (key==BIOS::KEY::Up)
        {
            if (mSubFocus > 0)
            {
                mItems[mFocus]->SetNeedsRedraw(mSubFocus);
                mSubFocus--;
                mItems[mFocus]->SetNeedsRedraw(mSubFocus);
            } else
            {
                if (mChild)
                {
//                    delete mItems[mFocus];
                    mItems.RemoveAt(mFocus);
                    mChild = false;
                    for (int i=mFocus; i<mItems.GetSize(); i++)
                    {
                        mItems[i]->SetNeedsRedraw(-1);
                        //mItems[i]->SetData(mItems[i]->GetData());
                    }
                }
                if (mFocus > 0)
                {
                    mItems[mFocus]->SetNeedsRedraw(mSubFocus);
                    mFocus--;
                    mSubFocus = mItems[mFocus]->GetSubitemCount()-1;
                    mItems[mFocus]->SetNeedsRedraw(mSubFocus);
                }
            }
        }
        if (key==BIOS::KEY::Down && mFocus < 10)
        {
            if (mSubFocus+1 >= mItems[mFocus]->GetSubitemCount())
            {
                if (mChild)
                {
                    return;
                }

                if (mFocus+1 < mItems.GetSize())
                {
                    mItems[mFocus]->SetNeedsRedraw(mSubFocus);
                    mFocus++;
                    mSubFocus = 0;
                    mItems[mFocus]->SetNeedsRedraw(mSubFocus);
                }
            } else
            {
                mItems[mFocus]->SetNeedsRedraw(mSubFocus);
                mSubFocus ++;
                mItems[mFocus]->SetNeedsRedraw(mSubFocus);
            }
        }
        
        if (key==BIOS::KEY::F3 && !mChild)
        {
            mPlaying = true;
        }

    }
};


bool setup()
{
    mPwm.begin();
    return true;
}

CList list;
void loop(BIOS::KEY::EKey key)
{
    CRect area(0, 16, BIOS::LCD::Width, BIOS::LCD::Height-16);
    list.OnKey(key);
    list.Draw(area);
    list.OnTick();
}


#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    CONSOLE::colorBack = RGB565(202020);

    APP::Init("Servo test");
    APP::Status("");

    if (setup())
    {
        BIOS::KEY::EKey key;
        while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
        {
          loop(key);
        }
    }
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(" [%d]: %s\n", line, cond);
    while (1);
}
