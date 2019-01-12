class CPalette
{
public:
    static int GetColor(int s)
    {
        static const int colors[] = {RGB565(ff0000), RGB565(00ff00), RGB565(0000ff), RGB565(ffff00)};
        return colors[s];
    }
};