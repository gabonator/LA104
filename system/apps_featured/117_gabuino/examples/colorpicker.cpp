#include <library.h>

uint32_t color = RGB32(255, 0, 0);

int main(void)
{
    BIOS::DBG::Print(R"-(<input type="color" onChange="updateColor(this.value)" value="#ff0000">)-");

    BIOS::DBG::Print(R"(<script>
      function updateColor(html)
      {
        var color = parseInt("0x"+html.substr(1));
        BIOS.memWrite(colorPtr, [color >> 16, color >> 8, color]);
      }
    </script>)");
    BIOS::DBG::Print(R"(<script>colorPtr = 0x%08x;</script>)", &color);

    while (BIOS::KEY::GetKey() == BIOS::KEY::EKey::None)
    {
        BIOS::LCD::Bar(40, 40, BIOS::LCD::Width-40, BIOS::LCD::Height-40, 
            RGB32TO565(color));
        BIOS::SYS::DelayMs(20);
    }
    return 0;
}
