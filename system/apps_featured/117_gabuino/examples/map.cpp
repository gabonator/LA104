#include <library.h>

int main(void)
{
    BIOS::DBG::Print("<div style='position:relative;'>");
    BIOS::DBG::Print("<div style='position:absolute; left:0px; top:80px; width:400px; height:200px;'><img src='https://www.world-maps.org/images/wor32k6.jpg'></div>");
    BIOS::DBG::Print("<div style='position:absolute; left:60px; top:120px; width:8px; height:8px; background:#ff0000;'>On top</div>");
    BIOS::DBG::Print("</div>");
    return 0;
}
