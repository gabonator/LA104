#include <library.h>
using namespace BIOS;

#include "SPI.h"
#include "WS2812B.h"
#include "colorspace.h"

#define RGBW32(r,g,b,w) ((r) | ((g)<<8) | ((b)<<16) | ((w)<<24))
#define GetColorW(rgbw) (((rgbw) >> 24)&0xff)

template <typename T> T minmax(T min, T val, T max)
{
	if (val < min)
		return min;
	if (val > max)
		return max;
	return val;
}

template <typename T> T enumclamp(int val, T max)
{
	if (val < 0)
		return (T)0;
	if (val > (int)max-1)
		return (T)((int)max-1);
	return (T)val;
}

class CSettings
{
	static CSettings* mSettings;
	
public:
	CSettings()
	{
		mSettings = this;
	}
	
	static CSettings& Settings()
	{
		_ASSERT(mSettings);
		return *mSettings;
	}
	
public:
	enum class EDriver { WS2812RGB, WS2812RGBW, Total } mDriver {EDriver::WS2812RGB};
	int mLedCount{24};
	enum class EPattern { UserColor, UserGradient, Black, Gray25, Gray50, Gray75, White, Red, Green, Blue, Gradient, Ring, Colors, Marching1, Marching3, MarchingSoft, NightSky, Run, Total} mPattern {EPattern::Gradient};
	enum class EFilter { None, Red, Green, Blue, Grayscale, Inverted, First, Strobo, Detector, Total} mFilter {EFilter::None};
	int mBrightness{64};
	int mSpeed{256};
	
	const char* PatternName()
	{
		const char* modes[] = {"User color", "User gradient", "Black", "25% Gray", "50% Gray", "75% Gray", "White", "Red", "Green", "Blue", "Gradient", "Ring", "Colors", "Marching1", "Marching3", "Marching soft",
			"Night sky", "Run"
		};
		return modes[(int)mPattern];
	}
	
	const char* FilterName()
	{
		const char* modes[] = {"None", "Red", "Green", "Blue", "Grayscale", "Inverted", "First pixel", "Strobo", "QP Detector"};
		return modes[(int)mFilter];
	}
	
	const char* PlayState()
	{
		return "Playing";
	}

	const char* DriverName()
	{
		const char* modes[] = {"WS2812 RGB", "WS2812 RGBW"};
		return modes[(int)mDriver];
	}

	int BytesPerPixel()
	{
		int modes[] = {3, 4};
		return modes[(int)mDriver];
	}
};

class CDriver
{
	static CDriver* mDriver;
	WS2812B mWs2812b;
	
public:
	static const int MaxCount = WS2812B_MAXCOUNT;
	
public:
	CDriver() : mWs2812b(MaxCount)
	{
		mDriver = this;
	}

	static CDriver& Driver()
	{
		_ASSERT(mDriver);
		return *mDriver;
	}

	void Update(const uint32_t* colorPattern, int colorCount)
	{
		int brightness = CSettings::Settings().mBrightness;
		int bytesPerPixel = CSettings::Settings().BytesPerPixel();

		if (mWs2812b.numPixels() != colorCount || mWs2812b.getBytesPerPixel() != bytesPerPixel)
		{
			mWs2812b.setBytesPerPixel(bytesPerPixel);
			mWs2812b.updateLength(colorCount);
		}
		
		for (int i=0; i<colorCount; i++)
		{
			uint32_t color32 = colorPattern[i];
			if (brightness == 256)
				mWs2812b.setPixelColor(i, color32);
			else
			{
				mWs2812b.setPixelColor(i, mWs2812b.Color(
					GetColorR(color32)*brightness>>8,
					GetColorG(color32)*brightness>>8,
					GetColorB(color32)*brightness>>8)); //TODO
			}
		}
		mWs2812b.show();
	}
};

class CPatternGenerator
{
	uint32_t mTempMemory[WS2812B_MAXCOUNT];
	static const uint8_t randomized[256];
public:
	uint32_t Interpolate( uint32_t clrA, uint32_t clrB, uint32_t nLevel )
	{
		int br = GetColorR(clrB)*nLevel;
		int bg = GetColorG(clrB)*nLevel;
		int bb = GetColorB(clrB)*nLevel;
		int bw = GetColorW(clrB)*nLevel;
		nLevel = 255-nLevel;
		int ar = GetColorR(clrA)*nLevel;
		int ag = GetColorG(clrA)*nLevel;
		int ab = GetColorB(clrA)*nLevel;
		int aw = GetColorW(clrA)*nLevel;
		ar = (ar+br) / 256;
		ag = (ag+bg) / 256;
		ab = (ab+bb) / 256;
		aw = (aw+bw) / 256;
		return RGBW32(ar, ag, ab, aw);
	}

	inline float xabs(float x)
	{
		if (x>=0)
			return x;
		return -x;
	}
	
	float xsin(float x)
	{
		const float tp = 1.f/(2.f*3.14159265f);
		x *= tp;
		x -= .25f + (int)(x + .25f);
		x *= 16.f * (xabs(x) - .5f);
		return x;
	}
	
	uint32_t GetColorGradient(int i, int time)
	{
		float a = time * 0.001f;
		const float q = 0.4f/3.0f;
		
		int r, g, b, w;
		r = 128 + xsin(a+i*q)*128;
		g = 128 + xsin(a*0.37f+i*q)*128;
		b = 128 + xsin(a*0.11f+i*q)*128;
		w = 128 + xsin(a*0.07f+i*q)*128;

		r = max(0, min(r, 255));
		g = max(0, min(g, 255));
		b = max(0, min(b, 255));
		w = max(0, min(w, 255));

		return RGBW32(r, g, b, w);
	}
		
	uint32_t GetColorRing(int i, int time)
	{
		static uint32_t refColor = 0;
		static long refTime = 0;
		if (time != refTime)
		{
			refTime = time;
			refColor = GetColorGradient(0, time/4);
		}
		
		int n = 24;

		i *= 256;
		int f = (time*3)%(256*n);
		int diff = i-f;
		int dist1 = abs(diff);
		int dist2 = abs(diff + 256*n);
		int dist3 = abs(diff - 256*n);
		int dist = min(dist1, min(dist2, dist3));
		
		int l = 255-dist/4;
		if (l<0)
			l = 0;
		
		int r = GetColorR(refColor) * l / 256;
		int g = GetColorG(refColor) * l / 256;
		int b = GetColorB(refColor) * l / 256;
		int w = GetColorB(refColor) * l / 256;

		r = max(0, min(r, 255));
		g = max(0, min(g, 255));
		b = max(0, min(b, 255));
		w = max(0, min(w, 255));

		return RGBW32(r, g, b, w);
	}
	
	uint32_t GetColorColors(int i, int time)
	{
		const static uint32_t colors[] = {0x00ff0000, 0x40ffff00, 0x8000ff00,
			0xff00ffff, 0x800000ff, 0x40ff00ff};
		
 		int index = time/1000;
		int part = time%1000;
		int N = 24;

		if (i > N*part/1000)
		  return colors[index % COUNT(colors)];
		else
		  return colors[(index+1) % COUNT(colors)];
	}
	
	uint32_t GetColorMarch1(int i, int time)
	{
		const static uint32_t colors[] = {0xff0000, 0x00ff00, 0x0000ff};

		time /= 256;
		return colors[(time+i)%COUNT(colors)];
	}

	uint32_t GetColorMarch3(int i, int time)
	{
		const static uint32_t colors[] = {
			0xff0000, 0xff0000, 0xff0000,
			0x00ff00, 0x00ff00, 0x00ff00,
			0x0000ff, 0x0000ff, 0x0000ff};

		time /= 256;
		return colors[(time+i)%COUNT(colors)];
	}

	uint32_t GetColorSoftMarch(int i, int time)
	{
		const static uint32_t colors[] = {
			0xff0000, 0xff0000, 0xff0000,
			0x00ff00, 0x00ff00, 0x00ff00,
			0x0000ff, 0x0000ff, 0x0000ff};

		int partial = time & 255;
		time /= 256;
		uint32_t colorA = colors[(time+i)%COUNT(colors)];
		uint32_t colorB = colors[(time+i+1)%COUNT(colors)];
		return Interpolate(colorA, colorB, partial);
	}

	uint32_t GetColorNightSky(int i, int time)
	{
		int r = 0;
		int g = 0;
		int b = randomized[(time + i)&255] >> 3; // 8 levels
		
		int dot = time >> 10;
		int dotindex = randomized[dot & 255] & 31;
		if (i == dotindex)
		{
			int subtime = time & 1023;
			if (subtime < 256)
				r = g = subtime;
			else if (subtime < 512)
				r = g = 512 - subtime;
		}
		return RGB32(r, g, b);
	}
	
	uint32_t GetColorRun(int i, int time)
	{
		constexpr int Period = 128;
		constexpr int Strength = 700;
		i = i*256;
		int current = ((time*5) & (256*Period-1)) - Strength;
		int dist = abs(i-current);
		if (dist > Strength)
			return 0;
		dist = Strength-dist;
		if (dist > 255)
			dist = 255;
		return RGBW32(dist, dist, dist, dist);
	}

	uint32_t GetPatternColor(int i, int time)
	{
		switch (CSettings::Settings().mPattern)
		{
			case CSettings::EPattern::UserColor: _ASSERT(0);
			case CSettings::EPattern::UserGradient: _ASSERT(0);
			case CSettings::EPattern::Black: return RGB32(0, 0, 0);
			case CSettings::EPattern::Gray25: return RGBW32(64, 64, 64, 64);
			case CSettings::EPattern::Gray50: return RGBW32(128, 128, 128, 128);
			case CSettings::EPattern::Gray75: return RGBW32(192, 192, 192, 192);
			case CSettings::EPattern::White: return RGBW32(255, 255, 255, 255);
			case CSettings::EPattern::Red: return RGB32(255, 0, 0);
			case CSettings::EPattern::Green: return RGB32(0, 255, 0);
			case CSettings::EPattern::Blue: return RGB32(0, 0, 255);
			case CSettings::EPattern::Gradient: return GetColorGradient(i, time);
			case CSettings::EPattern::Ring: return GetColorRing(i, time);
			case CSettings::EPattern::Colors: return GetColorColors(i, time);
			case CSettings::EPattern::Marching1: return GetColorMarch1(i, time);
			case CSettings::EPattern::Marching3: return GetColorMarch3(i, time);
			case CSettings::EPattern::MarchingSoft: return GetColorSoftMarch(i, time);
			case CSettings::EPattern::NightSky: return GetColorNightSky(i, time);
			case CSettings::EPattern::Run: return GetColorRun(i, time);
			default:
				_ASSERT(0);
		}
		return 0;
	}

	uint32_t GetColor(int i, int time)
	{
		CSettings::EFilter eFilter = CSettings::Settings().mFilter;
		if (eFilter == CSettings::EFilter::First)
			return GetPatternColor(0, time);

		uint32_t color = GetPatternColor(i, time);
		switch (eFilter)
		{
			case CSettings::EFilter::None: return color;
			case CSettings::EFilter::Red: return color & RGB32(255, 0, 0);
			case CSettings::EFilter::Green: return color & RGB32(0, 255, 0);
			case CSettings::EFilter::Blue: return color & RGB32(0, 0, 255);
			case CSettings::EFilter::Grayscale:
			{
				int t = GetColorR(color) + GetColorG(color) + GetColorB(color);
				t /= 3;
				return RGBW32(t, t, t, t);
			}
			case CSettings::EFilter::Inverted:
				return RGBW32(255 - GetColorR(color), 255 - GetColorG(color), 255 - GetColorB(color), 255 - GetColorW(color));
				break;
			case CSettings::EFilter::Strobo:
			{
				static int nPass = 0;
				static int nLastTime = 0;
				if (time != nLastTime)
				{
					nLastTime = time;
					nPass++;
				}

				if (nPass % 8 == 0)
					return color;
				else
					return 0;
				break;
			}
			case CSettings::EFilter::Detector:
			{
				uint32_t& memColor = mTempMemory[i];
				int r = GetColorR(memColor) - 3;
				int g = GetColorG(memColor) - 3;
				int b = GetColorB(memColor) - 3;
				int w = GetColorW(memColor) - 3;
				int nr = GetColorR(color);
				int ng = GetColorG(color);
				int nb = GetColorB(color);
				int nw = GetColorW(color);
				r = max(max(r, nr), 0);
				g = max(max(g, ng), 0);
				b = max(max(b, nb), 0);
				w = max(max(w, nw), 0);
				memColor = RGBW32(r, g, b, w);
				return memColor;
			}
			default:
				_ASSERT(0);
		}
		return color;
	}
};

// PRNG generator
const uint8_t CPatternGenerator::randomized[256] = {184, 74, 51, 56, 238, 145, 171, 89, 183, 55, 88, 79, 218, 78, 69, 28, 21, 27, 126, 158, 46, 176, 30, 110, 72, 191, 247, 159, 224, 19, 169, 67, 246, 189, 234, 138, 58, 61, 103, 211, 187, 235, 141, 166, 23, 68, 45, 146, 174, 252, 185, 22, 121, 95, 76, 231, 26, 71, 232, 157, 66, 219, 2, 102, 44, 207, 206, 13, 177, 81, 101, 20, 7, 151, 149, 98, 40, 182, 15, 49, 210, 24, 131, 163, 42, 124, 14, 65, 97, 202, 99, 94, 106, 250, 70, 114, 140, 16, 128, 34, 54, 168, 77, 33, 90, 35, 155, 4, 170, 29, 31, 217, 96, 32, 83, 143, 57, 111, 192, 226, 199, 10, 173, 37, 17, 222, 80, 115, 63, 133, 130, 172, 59, 119, 167, 12, 244, 52, 82, 227, 208, 53, 9, 123, 91, 100, 136, 228, 197, 236, 164, 239, 50, 86, 25, 75, 230, 175, 195, 142, 0, 144, 237, 251, 198, 178, 73, 240, 225, 132, 87, 135, 221, 253, 220, 92, 93, 201, 120, 161, 112, 41, 249, 60, 214, 150, 156, 134, 229, 118, 5, 162, 254, 116, 204, 1, 188, 205, 160, 245, 212, 180, 190, 196, 62, 117, 233, 203, 200, 107, 193, 137, 38, 125, 109, 181, 153, 8, 241, 152, 64, 113, 127, 108, 179, 194, 3, 43, 84, 216, 18, 243, 11, 6, 223, 248, 165, 48, 104, 148, 215, 242, 186, 129, 147, 139, 213, 209, 255, 47, 154, 36, 105, 85, 39, 122};


using CGetterFunction = int(*)(void);
using CSetterFunction = void(*)(int);

class CAssignable
{
	CGetterFunction mGetter;
	CSetterFunction mSetter;
public:
	CAssignable()
	{
		mGetter = [](){_ASSERT(0); return 0;};
		mSetter = [](int){_ASSERT(0);};
	}
	CAssignable(CGetterFunction getter, CSetterFunction setter)
	{
		mGetter = getter;
		mSetter = setter;
	}
	int operator=(int v)
	{
		mSetter(v);
		return v;
	}
	operator int()
	{
		return mGetter();
	}
};

class CPattern
{
	uint32_t mColors[CDriver::MaxCount];
	CPatternGenerator mGenerator;
	long lTime{0};
	long lLastTick{0};
	
	CAssignable mAssignable;
	int mIndex;
	
public:
	CPattern()
	{
		static CPattern* _this = this;
		mAssignable = CAssignable([]()
		   {
				return _this->Get(_this->mIndex);
		   }, [](int color)
		   {
				_this->Set(_this->mIndex, color);
		   });
	}
	
	void Update()
	{
		if (CSettings::Settings().mPattern == CSettings::EPattern::UserColor || CSettings::Settings().mPattern == CSettings::EPattern::UserGradient)
			return;
		
		long lCurrentTick = BIOS::SYS::GetTick();
		if (lLastTick == 0)
			lLastTick = lCurrentTick;
		
		if (CSettings::Settings().mSpeed == 256)
			lTime += lCurrentTick - lLastTick;
		else
			lTime += (lCurrentTick - lLastTick) * CSettings::Settings().mSpeed >> 8;
		
		lLastTick = lCurrentTick;

		const int count = CSettings::Settings().mLedCount;
		for (int i=0; i<count; i++)
			mColors[i] = mGenerator.GetColor(i, lTime);
	}
	
	int Get(int i)
	{
		return mColors[i];
	}
	
	void Set(int i, int color)
	{
		if (CSettings::Settings().mPattern == CSettings::EPattern::UserColor)
		{
			for (int i=0; i<CSettings::Settings().mLedCount; i++)
				mColors[i] = color;
			return;
		}
		if (CSettings::Settings().mPattern == CSettings::EPattern::UserGradient)
		{
			mColors[i] = color;
			return;
		}
		_ASSERT(0);
	}
	
	CAssignable& operator[](int i)
	{
		mIndex = i;
		return mAssignable;
	}
	
	const uint32_t* GetBuffer()
	{
		return mColors;
	}
	
	int GetSize()
	{
		return CSettings::Settings().mLedCount;
	}
};

class CBlock : public CWnd
{
public:
	virtual void OnPaint() override
	{
		CRect rcInside(m_rcClient);
		rcInside.Deflate(1, 1, 1, 1);
		GUI::Background(rcInside, RGB565(101010), RGB565(101010));

		if (HasFocus())
		{
			BIOS::LCD::Rectangle(m_rcClient, RGB565(b0b0b0));
			BIOS::LCD::Print(m_rcClient.left, m_rcClient.top-14, RGB565(ffffff), RGB565(b0b0b0), m_pszId);
		}
		else
		{
			BIOS::LCD::Rectangle(m_rcClient, RGB565(b0b0b0));
			BIOS::LCD::Print(m_rcClient.left, m_rcClient.top-14, RGB565(000000), RGB565(b0b0b0), m_pszId);
		}
	}
};

class CAniControl : public CBlock
{
public:
	const int Height = 66;
	int mFocus = 5;
	
	bool HasFocus(int i)
	{
		return CWnd::HasFocus() && mFocus == i;
	}
	
	virtual void OnPaint() override
	{
		#define FOCUS(x) HasFocus(x) ? RGBTRANS : RGB565(ffffff), HasFocus(x) ? RGB565(ffffff) : RGBTRANS, HasFocus(x)
		
		CBlock::OnPaint();

		int _x = m_rcClient.left+8, _y = m_rcClient.top+4+14*0;
		_x += BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "Driver: ");
		_x += BIOS::LCD::Printf(_x, _y, FOCUS(0) ? "<%d>" : "%d", CSettings::Settings().mLedCount);
		_x += BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "x");
		_x += 8;
		_x += BIOS::LCD::Printf(_x, _y, FOCUS(1) ? "<%s>" : "%s", CSettings::Settings().DriverName());

		_x = m_rcClient.left+8, _y += 14;
		_x += BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "Pattern: ");
		_x += BIOS::LCD::Printf(_x, _y, FOCUS(2) ? "<%s>" : "%s", CSettings::Settings().PatternName());

		_x = m_rcClient.left+8, _y += 14;
		_x += BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "Filter: ");
		_x += BIOS::LCD::Printf(_x, _y, FOCUS(3) ? "<%s>" : "%s", CSettings::Settings().FilterName());

		_x = m_rcClient.left+8, _y += 14;
		_x += BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "Brightness: ");
		_x += BIOS::LCD::Printf(_x, _y, FOCUS(4) ? "<%d%%>" : "%d%%", CSettings::Settings().mBrightness*100/256);
		_x = BIOS::LCD::Width/2+20;
		_x += BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "Speed: ");
		_x += BIOS::LCD::Printf(_x, _y, FOCUS(5) ? "<%d%%>" : "%d%%", CSettings::Settings().mSpeed*100/256);

		// Driver: WS2812/B
		// Leds: 24
		// Preset: Red, Green, Blue, GaboAnim
		// Play Pause Single
	}
	
	virtual void OnKey(int key) override
	{
		int dir = 0;
		if (key == BIOS::KEY::Left)
			dir = -1;
		if (key == BIOS::KEY::Right)
			dir = +1;
		if (dir != 0)
		{
			switch (mFocus)
			{
				case 0:
					CSettings::Settings().mLedCount = minmax(1, CSettings::Settings().mLedCount + dir, 32);
					GetParent()->Invalidate();
					break;
				case 1:
					CSettings::Settings().mDriver = enumclamp((int)CSettings::Settings().mDriver + dir, CSettings::EDriver::Total);
					GetParent()->Invalidate();
					break;
				case 2:
					CSettings::Settings().mPattern = enumclamp((int)CSettings::Settings().mPattern + dir, CSettings::EPattern::Total);
					Invalidate();
					break;
				case 3:
					CSettings::Settings().mFilter = enumclamp((int)CSettings::Settings().mFilter + dir, CSettings::EFilter::Total);
					Invalidate();
					break;
				case 4:
					CSettings::Settings().mBrightness = minmax(0, CSettings::Settings().mBrightness + dir*8, 256);
					Invalidate();
					break;
				case 5:
					CSettings::Settings().mSpeed = minmax(-256*5, CSettings::Settings().mSpeed + dir*32, 256*5);
					Invalidate();
					break;
			}
		}
		if (key == BIOS::KEY::Up)
		{
			if (mFocus > 0)
			{
				mFocus--;
				Invalidate();
				return;
			}
		}
		if (key == BIOS::KEY::Down)
		{
			if (mFocus < 5)
			{
				mFocus++;
				Invalidate();
				return;
			}
		}
		CBlock::OnKey(key);
	}

};

class CPreview : public CBlock
{
public:
	const int Height = 14;
	int mSelected = 0;
	CPattern mPattern;
	WS2812B mLeds;
	long lTime = 0;

	CPreview() : mLeds(WS2812B_MAXCOUNT)
	{
		mLeds.begin();
	}
	
	virtual void OnPaint() override
	{
		CBlock::OnPaint();
		Redraw();
		mSelected = min(mSelected, CSettings::Settings().mLedCount-1);
		DrawCursor(mSelected);
	}

	void Update()
	{
		mPattern.Update();
	}
	
	void Redraw()
	{
		Update();
		CDriver::Driver().Update(mPattern.GetBuffer(), mPattern.GetSize());
		
		const int count = min(24, CSettings::Settings().mLedCount);
		for (int i=0; i<count; i++)
			DrawColor(i, mPattern[i]);
	}
	
	void SetColor(uint32_t c)
	{
		mPattern[mSelected] = c;
	}
	
	CRect GetRect(int i)
	{
		const int spacing = 4;
		int x1 = m_rcClient.left + spacing + i*(7+spacing);
		int x2 = x1 + 7;
		return CRect(x1, m_rcClient.top+4, x2, m_rcClient.top + 4+6);
	}
	
	void DrawColor(int i, uint32_t color32)
	{
		int color = RGB32TO565(color32);
		BIOS::LCD::Bar(GetRect(i), color);
	}
	
	void DrawCursor(int i)
	{
		if (i>24)
			return;
		
		CRect rcRect(GetRect(i));
		if (HasFocus())
		{
			rcRect.Inflate(1, 1, 1, 1);
			BIOS::LCD::Rectangle(rcRect, RGB565(d0d0d0));
			rcRect.Inflate(1, 1, 1, 1);
			BIOS::LCD::Rectangle(rcRect, RGB565(ffffff));
		} else
		{
			rcRect.Inflate(1, 1, 1, 1);
			BIOS::LCD::Rectangle(rcRect, RGB565(404040));
			rcRect.Inflate(1, 1, 1, 1);
			BIOS::LCD::Rectangle(rcRect, RGB565(404040));
		}
	}
	
	void ClearCursor(int i)
	{
		if (i>=24)
			return;
		
		CRect rcRect(GetRect(i));

		rcRect.Inflate(2, 2, 2, 2);
		GUI::Background(rcRect, RGB565(101010), RGB565(101010));

		uint32_t color32 = mPattern[i];
		DrawColor(i, color32);
	}

	uint32_t GetColor()
	{
		return mPattern[mSelected];
	}
	
	virtual void OnKey(int key) override
	{
		if (key == BIOS::KEY::Left)
		{
			if (mSelected > 0)
			{
				ClearCursor(mSelected);
				mSelected--;
				DrawCursor(mSelected);
			}
			return;
		}
		if (key == BIOS::KEY::Right)
		{
			if (mSelected < CSettings::Settings().mLedCount-1)
			{
				ClearCursor(mSelected);
				mSelected++;
				DrawCursor(mSelected);
			}
			return;
		}
		CBlock::OnKey(key);
	}

};

class CColorSelector : public CBlock
{
	int mFocus{0};
	uint32_t mLastColor{0};
	int mLastFocus{-1};
	
public:
	const int Height = 64;
	
	virtual void OnPaint() override
	{
		mLastFocus = -1;
		CBlock::OnPaint();
		SetColor(mLastColor);
	}

	void SetColor(const uint32_t color)
	{
		if (mLastColor == color && mLastFocus == mFocus)
			return;
		
		mLastColor = color;
		mLastFocus = mFocus;
		
		CRect rcClear(m_rcClient.left+8, m_rcClient.top+4, m_rcClient.left+8+32+32, m_rcClient.top+4+58);
		GUI::Background(rcClear, RGB565(101010), RGB565(101010));

		Colorspace::RgbColor rgb{(uint8_t)GetColorR(color), (uint8_t)GetColorG(color), (uint8_t)GetColorB(color), (uint8_t)GetColorW(color)};

		FocusableInt(0, m_rcClient.left+8, m_rcClient.top+4+14*0, "R:", rgb.r);
		FocusableInt(1, m_rcClient.left+8, m_rcClient.top+4+14*1, "G:", rgb.g);
		FocusableInt(2, m_rcClient.left+8, m_rcClient.top+4+14*2, "B:", rgb.b);
		if (CSettings::Settings().mDriver == CSettings::EDriver::WS2812RGB)
			BIOS::LCD::Printf(m_rcClient.left+8, m_rcClient.top+4+14*3, RGB565(b0b0b0), RGBTRANS, "W:");
		else
			FocusableInt(3, m_rcClient.left+8, m_rcClient.top+4+14*3, "W:", rgb.w);

		Colorspace::HsvColor hsv = Colorspace::RgbToHsv(rgb);

		CRect rcClear2(m_rcClient.left+8+100, m_rcClient.top+4, m_rcClient.left+8+100+80, m_rcClient.top+4+58);
		GUI::Background(rcClear2, RGB565(101010), RGB565(101010));

		FocusableInt(4, m_rcClient.left+8+100, m_rcClient.top+4+14*0, "H:", hsv.h);
		FocusableInt(5, m_rcClient.left+8+100, m_rcClient.top+4+14*1, "S:", hsv.s);
		FocusableInt(6, m_rcClient.left+8+100, m_rcClient.top+4+14*2, "L:", hsv.v);
		if (CSettings::Settings().mDriver == CSettings::EDriver::WS2812RGB)
			BIOS::LCD::Printf(m_rcClient.left+8+100, m_rcClient.top+4+14*3, RGB565(b0b0b0), RGBTRANS, "#%06x", color & 0xffffff);
		else
			BIOS::LCD::Printf(m_rcClient.left+8+100, m_rcClient.top+4+14*3, RGB565(b0b0b0), RGBTRANS, "#%08x", color);

		CRect rcPreview(m_rcClient.right-50, m_rcClient.top+4, m_rcClient.right-4, m_rcClient.bottom-4);
		BIOS::LCD::Bar(rcPreview, RGB32TO565(color));
		rcPreview.left = rcPreview.left-10;
		rcPreview.right = rcPreview.left+10;
		int h = rcPreview.Height()/4;
		BIOS::LCD::Bar(CRect(rcPreview.left, rcPreview.top, rcPreview.right, rcPreview.top + h), RGB565RGB(rgb.r, 0, 0));
		BIOS::LCD::Bar(CRect(rcPreview.left, rcPreview.top+h, rcPreview.right, rcPreview.top + h*2), RGB565RGB(0, rgb.g, 0));
		BIOS::LCD::Bar(CRect(rcPreview.left, rcPreview.top+h*2, rcPreview.right, rcPreview.top+h*3), RGB565RGB(0, 0, rgb.b));
		if (CSettings::Settings().mDriver == CSettings::EDriver::WS2812RGBW)
			BIOS::LCD::Bar(CRect(rcPreview.left, rcPreview.top+h*3, rcPreview.right, rcPreview.bottom), RGB565RGB(rgb.w, rgb.w, rgb.w));
	}
	
	virtual void OnKey(int key) override
	{
		int diff = 0;
		if (key == BIOS::KEY::Left)
			diff = -1;
		if (key == BIOS::KEY::Right)
			diff = +1;
		
		if (diff != 0)
		{
			if (CSettings::Settings().mPattern != CSettings::EPattern::UserColor &&
				CSettings::Settings().mPattern != CSettings::EPattern::UserGradient)
			{
				// Start editing
				CSettings::Settings().mPattern = CSettings::EPattern::UserGradient;
				GetParent()->Invalidate();
				return;
			} else
			{
				diff *= 8;
				int diffR = 0, diffG = 0, diffB = 0, diffW = 0;
				int diffH = 0, diffS = 0, diffV = 0;
				switch (mFocus)
				{
					case 0: diffR = diff; break;
					case 1: diffG = diff; break;
					case 2: diffB = diff; break;
					case 3: diffW = diff; break;
					case 4: diffH = diff; break;
					case 5: diffS = diff; break;
					case 6: diffV = diff; break;
				}

				Colorspace::RgbColor rgb{(uint8_t)GetColorR(mLastColor), (uint8_t)GetColorG(mLastColor), (uint8_t)GetColorB(mLastColor), (uint8_t)GetColorW(mLastColor)};
				
				if (diffR != 0 || diffG != 0 || diffB != 0 || diffW != 0)
				{
					rgb.r = minmax(0, rgb.r+diffR, 255);
					rgb.g = minmax(0, rgb.g+diffG, 255);
					rgb.b = minmax(0, rgb.b+diffB, 255);
					rgb.w = minmax(0, rgb.w+diffW, 255);
				}

				if (diffH != 0 || diffS != 0 || diffV != 0)
				{
					Colorspace::HsvColor hsv = Colorspace::RgbToHsv(rgb);
					hsv.h = hsv.h+diffH;
					hsv.s = minmax(0, hsv.s+diffS, 255);
					hsv.v = minmax(0, hsv.v+diffV, 255);
					rgb = Colorspace::HsvToRgb(hsv);
				}
				
				uint32_t newColor = RGBW32(rgb.r, rgb.g, rgb.b, rgb.w);
				if (newColor != mLastColor)
				{
					SetColor(newColor);
					SendMessage(GetParent(), 0, newColor);
				}
			}
			return;
		}
		
		if (key == BIOS::KEY::Up)
		{
			if (mFocus > 0)
			{
				mFocus--;
				if (CSettings::Settings().mDriver == CSettings::EDriver::WS2812RGB && mFocus == 3)
					mFocus--;
				
				SetColor(mLastColor);
				return;
			}
		}
		if (key == BIOS::KEY::Down)
		{
			if (mFocus < 6)
			{
				mFocus++;
				if (CSettings::Settings().mDriver == CSettings::EDriver::WS2812RGB && mFocus == 3)
					mFocus++;

				SetColor(mLastColor);
				return;
			}
		}
		CBlock::OnKey(key);		
	}

	void FocusableInt(int id, int x, int y, const char* prefix, int value)
	{
		if (id == -1)
			BIOS::LCD::Printf(x, y, RGB565(b0b0b0), RGBTRANS, "%s %d", prefix, value);
		else if (mFocus == id && HasFocus())
			BIOS::LCD::Printf(x, y, RGBTRANS, RGB565(ffffff), "%s <%d>", prefix, value);
		else
			BIOS::LCD::Printf(x, y, RGB565(ffffff), RGBTRANS, "%s %d", prefix, value);
	}
};

class CApplication : public CWnd, public CSettings
{
	CAniControl mControl;
	CPreview mPreview;
	CColorSelector mColor;
	
public:
	void Create()
	{
		CWnd::Create("Application", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
		
		const int padding = 20;
		
		mControl.Create(" Control ", CWnd::WsVisible, CRect(padding, 14+padding, BIOS::LCD::Width-padding, 14+padding + mControl.Height), this);
		mPreview.Create(" Preview ", CWnd::WsVisible, CRect(padding, mControl.m_rcClient.bottom + padding, BIOS::LCD::Width-padding, mControl.m_rcClient.bottom + padding + mPreview.Height), this);
		mColor.Create(" Color ", CWnd::WsVisible, CRect(padding, mPreview.m_rcClient.bottom + padding, BIOS::LCD::Width-padding, mPreview.m_rcClient.bottom + padding + mColor.Height), this);

		mColor.SetFocus();
		
		SetTimer(50);
	}
	
	void Destroy()
	{
        SPI.end();
	}
	
	virtual void OnTimer() override
	{
		mPreview.Redraw();
		EVERY(200)
		{
			mColor.SetColor(mPreview.GetColor());
		}
	}
	
	virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override
	{
		if (pSender == &mColor)
		{
			mPreview.SetColor(data);
		}
	}
	
private:
	virtual void OnPaint() override
	{
		GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
		
		CRect rc1(m_rcClient);
		rc1.bottom = 14;
		GUI::Background(rc1, RGB565(4040b0), RGB565(404040));
		BIOS::LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "WS2812 addressable led tester");
	}
};

CSettings* CSettings::mSettings;
CSettings set;
CDriver* CDriver::mDriver;
CDriver driver;
CApplication app;

void mainInit()
{
    app.Create();
    app.WindowMessage( CWnd::WmPaint );
}

bool mainLoop()
{
    BIOS::KEY::EKey key;
    key = KEY::GetKey();
    if (key != BIOS::KEY::None)
        app.WindowMessage(CWnd::WmKey, key);
    app.WindowMessage(CWnd::WmTick);

    return key != KEY::EKey::Escape;
}

void mainFinish()
{
    app.Destroy();
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
	mainInit();
	while (mainLoop());
	mainFinish();
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(" [%d]: %s\n", line, cond);
#ifdef __APPLE__
    //kill(getpid(), SIGSTOP);
#endif
    while (1);
}
