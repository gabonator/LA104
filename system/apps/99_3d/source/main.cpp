#include <library.h>
#include "../../os_host/source/framework/SimpleApp.h"

void SetUpPoints();
bool MoveAround();
void Init();

extern bool lighting;
extern bool zooming;
extern int speed;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    APP::backgroundColor = RGB565(202020);
    APP::Init("3D rotating cube");
    APP::Status("");
    SetUpPoints();
    Init();
    int redraws = 0, lastRedraws = 0;
    BIOS::KEY::EKey key;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
    {
        if (key == BIOS::KEY::EKey::F1)
            lighting = !lighting;
        if (key == BIOS::KEY::EKey::F3)
            zooming = !zooming;
        if (key == BIOS::KEY::EKey::Left)
            speed ++;
        if (key == BIOS::KEY::EKey::Right && speed > 1)
            speed --;

        MoveAround();
        EVERY(1000)
        {
            int curRedraws = redraws - lastRedraws;
            lastRedraws = redraws;
            if (curRedraws > 0)
            {
                char status[64];
                sprintf(status, "%d fps, lighting: %s", curRedraws, lighting ? "Yes" : "No");
                APP::Status(status);
            }
        }
        redraws++;
    }
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}
