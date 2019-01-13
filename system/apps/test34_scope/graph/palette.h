class CPalette
{
public:
    static int GetColor(int s)
    {
        static const int colors[] = {
            RGB565(ff0000), RGB565(00b000), RGB565(0000ff), RGB565(ffff00),
            RGB565(00ffff), RGB565(ff00ff), RGB565(000000), RGB565(ffffff)
        };
        return colors[s];
    }
};
