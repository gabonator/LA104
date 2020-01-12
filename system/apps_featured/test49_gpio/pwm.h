#include <math.h>

class CPwm : public CWindow
{
	int mCol{0};
	int mRow{0};
	int mPrescaler{72};
	bool mExponential{true};
	bool mAnimate{false};
	
	struct TRow
	{
		CGpio::EPin pin;
		const char* id;
		bool enabled;
		int level;
		
		bool animate;
		
		int GetPercent() const
		{
			return level*100/GPIO::AnalogRange;
		}
		int GetPosition(int w) const
		{
			return level*w/GPIO::AnalogRange;
		}
	};
	
	TRow mConfig[4];
public:
	CPwm()
	{
		CGpio::EPin pins[] = {CGpio::EPin::P1, CGpio::EPin::P2, CGpio::EPin::P3, CGpio::EPin::P4};
		const char* channels[] = {" P1:", " P2:", " P3:", " P4:"};
		
		for (int i=0; i<COUNT(mConfig); i++)
		{
			TRow& row = mConfig[i];
			row.pin = pins[i];
			row.id = channels[i];
			row.enabled = true;
			row.level = 0;
			row.animate = false;
		}
	}
	
	void DrawSlider(const CRect& rc, int pos)
	{
		LCD::Bar(rc, gBackground);
		
		int mid = (rc.top + rc.bottom)/2;
		CRect rcBar(rc.left+2, mid-2, rc.right-2, mid+2);
		LCD::Rectangle(rcBar, gControl);
		
		int x = rc.left + pos;
		if (x < rc.left+2)
			x = rc.left+2;
		if (x >= rc.right-2)
			x = rc.right-2;
		
		CRect rcTick(x-2, rc.top, x+2, rc.bottom);
		LCD::Bar(rcTick, RGB565(d0d0d0));
	}

	void DrawLine(int _x, int _y, const TRow& row, bool selected)
	{
		if (selected && (mCol == 0 || !row.enabled))
			_x += LCD::Print(_x, _y, RGB565(000000), RGB565(FFFFFF), row.id);
		else
			_x += LCD::Print(_x, _y, row.enabled ? RGB565(d0d0d0) : RGB565(808080), gBackground, row.id);
		
		if (!row.enabled)
		{
			LCD::Bar(_x+8, _y, _x+220, _y+14, gBackground);
			return;
		}

		_x += 8;
		_x += LCD::Printf(_x, _y, gControl, gBackground, "%3d%%", row.GetPercent());
		
		CRect rcSlider(_x+16, _y+2, _x+16+100, _y+12);
		DrawSlider(rcSlider, row.GetPosition(rcSlider.Width()));
	}
	
	void DrawLine(int i)
	{
		int _x = m_rcClient.left + 8;
		int _y = m_rcClient.top + 4 + 16 + i*18+18;
		bool focus = HasFocus() && i==mRow;
		
		if (i < COUNT(mConfig))
			DrawLine(_x, _y, mConfig[i], focus);
		else
		{
			int clrF = focus ? RGB565(000000) : RGB565(d0d0d0);
			int clrB = focus ? RGB565(ffffff) : gBackground;
			
			if (i==4)
			{
				_x += LCD::Print(_x, _y, clrF, clrB, " Frequency:");
				_x += LCD::Printf(_x, _y, gControl, gBackground, " %dHz  ", (int)mGpio.GetPwmFrequency());
			}
			if (i==5)
			{
				_x += LCD::Print(_x, _y, clrF, clrB, " Animate:");
				_x += LCD::Print(_x, _y, gControl, gBackground, mAnimate ? " ON " : " off ");
			}
			if (i==6)
			{
				_x += LCD::Print(_x, _y, clrF, clrB, " Curve:");
				_x += LCD::Print(_x, _y, gControl, gBackground, mExponential ? " exponential " : " linear     ");
			}
		}
	}

	virtual void OnPaint() override
	{
		CWindow::OnPaint();

		int _x = m_rcClient.left + 8;
		int _y = m_rcClient.top + 4 + 16;
		LCD::Bar(_x, _y+12, m_rcClient.right-8, _y+12+1, RGB565(505050));
		LCD::Print(_x, _y, RGB565(808080), RGBTRANS, " Pin  PWM   Level");
		
		for (int i=0; i<COUNT(mConfig)+3; i++)
			DrawLine(i);
	}
	
	int PrescalerStep()
	{
		int step = mPrescaler / 10;

		if (mGpio.GetPwmFrequency() < 100)
			step /= 2;
		
		if (step <= 0)
			return 1;
		
		return step;
	}

	int AnalogStep()
        {
          static long lastChange1 = 0;
          static long lastChange2 = 0;
          long now = BIOS::SYS::GetTick();
          if (lastChange1 == 0)
          {
            lastChange1 = now;
            lastChange2 = now;
            return GPIO::AnalogRange/20;
          }

          int duration = now-lastChange2;
          lastChange2 = lastChange1;
          lastChange1 = now;

          if (duration < 50)
            return GPIO::AnalogRange/20;  // 50
          if (duration < 100)
            return GPIO::AnalogRange/50;  // 20
          if (duration < 300)
            return GPIO::AnalogRange/100; // 10
          return 1;                       // 1
        }

	void Update(int n)
	{
		if (!mConfig[n].enabled)
			return;
		
		int level = mConfig[n].level;
		if (mExponential)
		{
			static int expCurve[65];
			static bool init{false};
			if (!init)
			{
				_ASSERT(GPIO::AnalogRange == 1024);
				
				for (int i=0; i<=64; i++)
					expCurve[i] = (int)(pow(i/64.0f, 2.5f)*1024);

				init = true;
			}
			level = expCurve[level/16];
		}
		GPIO::AnalogWrite(mConfig[n].pin, level);
	}
	
	virtual void OnKey(int nKey) override
	{
		if (nKey == BIOS::KEY::Up && mRow > 0)
		{
			mRow--;
			
			DrawLine(mRow+1);
			DrawLine(mRow);
			return;
		}
		if (nKey == BIOS::KEY::Down && mRow < COUNT(mConfig)-1+3)
		{
			mRow++;
			
			DrawLine(mRow-1);
			DrawLine(mRow);
			return;
		}
		if (nKey == BIOS::KEY::Left)
		{
			if (mRow < COUNT(mConfig))
			{
				TRow& row = mConfig[mRow];
				if (row.enabled && row.level > 0)
				{
					row.animate = false;
					row.level -= AnalogStep();
					if (row.level < 0)
						row.level = 0;
					
					Update(mRow);
					DrawLine(mRow);
				}
			}
			if (mRow == COUNT(mConfig))
			{
				mPrescaler += PrescalerStep();
				if (mPrescaler > 0xffff)
					mPrescaler = 0xffff;
				mGpio.SetPwmPrescaler(mPrescaler);
				DrawLine(mRow);
			}
		}
		if (nKey == BIOS::KEY::Right)
		{
			if (mRow < COUNT(mConfig))
			{
				TRow& row = mConfig[mRow];
				
				if (row.enabled && row.level < GPIO::AnalogRange)
				{
					row.animate = false;
					row.level += AnalogStep();
					if (row.level > GPIO::AnalogRange)
						row.level = GPIO::AnalogRange;
					
					Update(mRow);
					DrawLine(mRow);
				}
			}
			if (mRow == COUNT(mConfig) && mPrescaler > 2)
			{
				mPrescaler -= PrescalerStep();
				mGpio.SetPwmPrescaler(mPrescaler);
				DrawLine(mRow);
			}
		}
		if (nKey == BIOS::KEY::Enter)
		{
			if (mRow < COUNT(mConfig))
			{
				TRow& row = mConfig[mRow];
				row.enabled = !row.enabled;
				DrawLine(mRow);
			}
			if (mRow == COUNT(mConfig))
			{
				mPrescaler = 72;
				mGpio.SetPwmPrescaler(mPrescaler);
				DrawLine(mRow);
				return;
			}
			if (mRow == COUNT(mConfig)+1)
			{
				mAnimate = !mAnimate;
				if (mAnimate)
				{
					mConfig[0].animate = true;
					mConfig[1].animate = true;
					mConfig[2].animate = true;
					mConfig[3].animate = true;
				}
				DrawLine(mRow);
				return;
			}
			if (mRow == COUNT(mConfig)+2)
			{
				mExponential = !mExponential;
				DrawLine(mRow);
				Update(0);
				Update(1);
				Update(2);
				Update(3);
				return;
			}
		}

		CWnd::OnKey(nKey);
	}
	
	virtual void OnTimer() override
	{
		if (HasFocus() && mAnimate)
		{
			int tick = SYS::GetTick();
			
			if (mConfig[0].animate)
			{
				mConfig[0].level = (int)(sin(tick*0.0001f*5)*511+512);
				Update(0);
				DrawLine(0);
			}
			if (mConfig[1].animate)
			{
				mConfig[1].level = (int)(sin(tick*0.00013f*5)*511+512);
				Update(1);
				DrawLine(1);
			}
			if (mConfig[2].animate)
			{
				mConfig[2].level = (int)(sin(tick*0.00017f*5)*511+512);
				Update(2);
				DrawLine(2);
			}
			if (mConfig[3].animate)
			{
				mConfig[3].level = (int)(sin(tick*0.00019f*5)*511+512);
				Update(3);
				DrawLine(3);
			}
		}
	}
	
	virtual void WindowMessage(int nMsg, int nParam) override
	{
		if (nMsg == CWnd::WmWillShow)
		{
			SetTimer(50);
			GPIO::PinMode(GPIO::EPin::P1, GPIO::EMode::Pwm);
			GPIO::PinMode(GPIO::EPin::P2, GPIO::EMode::Pwm);
			GPIO::PinMode(GPIO::EPin::P3, GPIO::EMode::Pwm);
			GPIO::PinMode(GPIO::EPin::P4, GPIO::EMode::Pwm);
			Update(0);
			Update(1);
			Update(2);
			Update(3);
		}
		
		if (nMsg == CWnd::WmWillHide)
		{
			GPIO::PinMode(GPIO::EPin::P1, GPIO::EMode::Input);
			GPIO::PinMode(GPIO::EPin::P2, GPIO::EMode::Input);
			GPIO::PinMode(GPIO::EPin::P3, GPIO::EMode::Input);
			GPIO::PinMode(GPIO::EPin::P4, GPIO::EMode::Input);
			KillTimer();
		}
		
		CWnd::WindowMessage(nMsg, nParam);
	}

};
