namespace MyGui
{
	constexpr uint16_t TitleColor = RGB565(b0b0b0);
	constexpr uint16_t BackgroundColor = RGB565(808080);
	constexpr uint16_t LabelColor = RGB565(b0b0b0);
	constexpr uint16_t AxisTickColor = RGB565(b0b0b0);
	constexpr uint16_t AxisZeroColor = RGB565(b0b0b0);

	void Window(const CRect& rcRect, uint16_t clr)
	{
	  CRect rcClient = rcRect;
	  BIOS::LCD::RoundRect(rcClient, RGB565(000000));
	  rcClient.Deflate(2, 2, 2, 2);
	  BIOS::LCD::RoundRect(rcClient, clr);
	  rcClient.Deflate(2, 14, 2, 2);
	  BIOS::LCD::RoundRect(rcClient, BackgroundColor);
	}
}

class CPalette
{
public:
    static int GetColor(int s)
    {
        static const int colors[] = {
            RGB565(ff0000), RGB565(008000), RGB565(0000ff), RGB565(ffff00),
            RGB565(00ffff), RGB565(ff00ff), RGB565(000000), RGB565(ffffff)
        };
        return colors[s];
    }
};
