#include <stdint.h>
#include <library.h>
#include "SPI.h"
#include "../../os_host/source/framework/Console.h"
using namespace BIOS;
#ifdef EMULATED
#include <math.h>
#define abs fabs
#endif

#include "WS2812B.h"
#include "colorspace.h"


template <typename T> T minmax(T min, T val, T max)
{
	if (val < min)
		return min;
	if (val > max)
		return max;
	return val;
}

template <typename T, typename T2> T enumclamp(T2 val, T max)
{
	if (val < 0)
		return (T)0;
	if (val > max-1)
		return (T)(max-1);
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
	enum EDriver { WS2812, WS2812B } mDriver {WS2812};
	int mLedCount{24};
	enum EPattern { UserColor, UserGradient, Black, Gray25, Gray50, Gray75, White, Red, Green, Blue, Gradient, Ring, Colors, Total} mPattern {Gradient};
	int mBrightness{64};
	int mSpeed{256};
	
	const char* PatternName()
	{
		const char* modes[] = {"User color", "User gradient", "Black", "25% Gray", "50% Gray", "75% Gray", "White", "Red", "Green", "Blue", "Gradient", "Ring", "Colors"};
		return modes[mPattern];
	}
	
	const char* PlayState()
	{
		return "Playing";
	}
};

class CPatternGenerator
{
public:
	inline float xabs(float x)
	{
		if (x>=0)
			return x;
		return -x;
	}
	
	float xsin(float x)
	{
		constexpr float tp = 1.f/(2.f*3.14159265f);
		x *= tp;
		x -= .25f + (int)(x + .25f);
		x *= 16.f * (xabs(x) - .5f);
		return x;
	}
	
	uint32_t GetColorGradient(int i, int time)
	{
		float a = time * 0.001f;
		constexpr float q = 0.4f/3.0f;
		
		int r, g, b;
		r = 128 + xsin(a+i*q)*128;
		g = 128 + xsin(a*0.37f+i*q)*128;
		b = 128 + xsin(a*0.11f+i*q)*128;
		 
		r = max(0, min(r, 255));
		g = max(0, min(g, 255));
		b = max(0, min(b, 255));

		return RGB32(r, g, b);
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
		 
		r = max(0, min(r, 255));
		g = max(0, min(g, 255));
		b = max(0, min(b, 255));

		return RGB32(r, g, b);
	}
	
	uint32_t GetColorColors(int i, int time)
	{
		const static uint32_t colors[] = {0xff0000, 0xffff00, 0x00ff00, 0x00ffff, 0x0000ff, 0xff00ff};
 		int index = time/1000;
		int part = time%1000;
		int N = 24;

		if (i > N*part/1000)
		  return colors[index % COUNT(colors)];
		else
		  return colors[(index+1) % COUNT(colors)];
	}
	
	uint32_t GetColor(int i, int time)
	{
		switch (CSettings::Settings().mPattern)
		{
			case CSettings::UserColor: _ASSERT(0);
			case CSettings::UserGradient: _ASSERT(0);
			case CSettings::Black: return RGB32(0, 0, 0);
			case CSettings::Gray25: return RGB32(64, 64, 64);
			case CSettings::Gray50: return RGB32(128, 128, 128);
			case CSettings::Gray75: return RGB32(192, 192, 192);
			case CSettings::White: return RGB32(255, 255, 255);
			case CSettings::Red: return RGB32(255, 0, 0);
			case CSettings::Green: return RGB32(0, 255, 0);
			case CSettings::Blue: return RGB32(0, 0, 255);
			case CSettings::Gradient: return GetColorGradient(i, time);
			case CSettings::Ring: return GetColorRing(i, time);
			case CSettings::Colors: return GetColorColors(i, time);
			default:
				_ASSERT(0);
		}
		return 0;
	}
};

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
	uint32_t mColors[WS2812B_MAXCOUNT];
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
		if (CSettings::Settings().mPattern == CSettings::UserColor || CSettings::Settings().mPattern == CSettings::UserGradient)
			return;
		
		long lCurrentTick = BIOS::SYS::GetTick();
		if (lLastTick == 0)
			lLastTick = lCurrentTick;
		
		if (CSettings::Settings().mSpeed == 256)
			lTime += lCurrentTick - lLastTick;
		else
			lTime += (lCurrentTick - lLastTick) * CSettings::Settings().mSpeed >> 8;
		
		lLastTick = lCurrentTick;

		constexpr int count = CSettings::Settings().mLedCount;
		for (int i=0; i<count; i++)
			mColors[i] = mGenerator.GetColor(i, lTime);
	}
	
	int Get(int i)
	{
		return mColors[i];
	}
	
	void Set(int i, int color)
	{
		if (CSettings::Settings().mPattern == CSettings::UserColor)
		{
			for (int i=0; i<CSettings::Settings().mLedCount; i++)
				mColors[i] = color;
			return;
		}
		if (CSettings::Settings().mPattern == CSettings::UserGradient)
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
	constexpr int Height = 56;
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
		_x += BIOS::LCD::Printf(_x, _y, FOCUS(1) ? "<%s>" : "%s", "WS2812");

		_x = m_rcClient.left+8, _y += 14;
		_x += BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "Pattern: ");
		_x += BIOS::LCD::Printf(_x, _y, FOCUS(2) ? "<%s>" : "%s", CSettings::Settings().PatternName());

		_x = m_rcClient.left+8, _y += 14;
		_x += BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "Brightness: ");
		_x += BIOS::LCD::Printf(_x, _y, FOCUS(3) ? "<%d>%%" : "%d%%", CSettings::Settings().mBrightness*100/256);
		_x = BIOS::LCD::Width/2+20;
		_x += BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "Speed: ");
		_x += BIOS::LCD::Printf(_x, _y, FOCUS(4) ? "<%d>%%" : "%d%%", CSettings::Settings().mSpeed*100/256);

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
					CSettings::Settings().mLedCount = minmax(1, CSettings::Settings().mLedCount + dir, 25);
					GetParent()->Invalidate();
					break;
				case 2:
					CSettings::Settings().mPattern = enumclamp(CSettings::Settings().mPattern + dir, CSettings::EPattern::Total);
					Invalidate();
					break;
				case 3:
					CSettings::Settings().mBrightness = minmax(0, CSettings::Settings().mBrightness + dir*8, 256);
					Invalidate();
					break;
				case 4:
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
			if (mFocus < 4)
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
	constexpr int Height = 14;
	int mSelected = 0;
	CPattern mPattern;
	WS2812B mLeds;
	long lTime = 0;
	//uint32_t mColors[WS2812B_MAXCOUNT];

	CPreview() : mLeds(WS2812B_MAXCOUNT)
	{
		//lBaseTime = BIOS::SYS::GetTick();
		mLeds.begin();
		//mLeds.setBrightness(CSettings::Settings().mBrightness);
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
		constexpr int count = CSettings::Settings().mLedCount;
		int brightness = CSettings::Settings().mBrightness;
		
		for (int i=0; i<count; i++)
		{
			uint32_t color32 = mPattern[i];
			if (brightness == 256)
				mLeds.setPixelColor(i, color32);
			else
			{
				mLeds.setPixelColor(i, mLeds.Color(
					GetColorR(color32)*brightness>>8,
					GetColorG(color32)*brightness>>8,
					GetColorB(color32)*brightness>>8));
			}
			DrawColor(i, color32);
		}
		mLeds.show();
	}
	
	void SetColor(uint32_t c)
	{
		mPattern[mSelected] = c;
	}
	
	CRect GetRect(int i)
	{
		constexpr int spacing = 4;
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
	uint32_t mLastColor{0x10000000};
	int mLastFocus{-1};
	
public:
	constexpr int Height = 64;
	
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

		Colorspace::RgbColor rgb{(uint8_t)GetColorR(color), (uint8_t)GetColorG(color), (uint8_t)GetColorB(color)};

		FocusableInt(0, m_rcClient.left+8, m_rcClient.top+4+14*0, "R:", rgb.r);
		FocusableInt(1, m_rcClient.left+8, m_rcClient.top+4+14*1, "G:", rgb.g);
		FocusableInt(2, m_rcClient.left+8, m_rcClient.top+4+14*2, "B:", rgb.b);
		FocusableInt(-1, m_rcClient.left+8, m_rcClient.top+4+14*3, "W:", 0);

		Colorspace::HsvColor hsv = Colorspace::RgbToHsv(rgb);

		CRect rcClear2(m_rcClient.left+8+100, m_rcClient.top+4, m_rcClient.left+8+100+80, m_rcClient.top+4+58);
		GUI::Background(rcClear2, RGB565(101010), RGB565(101010));

		FocusableInt(3, m_rcClient.left+8+100, m_rcClient.top+4+14*0, "H:", hsv.h);
		FocusableInt(4, m_rcClient.left+8+100, m_rcClient.top+4+14*1, "S:", hsv.s);
		FocusableInt(5, m_rcClient.left+8+100, m_rcClient.top+4+14*2, "L:", hsv.v);
		BIOS::LCD::Printf(m_rcClient.left+8+100, m_rcClient.top+4+14*3, RGB565(b0b0b0), RGBTRANS, "#%06x", color);

		BIOS::LCD::Bar(CRect(m_rcClient.right-50, m_rcClient.top+4, m_rcClient.right-4, m_rcClient.bottom-4), RGB32TO565(color));
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
			if (CSettings::Settings().mPattern != CSettings::UserColor &&
				CSettings::Settings().mPattern != CSettings::UserGradient)
			{
				// Start editing
				CSettings::Settings().mPattern = CSettings::UserGradient;
				GetParent()->Invalidate();
				return;
			} else
			{
				diff *= 8;
				int diffR = 0, diffG = 0, diffB = 0;
				int diffH = 0, diffS = 0, diffV = 0;
				switch (mFocus)
				{
					case 0: diffR = diff; break;
					case 1: diffG = diff; break;
					case 2: diffB = diff; break;
					case 3: diffH = diff; break;
					case 4: diffS = diff; break;
					case 5: diffV = diff; break;
				}

				Colorspace::RgbColor rgb{(uint8_t)GetColorR(mLastColor), (uint8_t)GetColorG(mLastColor), (uint8_t)GetColorB(mLastColor)};
				if (diffR != 0 || diffG != 0 || diffB != 0)
				{
					rgb.r = minmax(0, rgb.r+diffR, 255);
					rgb.g = minmax(0, rgb.g+diffG, 255);
					rgb.b = minmax(0, rgb.b+diffB, 255);
				}

				if (diffH != 0 || diffS != 0 || diffV != 0)
				{
					Colorspace::HsvColor hsv = Colorspace::RgbToHsv(rgb);
					hsv.h = hsv.h+diffH;
					hsv.s = minmax(0, hsv.s+diffS, 255);
					hsv.v = minmax(0, hsv.v+diffV, 255);
					rgb = Colorspace::HsvToRgb(hsv);
				}
				uint32_t newColor = RGB32(rgb.r, rgb.g, rgb.b);
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
				SetColor(mLastColor);
				return;
			}
		}
		if (key == BIOS::KEY::Down)
		{
			if (mFocus < 5)
			{
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
		
		constexpr int padding = 26;
		
		mControl.Create(" Control ", CWnd::WsVisible, CRect(padding, 14+padding, BIOS::LCD::Width-padding, 14+padding + mControl.Height), this);
		mPreview.Create(" Preview ", CWnd::WsVisible, CRect(padding, mControl.m_rcClient.bottom + padding, BIOS::LCD::Width-padding, mControl.m_rcClient.bottom + padding + mPreview.Height), this);
		mColor.Create(" Color ", CWnd::WsVisible, CRect(padding, mPreview.m_rcClient.bottom + padding, BIOS::LCD::Width-padding, mPreview.m_rcClient.bottom + padding + mColor.Height), this);

		mColor.SetFocus();
		
		SetTimer(50);
	}
	
	void Destroy()
	{
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
CApplication app;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    app.Create();
	app.WindowMessage( CWnd::WmPaint );
    BIOS::KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
		if (key != BIOS::KEY::None)
			app.WindowMessage(CWnd::WmKey, key);
		app.WindowMessage(CWnd::WmTick);
    }
    
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    CONSOLE::Color(RGB565(ffff00));
    CONSOLE::Print("Assertion failed in ");
    CONSOLE::Print(file);
    CONSOLE::Print(" [%d]: %s\n", line, cond);
#ifdef __APPLE__
    //kill(getpid(), SIGSTOP);
#endif
    while (1);
}

