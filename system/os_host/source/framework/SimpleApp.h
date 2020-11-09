namespace APP
{
    #ifndef CONSOLE_MODE_B
    uint16_t backgroundColor = RGB565(0000b0);
    #endif
    CRect client(0, 0, BIOS::LCD::Width, BIOS::LCD::Height);

    void Title(char* title)
    {
        CRect rc1(client);
        rc1.bottom = 14;
        GUI::Background(rc1, RGB565(4040b0), RGB565(404040));
        BIOS::LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, title);
    }

    void Status(const char* status)
    {
        CRect rc2(client);
        rc2.top = rc2.bottom-14;
        GUI::Background(rc2, RGB565(404040), RGB565(202020));
        BIOS::LCD::Print(8, rc2.top, RGB565(808080), RGBTRANS, status);
    }

    void Status(char* status)
    {
        Status((const char*)status);
    }

void Init(const char* title)
    {
#ifdef CONSOLE_MODE_B
        CONSOLE::window = CRect(8, 16, BIOS::LCD::Width-8, BIOS::LCD::Height-14);
        CONSOLE::cursor = CONSOLE::window.TopLeft();
        BIOS::LCD::Bar(client, CONSOLE::colorBack);
#else
        BIOS::LCD::Bar(client, backgroundColor);
#endif
        Title((char*)title);
        Status((char*)"");
    }
}
