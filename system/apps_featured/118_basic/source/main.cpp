#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include "../../../os_host/source/framework/BufferedIo.h"

#include "file/shapes.h"
#include "file/layout.h"
#include "file/file.h"

#include <Arduino.h>
#include "terminal-basic/basic_interpreter.hpp"
#include "terminal-basic/version.h"

uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];

class Output : public VT100::Print
{
public:
    Output() : VT100::Print() {}
    void clear() override {}
    
protected:
    uint8_t getCursorX() override { return 0; }
    void setCursor(uint8_t, uint8_t) override {}
    void setCursorX(uint8_t) override {}
    void writeChar(uint8_t c) override {assert(0);}
    
    virtual void addAttribute(VT100::TextAttr) override {assert(0);}
    virtual void resetAttributes() override {assert(0);}

    virtual size_t write(uint8_t c) override {
        static char c0, c1, c2, c3;
        c0 = c1; c1 = c2; c2 = c3; c3 = c;
        if (c0 == 27 || c1 == 27 || c2 == 27 || c3 == 27)
        {
            if (c0 == 27)
            {
                if (c1 == '[' && c2 == '1' && c3 == 'm')
                    CONSOLE::Color(RGB565(ffff50));
                else if (c1 == '[' && c2 == '0' && c3 == 'm')
                    CONSOLE::Color(RGB565(ffffff));
                else
                    assert(0);
            }
            return 0;
        }
        CONSOLE::Putch(c);        
        return 0;
    }

private:
    void scroll() {}
};

uint32_t HAL_random_generate(uint32_t max) { return rand() % max; }
void HAL_random_seed(uint32_t seed) { srand(seed); }
uint32_t HAL_time_gettime_ms() { return BIOS::SYS::GetTick(); }
void HAL_time_sleep_ms(uint32_t ms) { BIOS::SYS::DelayMs(ms); }
void HAL_update() { }

#if 0
bool setup()
{
    GUI::Background(CRect(0, 14, BIOS::LCD::Width, BIOS::LCD::Height-14), RGB565(0000b0), RGB565(4040d0));

    return true;
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    CONSOLE::colorBack = RGB565(202020);

    APP::Init("BASIC Interpreter");
    APP::Status("");

    Stream _stream;
    Print _print;
    Output _output;
    BASIC::Interpreter basic(_stream, _output, BASIC::SINGLE_PROGSIZE);
    basic.newProgram();
    
    if (setup())
    {
        BIOS::KEY::EKey key;
        while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
        {
            basic.step();
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

#else


const char* replPreset[] = {"PRINT \"Hello!\"", "10 FOR I = 1 TO 10", "20 PRINT I, I*I",
    "30 NEXT I", "LIST", "PRINT RND()*1000", "RUN", "LOAD"};


class CBasic
{
    Stream _stream;
    Print _print;
    Output _output;
    bool mRunning{false};

public:
    BASIC::Interpreter mBasic;

    CBasic() : mBasic(_stream, _output, BASIC::SINGLE_PROGSIZE)
    {
    }
    void Init()
    {
        mBasic.newProgram();
    }
    void SetBuffer(const char* text)
    {
        mBasic._inputPosition = 0;
        //mBasic._inputL strlen(text);
        strcpy((char*)mBasic._inputBuffer, text);
    }
    char* GetBuffer()
    {
        return (char*)mBasic._inputBuffer;
    }
    void Eval()
    {
        mBasic.exec();
    }
    void Step()
    {
        if (mBasic._state == BASIC::Interpreter::EXECUTE)
            mBasic.step();
    }
    bool IsRunning()
    {
        if (mBasic._state == BASIC::Interpreter::EXECUTE)
        {
            mRunning = true;
            return true;
        }
        return false;
    }
    bool FinishedRunning()
    {
        if (mBasic._state != BASIC::Interpreter::EXECUTE && mRunning)
        {
            mRunning = false;
            return true;
        }
        return false;
    }
};

class CRepl : public CWnd
{
    int mReplPresetIndex{0};
    CBasic mBasic;
    CFileDialog mFile;
    CFileFilterSuffix mFilter{".BAS"};
    CBufferedReader mReader;
    bool mBanner{true};
    
public:
    void Create()
    {
        CWnd::Create("main", CWnd::WsVisible | CWnd::WsTick,
                     CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
        
        Clear();
        
        mBasic.SetBuffer(replPreset[mReplPresetIndex]);
        ShowQuery();
        
        SetTimer(500);
        mBasic.Init();
    }
    
    void Clear()
    {
        APP::Init("BASIC Interpreter");
        CONSOLE::Clear();
        CONSOLE::Color(RGB565(b0b0b0));
        if (mBanner)
        {
            CONSOLE::Print("TerminalBasic %s\n", VERSION);
            CONSOLE::Color(RGB565(ffffff));
            CONSOLE::Print("READY\n");
            mBanner = false;
        }
        APP::Status("");
    }
    
    void PrintAnswer(const char* msg, size_t len)
    {
        CONSOLE::Color(RGB565(b0b0b0));
        for (int i=0; i<len; i++)
            if (msg[i] != '\r')
                CONSOLE::Putch(msg[i]);
    }
    
    void PrintError(const char* msg, size_t len)
    {
        CONSOLE::Color(RGB565(ffff00));
        for (int i=0; i<len; i++)
            if (msg[i] != '\r')
                CONSOLE::Putch(msg[i]);
    }
    
    virtual void OnKey(BIOS::KEY::EKey key)
    {
        if (key == BIOS::KEY::EKey::Up && mReplPresetIndex > 0)
        {
            mReplPresetIndex--;
            mBasic.SetBuffer(replPreset[mReplPresetIndex]);
            ShowQuery();
        }
        if (key == BIOS::KEY::EKey::Down && mReplPresetIndex < COUNT(replPreset)-1)
        {
            mReplPresetIndex++;
            mBasic.SetBuffer(replPreset[mReplPresetIndex]);
            ShowQuery();
        }
        if (key == BIOS::KEY::EKey::Enter)
        {
            ShowQuery(false);
            HideCursor();
            CONSOLE::Print("\n");
            if (strcmp(mBasic.GetBuffer(), "mini.loadScript()") == 0)
            {
                LoadScript();
            } else
            {
                CONSOLE::Color(RGB565(ffffff));
                mBasic.Eval();
            }
            if (!mBasic.IsRunning())
                ShowQuery();
        }
    }
     
    void RunScript(const char* text)
    {
        /*
        strcpy((char*)BIOS::FAT::GetSharedBuffer(), text);
        vstr_t line;
        line.alloc = BufferSectorSize;
        line.len = strlen(text);
        line.buf = (char*)BIOS::FAT::GetSharedBuffer();
        mReader.Close();
                                
        gc_reset();
        mPython.Eval(line, true);
         */
        _ASSERT(0);
    }

    void LoadScript()
    {
        if (mFile.ModalShow(nullptr, "Load python script", &mFilter))
        {
            CONSOLE::Clear();
            APP::Status(mFile.GetFilename());
            
            if (mReader.Open(mFile.GetFilename()))
            {
                /*
                vstr_t line;
                line.alloc = BufferSectorSize;
                line.len = mReader.GetFileSize();
                line.buf = (char*)mReader.GetBuffer();
                mReader.Close();
                                
                //gc_collect();
                usingKeys = false;
                gc_reset();
                mPython.Eval(line, true);
                
                if (!usingKeys)
                {
                    CONSOLE::Color(RGB565(aaffaa));
                    CONSOLE::Print("Press any key to continue...");
                    BIOS::KEY::EKey key;
                    while ((key = BIOS::KEY::GetKey()) == BIOS::KEY::None);
                    CONSOLE::Print("\n");
                } else {
                    Clear();
                }
                */
                _ASSERT(0);
            } else {
                Clear();
                const char* message = "cannot open file\n";
                PrintError(message, strlen(message));
            }
        } else {
            Clear();
            const char* message = "loadScript() failed\n";
            PrintError(message, strlen(message));
        }
        SetFocus();
    }

    void ListScript()
    {
        if (mFile.ModalShow(nullptr, "Load python script", &mFilter))
        {
            CONSOLE::Clear();
            CONSOLE::Color(RGB565(b0b0b0));
            ShowQuery(false);
            PrintAnswer("\n", 1);
            if (mReader.Open(mFile.GetFilename()))
            {
                PrintAnswer((char*)mReader.GetBuffer(), mReader.GetFileSize());
            }
        } else {
            Clear();
            const char* message = "loadScript() failed\n";
            PrintError(message, strlen(message));
        }
        SetFocus();
    }

    void ShowQuery(bool active = true)
    {
        HideCursor();
        if (CONSOLE::cursor.x > CONSOLE::window.left)
            BIOS::LCD::Bar(CONSOLE::window.left , CONSOLE::cursor.y, CONSOLE::cursor.x, CONSOLE::cursor.y+14, CONSOLE::colorBack);
        CONSOLE::cursor.x = CONSOLE::window.left;
        CONSOLE::Color(active ? RGB565(00ff00) : RGB565(aaffaa));
        CONSOLE::Print(">>> ");
        CONSOLE::Color(active ? RGB565(ffffff) : RGB565(aaffaa));
        CONSOLE::Print(mBasic.GetBuffer());
        if (active)
            ShowCursor();
    }
    
    void BlinkCursor()
    {
        static bool on = false;
        if (!HasFocus())
            return;

        if (on)
            ShowCursor();
        else
            HideCursor();
        on = !on;
    }

    void ShowCursor()
    {
        using namespace CONSOLE;
        BIOS::LCD::Draw(cursor.x, cursor.y, colorFront, colorBack, CShapes_cursor);
    }

    void HideCursor()
    {
        CONSOLE::Print(" \x08");
    }
    
    virtual void OnTimer() override
    {
        BlinkCursor();
    }
    virtual void OnTick()
    {
        mBasic.Step();
        if (mBasic.FinishedRunning())
            ShowQuery();
    }
    virtual void WindowMessage(int nMsg, int nParam =0) override
    {
        if (nMsg == CWnd::WmTick)
            OnTick();
        CWnd::WindowMessage(nMsg, nParam);
    }
};

CRepl mRepl;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
#ifdef __APPLE__
    BIOS::OS::SetArgument((char*)"devel/cpython.elf");
#endif
    _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
    BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);

    mRepl.Create();
    mRepl.SetFocus();
    BIOS::KEY::EKey key;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
    {
        if (key != BIOS::KEY::EKey::None)
        {
            mRepl.OnKey(key);
        }
        mRepl.WindowMessage(CWnd::WmTick);
    }

    BIOS::FAT::SetSharedBuffer(nullptr);
    return 0;
}


#ifndef __APPLE__
extern "C" void __cxa_pure_virtual(void)
{
  _ASSERT(!"Pure virtual call");
}
#endif

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(" [%d]: %s\n", line, cond);
    while (1);
}
#endif
