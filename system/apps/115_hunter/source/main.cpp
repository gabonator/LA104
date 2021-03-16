#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/SimpleApp.h"
#include "hunter.h"
#include "framework/shapes.h"
#include "framework/layout.h"

using namespace BIOS;

class CHunterInterface
{
public:
    void StartZone(int zone, int minutes)
    {
        CONSOLE::Print("Starting zone %d for %d minutes.\n", zone, minutes);
        HunterStart(zone, minutes);
    }
    
    void StopZone(int zone)
    {
        CONSOLE::Print("Stopping zone %d.\n", zone);
        HunterStop(zone);
    }
    
    void RunProgram(int program)
    {
        CONSOLE::Print("Running program %c.\n", 'A' - 1 + program);
        HunterProgram(program);
    }
};

class CAppPage : public CWnd, public CHunterInterface
{
public:
    int mX{0};
    int mY{0};
    int mnPage{0};
    
    int mnRunZone{1};
    int mnRunFor{10};
    int mnRunProgram{3};
    
    bool mDemo{false};
    int mDemoZone{0};


    void DrawMenu()
    {
        const char* mPages[] = {"Control"};
        
        CRect rcControls(0, 14+14*10, BIOS::LCD::Width, 14+14*10+14+4);
        
        LCD::Bar(rcControls, CONSOLE::colorBack);
        rcControls.top += 4;
        int x = 8;
        for (int i=0; i<COUNT(mPages); i++)
        {
            if (mnPage != i)
            {
                x += BIOS::LCD::Draw(x, rcControls.top, RGB565(4040b0), RGBTRANS, CShapes_tab_left);
                x += BIOS::LCD::Print(x, rcControls.top, RGB565(b0b0b0), RGB565(4040b0), mPages[i]);
                x += BIOS::LCD::Draw(x, rcControls.top, RGB565(4040b0), RGBTRANS, CShapes_tab_right);
            } else if (mY != -1) {
                x += BIOS::LCD::Draw(x, rcControls.top, RGB565(404040), RGBTRANS, CShapes_tab_left);
                x += BIOS::LCD::Print(x, rcControls.top, RGB565(ffffff), RGB565(404040), mPages[i]);
                x += BIOS::LCD::Draw(x, rcControls.top, RGB565(404040), RGBTRANS, CShapes_tab_right);
            } else {
                x += BIOS::LCD::Draw(x, rcControls.top, RGB565(ffffff), RGBTRANS, CShapes_tab_left);
                x += BIOS::LCD::Print(x, rcControls.top, RGB565(000000), RGB565(ffffff), mPages[i]);
                x += BIOS::LCD::Draw(x, rcControls.top, RGB565(ffffff), RGBTRANS, CShapes_tab_right);
            }
            x += 8;
        }
    }

    void Draw(Layout::Render& r)
    {
        using namespace Layout;
        GUI::Background(r.GetRectangle(), RGB565(404040), RGB565(404040));
        r << Padding(4, 4, 4, 2);
        
        Color def(RGB565(b0b0b0));
        Color hig(RGB565(ffffff));
        
        #define S(x, y) Select(mX == x && mY == y)

        r << S(0, 0) << RadioButton(true, "Run") <<
            "zone" << S(1, 0) << RadioButton(false, "\x11") << toString(mnRunZone) << S(2, 0) << RadioButton(false, "\x10") <<
            "for" << S(3, 0) << RadioButton(false, "\x11") << toString(mnRunFor) << S(4, 0) << RadioButton(false, "\x10") <<
            "minutes" << NewLine();
        r << S(0, 1) << RadioButton(true, "Stop") << "zone " << toString(mnRunZone) << NewLine();
        r << S(0, 2) << RadioButton(true, "Run") << "program" << S(1, 2) << RadioButton(false, "\x11") << toChar(mnRunProgram) << S(2, 2) << RadioButton(false, "\x10")
            << Spacer(64)
            << S(3, 2) << Radio(mDemo, "Demo");
        
        #undef S
    }

    void DrawPage()
    {
        CRect rcControls(0, 14+14*10+14+4, BIOS::LCD::Width, BIOS::LCD::Height-14);
        Layout::Render r(rcControls);
        Draw(r);
    }

    virtual void OnKey(int key) override
    {
        if (key == BIOS::KEY::Enter)
        {
            switch (mY*16 + mX)
            {
                // 1st row
                case 0x00:
                    StartZone(mnRunZone, mnRunFor);
                    break;
                case 0x01:
                    if (mnRunZone > 1)
                        mnRunZone--;
                    break;
                case 0x02:
                    if (mnRunZone < 48)
                        mnRunZone++;
                    break;
                case 0x03:
                    if (mnRunFor > 5)
                        mnRunFor-=5;
                    break;
                case 0x04:
                    if (mnRunFor <= 240-5)
                        mnRunFor+=5;
                    break;
                // 2nd row
                case 0x10:
                    StopZone(mnRunZone);
                    break;
                // 3rd row
                case 0x20:
                    RunProgram(mnRunProgram);
                    break;
                case 0x21:
                    if (mnRunProgram > 1)
                        mnRunProgram--;
                    break;
                case 0x22:
                    if (mnRunProgram < 4)
                        mnRunProgram++;
                    break;
                case 0x23:
                    mDemo = !mDemo;
                    if (!mDemo)
                        StopDemo();
                    break;
            }
        }

        switch (mY)
        {
            case 0: mX = max(0, min(mX, 4)); break;
            case 1: mX = max(0, min(mX, 0)); break;
            case 2: mX = max(0, min(mX, 3)); break;
            case 3: mY = 2; break;
        }
    }
    
    virtual void OnTimer() override
    {
        if (mDemo)
            DoDemo();
    }
        
    void DoDemo()
    {
        if (++mDemoZone > 4)
            mDemoZone = 1;
        StartZone(mDemoZone, 30);
    }
    
    void StopDemo()
    {
        StopZone(mDemoZone);
    }
    
private:
    char* toString(int n)
    {
        static char temp[16];
        sprintf(temp, "%d", n);
        return temp;
    }
    
    char* toChar(int n)
    {
        static char temp[2];
        sprintf(temp, "%c", 'A'-1+n);
        return temp;
    }
};

class CApp : public CAppPage
{
public:
    //CApp() : CAppPage() {}
    
    void Create()
    {
        CWnd::Create("application", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
        
        CONSOLE::colorBack = RGB565(4040b0);
        CONSOLE::colorFront = RGB565(aaaaaa);
        APP::Init("Hunter X-Core controller");
        CONSOLE::window = CRect(8, 14, BIOS::LCD::Width-8, 14+14*10);
        CONSOLE::cursor = CONSOLE::window.TopLeft();

        DrawLayout();
        APP::Status("AC1: n/c, AC2: +3V, REM: P1");
        HunterInit();
        
        SetTimer(5000);
    }
    
    void Destroy()
    {
        HunterDeinit();
    }
    
    void DrawLayout()
    {
        DrawMenu();
        DrawPage();
    }

    virtual void OnKey(int key) override
    {
        if (key == BIOS::KEY::Up && mY > 0)
            mY--;
        if (key == BIOS::KEY::Down)
            mY++;
        if (key == BIOS::KEY::Left)
            mX--;
        if (key == BIOS::KEY::Right)
            mX++;

        CAppPage::OnKey(key);
        DrawPage();
    }
};

CApp app;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    app.Create();
    BIOS::KEY::EKey key;
    while ((key = KEY::GetKey()) != BIOS::KEY::EKey::Escape)
    {
        if (key != BIOS::KEY::EKey::None)
            app.OnKey(key);
        app.WindowMessage(CWnd::WmTick);
    }
    app.Destroy();
    
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    CONSOLE::Color(RGB565(ffff00));
    CONSOLE::Print("Assertion failed in ");
    CONSOLE::Print(file);
    CONSOLE::Print(" [%d]: %s\n", line, cond);
#ifdef __APPLE__
    //kill(getpid(), SIGSTOP);
#endif
    while (1);
}

