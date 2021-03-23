#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include "../../../os_host/source/framework/BufferedIo.h"

#include "file/shapes.h"
#include "file/layout.h"
#include "file/file.h"

extern "C" {
#include "exec.h"
}

#if 0
const char* testcode = R"--(
import mini
import math
lcd = mini.lcd()
key = mini.key()

lcd.color(0x404040)
lcd.bar(0, 14, lcd.width, lcd.height-14)
a = 0
lx = 0
ly = 0
r = min(lcd.width, lcd.height)*0.4
cx = lcd.width/2
cy = lcd.height/2
step = 0.95
dir = -0.0003
gexc = 0
while key.get() == 0:
    gexc = (gexc + 1) % 256
    lcd.color(0xffff00+gexc - gexc*256)
    nx = cx + math.sin(a)*r
    ny = cy + math.cos(a)*r
    a = a + math.pi * step
    if step >= 1 or step <= 0.5:
        dir = -dir
    step = step + dir
    if lx != 0:
        lcd.line(lx, ly, nx, ny)
    lx = nx
    ly = ny
)--";
#else
const char* testcode = "";

#endif

uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];

const char* replPreset[] = {"help()", "help(\"modules\")", /*"mini.test()",*/ "mini.listScript()", "mini.loadScript()", "print(\"Hello!\")", "355/113", "42 + "};

extern bool usingKeys;
char heap[1024*5];
char stack[1024*2];

void gc_reset()
{
    gc_init(heap, heap + sizeof(heap));
    mp_init();
}

class CPython
{
public:
    char mQuery[32];
    
    void Init()
    {
        int stack_dummy;
        mp_stack_set_limit(20000 * (BYTES_PER_WORD / 4));
        mp_stack_set_top(&stack_dummy);
        mp_pystack_init(stack, stack + (sizeof(stack) / sizeof(size_t)));
        gc_init(heap, heap + sizeof(heap));
        mp_init();
        mp_stack_ctrl_init();
    }
    
    void Eval(char* query)
    {
        vstr_t line;
        line.alloc = sizeof(mQuery);
        line.len = strlen(mQuery);
        line.buf = query;

        Eval(line, false);
    }

    bool Eval(vstr_t& line, bool file)
    {
        _ASSERT(!mp_repl_continue_with_input(vstr_null_terminated_str(&line)));
        int ret = parse_compile_execute(&line, file ? MP_PARSE_FILE_INPUT : MP_PARSE_SINGLE_INPUT,
            EXEC_FLAG_IS_REPL | EXEC_FLAG_SOURCE_IS_VSTR, NULL);
        return ret == 0;
    }
};

class CRepl : public CWnd
{
    int mReplPresetIndex{0};
    CPython mPython;
    CFileDialog mFile;
    CFileFilterSuffix mFilter{".PY"};
    CBufferedReader mReader;
    bool mBanner{true};
    
public:
    void Create()
    {
        CWnd::Create("main", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
        
        Clear();
        
        strcpy(mPython.mQuery, replPreset[mReplPresetIndex]);
        ShowQuery();
        
        SetTimer(500);
        mPython.Init();
        
        if (strlen(testcode) > 0)
            RunScript(testcode);
    }
    
    void Clear()
    {
        APP::Init("Circuit python " MICROPY_VERSION_STRING);
        CONSOLE::Clear();
        CONSOLE::Color(RGB565(b0b0b0));
        if (mBanner)
        {
            CONSOLE::Print("%s\n", MICROPY_FULL_VERSION_INFO);
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
            strcpy(mPython.mQuery, replPreset[mReplPresetIndex]);
            ShowQuery();
        }
        if (key == BIOS::KEY::EKey::Down && mReplPresetIndex < COUNT(replPreset)-1)
        {
            mReplPresetIndex++;
            strcpy(mPython.mQuery, replPreset[mReplPresetIndex]);
            ShowQuery();
        }
        if (key == BIOS::KEY::EKey::Enter)
        {
            ShowQuery(false);
            HideCursor();
            CONSOLE::Print("\n");
            if (strcmp(mPython.mQuery, "mini.test()") == 0)
            {
                
            }
            else if (strcmp(mPython.mQuery, "mini.loadScript()") == 0)
            {
                LoadScript();
            } else
            if (strcmp(mPython.mQuery, "mini.listScript()") == 0)
            {
                ListScript();
            } else
            {
                mPython.Eval(mPython.mQuery);
            }
            ShowQuery();
        }
    }
     
    void RunScript(const char* text)
    {
        strcpy((char*)BIOS::FAT::GetSharedBuffer(), text);
        vstr_t line;
        line.alloc = BufferSectorSize;
        line.len = strlen(text);
        line.buf = (char*)BIOS::FAT::GetSharedBuffer();
        mReader.Close();
                                
        gc_reset();
        mPython.Eval(line, true);
    }

    void LoadScript()
    {
        if (mFile.ModalShow(nullptr, "Load python script", &mFilter))
        {
            CONSOLE::Clear();
            APP::Status(mFile.GetFilename());
            
            if (mReader.Open(mFile.GetFilename()))
            {
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
        CONSOLE::Print(mPython.mQuery);
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
    
    virtual void OnTimer()
    {
        BlinkCursor();
    }
};

CRepl mRepl;

extern "C" void _PrintError(const char* msg, size_t len)
{
    mRepl.PrintError(msg, len);
}

extern "C" void _PrintAnswer(const char* msg, size_t len)
{
    mRepl.PrintAnswer(msg, len);
}

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
