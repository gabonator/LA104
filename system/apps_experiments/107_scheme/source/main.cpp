#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include "json.h"
#include "data.h"
#include "draw.h"

void drawVectors(int basex, int basey, int* vectors, int length, int rotation)
{
    for (int i=0; i<length; i+=4)
    {
        int x1 = basex + vectors[i];
        int y1 = basey + vectors[i+1];
        int x2 = basex + vectors[i+2];
        int y2 = basey + vectors[i+3];
        drawline_aa(x1*15, 200*256-y1*15, x2*15, 200*256-y2*15, RGB565(b00000));
    }
}

bool setup()
{
    CJson json(p);
    json["wires"].ForEach([](const CSubstring& s){
        CJson wire(s);
        int x1 = wire["x1"].GetNumber();
        int y1 = wire["y1"].GetNumber();
        int x2 = wire["x2"].GetNumber();
        int y2 = wire["y2"].GetNumber();
        drawline_aa(x1*15, 200*256-y1*15, x2*15, 200*256-y2*15, RGB565(008000));
    });
    json["junctions"].ForEach([](const CSubstring& s){
        CJson junction(s);
        int x = junction["x"].GetNumber()*15/256;
        int y = 200-junction["y"].GetNumber()*15/256;
        BIOS::LCD::Bar(x-2, y-2, x+2, y+2, RGB565(008000));
    });
    json["instances"].ForEach([](const CSubstring& s){
        CJson instance(s);
        
        int x = instance["x"].GetNumber();
        int y = instance["y"].GetNumber();
        int r = instance["rot"].GetNumber();
        char* device = instance["device"].GetString();
        
        if (strcmp(device, "LED") == 0)
        {
            int vectors[] = {
                -250, 0, 0, 0,
                0, -160, 0, 160,
                0, -160, 300, 0,
                0, 160, 300, 0,
                300, -160, 300, 160,
                300, 0, 500, 0,
                // arrows
                240, 220, 240-100, 220-100,
                  240, 220, 240-20, 220-80,
                  240, 220, 240-80, 220-20,
                360, 220, 360-100, 220-100,
                  360, 220, 360-20, 220-80,
                  360, 220, 360-80, 220-20,
            };
            drawVectors(x, y, vectors, COUNT(vectors), r-90);
        } else
        if (strcmp(device, "R-EU_") == 0)
        {
            int vectors[] = {
                -500, 0, -300, 0,
                300, 0, 500, 0,
                -300, 100, 300, 100,
                -300, -100, 300, -100,
                -300, -100, -300, 100,
                300, -100, 300, 100,
            };
            drawVectors(x, y, vectors, COUNT(vectors), r);
        } else
        if (strcmp(device, "MA04-1") == 0)
        {
            int vectors[] = {
                -100, -750, -100, 500,
                400, -750, 400, 500,
                -100, -750, 400, -750,
                -100, 500, 400, 500,
                100, 0, 800, 0,
                100, 250, 800, 250,
                100, -250, 800, -250,
                100, -500, 800, -500,

                120, 0+20, 280, 0+20,
                120, 250+20, 280, 250+20,
                120, -250+20, 280, -250+20,
                120, -500+20, 280, -500+20,

                120, 0-20, 280, 0-20,
                120, 250-20, 280, 250-20,
                120, -250-20, 280, -250-20,
                120, -500-20, 280, -500-20,

            };
            drawVectors(x, y, vectors, COUNT(vectors), r);
        } else
        {
            int x = instance["x"].GetNumber()*15/256;
            int y = 200-instance["y"].GetNumber()*15/256;
            BIOS::LCD::Bar(x-2, y-2, x+2, y+2, RGB565(ff));
        }        
    });

    return true;
}

void loop(BIOS::KEY::EKey key)
{
}


#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    CONSOLE::colorBack = RGB565(ffffff);

    APP::Init("Eagle scheme");
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
