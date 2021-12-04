#include <library.h>

uint32_t color = RGB32(255, 0, 0);

int main(void)
{
    BIOS::DBG::Print(R"(
    <input type="color" value="#ff0000">
    <script>
      document
        .querySelector("input[type='color']")
        .addEventListener("input", updateColor, false);
        
      function updateColor()
      {
        var html = event.target.value;
        var color = parseInt("0x"+html.substr(1));
        BIOS.memWrite(colorPtr, [color >> 16, color >> 8, color]);
      }
    </script>)");
    BIOS::DBG::Print(R"(<script>colorPtr = 0x%08x;</script>)", &color);

    int padding = 8;
    CRect rect(padding, 14 + padding, 
        BIOS::LCD::Width - padding, BIOS::LCD::Height - 14 - padding);
        
    while (BIOS::KEY::GetKey() == BIOS::KEY::EKey::None)
    {
        BIOS::LCD::Bar(rect, RGB32TO565(color));
        BIOS::SYS::DelayMs(20);
    }
    return 0;
}

