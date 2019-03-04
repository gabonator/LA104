#pragma once

class CSampler;

class CSettings
{
    /*
public:
    class CTrigger // filter
    {
    public:
        // Primary
        int nMaxGap{5000};
        int nMaxGapOnce{9000}; // preamble len
        int nMaxTransitions{-1};  // CHCEME BREAKNUT READING PRE ATTACK? alebo nacitat cely buffer   

        // Secondary
        int nMinTransitions{15}; //pozor! prepisuje buffer aj ked nematchuje!
        int nMinFirstPulseDuration{-1};
        int nMaxFirstPulseDuration{-1};
        bool bPaused;//running?

        // Tertiary
        enum TMode
        {
            Single, All
        } eMode;
        enum TSkip
        {
            Every, Second, Third, Fourth, Fifth
        } eSkip;
        enum TSaveMode
        {
            Manual, Append, AutoAppend
        } eSave;
        bool bBeepOnMatch{true};
    };
     */
public:
    CSampler* mDeviceCurrent{nullptr};
    
    bool mConnected{true};
    bool mTriggerSingle{false};

    int mHistogramScaleX{3};
    int mHistogramScaleY{2};
    int mSignalScaleX{20};
    int mSignalOffset{0};
    int mMinGapTime{8000};
        
    bool mBeepCapture{true};
    bool mBeepDiscard{true};
    //int mGapTime{5000};
    int mSampleMax{200};
    int mTriggerTransitionsMin{15};
    int mTriggerTransitionsMax{-1};
    bool mTriggerFirstGap{false};
    bool mTriggerFirstPulse{false};
    int mInhibit{0};
} mSettings;

class CRuntime
{
public:
    bool mEnabled{true};
    bool mReceived{false};
    bool mTransmitted{false};
#ifdef EMULATED
    char mPath[512];
#else
    char mPath[128];
#endif
    bool mRecording{false};  
    int mSeconds{0};
} mRuntime;

class CStorage
{
public:
    int mSignalLength{0};
    uint16_t mSignalData[450];
} mStorage;
