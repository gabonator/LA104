#include <library.h>

using namespace BIOS;
char msg[128];

typedef char* string;

void getToken(string& p, char* token, char delim)
{
    while(*p != 0)
    {
        if (*p == delim)
        {
            *token = 0;
            *p++;
            return;
        }
        *token++ = *p++;
    }
    *token = 0;
    return;
}

int ord(char c) 
{
    //_ASSERT(c >= '0' && c <= '9');
    return c - '0';
}

bool parseDegree(char* degree, int& deg, int &min, int& dec)
{
    if (strlen(degree) == 9 && degree[4] == '.')
    {
        deg = ord(degree[0])*10 + ord(degree[1]);
        min = ord(degree[2])*10 + ord(degree[3]);
        dec = ord(degree[5])*1000 + ord(degree[6])*100 +
          ord(degree[7])*10 + ord(degree[8]);
        return true;
    }
    
    if (strlen(degree) == 10 && degree[5] == '.')
    {
        deg = ord(degree[0])*100 + ord(degree[1])*10 +
            ord(degree[2])*1;
        min = ord(degree[3])*10 + ord(degree[4]);
        dec = ord(degree[6])*1000 + ord(degree[7])*100 +
          ord(degree[8])*10 + ord(degree[9]);
        return true;
    }
    return false;
}

bool parseTime(char* str, int& h, int& m, int& s)
{
    if (strlen(str) != 10 || str[6] != '.')
        return false;
    h = ord(str[0])*10 + ord(str[1]);
    m = ord(str[2])*10 + ord(str[3]);
    s = ord(str[4])*10 + ord(str[5]);
    return true;
}

void process(char* p)
{
    // http://www.technoblogy.com/show?SJ0
    // $GNRMC,180159.212,A,4808.8024,N,01708.5613,E,1.95,46.83,020821,,,A*4D
    char token[32];
    getToken(p, token, ',');
    if (strcmp(token, "$GNRMC") != 0)
        return;

    getToken(p, token, ','); // time
    int hour, min, sec;
    if (!parseTime(token, hour, min, sec))
        return;

    getToken(p, token, ','); 
    if (strcmp(token, "A") != 0) // has valid signal
    {
        DBG::Print("No signal");
        return;
    }

    int latDeg, latMin, latMinDec;
    getToken(p, token, ','); // latitude coord
    if (!parseDegree(token, latDeg, latMin, latMinDec))
        return;
    getToken(p, token, ','); // latitude N/S
    int latSgn = strcmp(token, "S") == 0 ? -1 : 1;

    int lonDeg, lonMin, lonMinDec;
    getToken(p, token, ','); // longitude coord
    if (!parseDegree(token, lonDeg, lonMin, lonMinDec))
        return;
    getToken(p, token, ','); // longitude E/W
    int lonSgn = strcmp(token, "W") == 0 ? -1 : 1;

    DBG::Print("<script>onData({h:%d,m:%d,s:%d,"
      "lat:%s(%d+(%d.%04d)/60),"
      "lon:%s(%d+(%d.%04d)/60)})</script>",
      hour, min, sec, 
      latSgn==-1 ? "-" : "", latDeg, latMin, latMinDec,
      lonSgn==-1 ? "-" : "", lonDeg, lonMin, lonMinDec);
      /*
    DBG::Print("%02d:%02d:%02d GMT lat=%d&deg; %d.%04d, lon=%d&deg; %d.%04d\n", 
        hour, min, sec,
        latDeg*latSgn, latMin, latMinDec,
        lonDeg*lonSgn, lonMin, lonMinDec);
        */
}

int main(void)
{
    BIOS::DBG::Print(R"(
<div id="time"></div>
<div style='position:relative; width:500px; height:250px; background:#ffff00'>
<div style='position:absolute; left:0px; top:0px; width:500px; height:250px; background:url(https://www.world-maps.org/images/wor32k6.jpg); background-size: 500px 250px; '><div>
<div id='marker' style='position:absolute; width:48px; height:48px; background:url(https://img.icons8.com/fluent/48/000000/marker-storm.png);'></div>
</div>
)");

BIOS::DBG::Print(R"(<script>
  function onData(x) 
  { 
    console.log(x); 
    var m = document.getElementById("marker");
    m.style.left = (x.lon+180)/360*500 - 24 + "px";
    m.style.top = (-x.lat+90)/180*250 - 48 + "px";
    document.getElementById("time").innerHTML = "GPS time: " + 
        [x.h, x.m, x.s].map(v=>("0"+v).substr(-2)).join(":");
  }
</script>)");



    //DBG::Print("<script>function onData(x) { console.log(x); }</script>");    
    //process((char*)"$GNRMC,180159.212,A,4808.8024,N,01708.5613,E,1.95,46.83,020821,,,A*4D");
    //return 123;
    
    BIOS::GPIO::UART::Setup(9600, BIOS::GPIO::UART::length8);
    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Uart);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Uart);

    BIOS::KEY::EKey key;
    while ((key = KEY::GetKey()) != BIOS::KEY::EKey::Escape)
    {
        int i = 0;
        while (GPIO::UART::Available())
        {
            char ch = GPIO::UART::Read();
            if (ch == '\x0d' || ch == '\x0a')
            {
                msg[i] = 0;
                if (i > 0)
                    process(msg);
                i = 0;
            }
            else
            if ( i < COUNT(msg)-1)
                msg[i++] = ch;
        }
        msg[i] = 0;

      SYS::DelayMs(100);
    }
    
    return 0;
}
