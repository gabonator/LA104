#include <library.h>
#include "shapes.h"
#include "shapes2.h"
#include "Menu.h"
#include "stm32pwm.h"

using namespace BIOS;

class CMenuMain : public CTopMenu
{
public:
	virtual TItem GetItem(int i)
	{
		switch (i)
		{
			case 0: return TItem{"GPIO Control", TItem::Static};
			case 1: return TItem{"Digital", TItem::Default};
			case 2: return TItem{"PWM", TItem::Default};
			default: return TItem{nullptr, TItem::None};
		}
	}
};

class CGpio
{
public:
	enum EDirection {Disabled, Input, Output};
	using EPin = BIOS::GPIO::EPin;
	
	void SetDirection(EPin pin, EDirection dir)
	{
		switch (dir)
		{
			case Disabled:
			case Input:
				GPIO::PinMode(pin, GPIO::EMode::Input);
				break;
			case Output:
				GPIO::PinMode(pin, GPIO::EMode::Output);
				break;
		}
	}

	bool Read(EPin pin)
	{
#ifdef __APPLE__
		return random() & 1;
#endif
		return GPIO::DigitalRead(pin);
	}

	void Write(EPin pin, bool level)
	{
		GPIO::DigitalWrite(pin, level);
	}

#ifdef __APPLE__
	int mPrescaler{72};
#endif

	void SetPwmPrescaler(int prescaler)
	{
#ifdef __APPLE__
		mPrescaler = prescaler;
#else
		TIM2->PSC = prescaler;
		TIM4->PSC = prescaler;
#endif
	}
	
	int GetPwmFrequency()
	{
		uint32_t SystemCoreClock = 72 * 1000000;
		
#ifdef __APPLE__
		return SystemCoreClock / (GPIO::AnalogRange) / (mPrescaler + 1);
#else
		return SystemCoreClock / (TIM2->ARR) / (TIM2->PSC + 1);
#endif
	}

} mGpio;

constexpr uint16_t gBackground(RGB565(404040));
constexpr uint16_t gControl(RGB565(909090));

class CWindow : public CWnd
{
public:
	void Window(const CRect& rcRect, uint16_t clr)
	{
		CRect rcClient = rcRect;
		BIOS::LCD::RoundRect(rcClient, RGB565(000000));
		rcClient.Deflate(2, 2, 2, 2);
		BIOS::LCD::RoundRect(rcClient, clr);
		rcClient.Deflate(2, 14, 2, 2);
		BIOS::LCD::RoundRect(rcClient, gBackground);
	}

	virtual void OnPaint() override
	{
		Window(m_rcClient, RGB565(d0d0d0));
		BIOS::LCD::Print((m_rcClient.right + m_rcClient.left - (int)strlen(m_pszId)*8)/2, m_rcClient.top + 2,
						 RGB565(000000), RGBTRANS, m_pszId);
	}
};

#include "digital.h"
#include "pwm.h"

class CApplication : public CWnd
{
	CMenuMain mMenu;
	CDigital mDigital;
	CPwm mPwm;
	
public:
	void Create()
	{
		CWnd::Create("Application", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
		mMenu.Create("MainMenu", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, 14), this);
		
		constexpr int padding = 20;
		mDigital.Create("Digital IO control", CWnd::WsHidden, CRect(padding, 14+padding, BIOS::LCD::Width-padding, BIOS::LCD::Height-padding), this);
		mPwm.Create("Pulse width modulation", CWnd::WsHidden, CRect(padding, 14+padding, BIOS::LCD::Width-padding, BIOS::LCD::Height-padding), this);

		mDigital.ShowWindow( true );
		mDigital.SetFocus();

		//mPwm.ShowWindow(true);
		//mPwm.SetFocus();
	}
	
	void Destroy()
	{
        GPIO::PinMode(GPIO::EPin::P1, GPIO::EMode::Pwm);
        GPIO::PinMode(GPIO::EPin::P1, GPIO::EMode::Input);
	}
	
	virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override
	{
		if (code == ToWord('M', 'S'))
		{
			if (pSender == &mMenu && data == 1 && !mDigital.IsVisible())
			{
				mPwm.ShowWindow(false);
				mDigital.ShowWindow(true);
				Invalidate();
			}
			if (pSender == &mMenu && data == 2 && !mPwm.IsVisible())
			{
				mDigital.ShowWindow(false);
				mPwm.ShowWindow(true);
				Invalidate();
			}
		}
	}
private:
	virtual void OnPaint() override
	{
		GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
	}

};

CApplication app;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
	app.Create();
	
	app.WindowMessage( CWnd::WmPaint );
	
	BIOS::KEY::EKey key;
	while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
	{
		if (key != BIOS::KEY::None)
			app.WindowMessage(CWnd::WmKey, key);
		app.WindowMessage(CWnd::WmTick);
	}
	
	app.Destroy();
	return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(") [%d]: %s\n", line, cond);
#ifdef __APPLE__
    //kill(getpid(), SIGSTOP);
#endif
    while (1);
}

