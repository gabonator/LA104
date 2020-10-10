class CDigital : public CWindow
{
	int mCol{0};
	int mRow{4};
	struct TRow
	{
		CGpio::EPin pin;
		const char* id;
		bool enabled;
		enum {OnlyInput, Input, Output} dir;
		bool logic;
		bool prev;

		CGpio::EDirection GetDirection()
		{
			if (!enabled)
				return CGpio::EDirection::Disabled;
			else
				return dir == TRow::Output ? CGpio::EDirection::Output : CGpio::EDirection::Input;
		}
	};
	
	TRow mConfig[8];

public:
	CDigital()
	{
		CGpio::EPin pins[] = {CGpio::EPin::CH1, CGpio::EPin::CH2, CGpio::EPin::CH3, CGpio::EPin::CH4,
			CGpio::EPin::P1, CGpio::EPin::P2, CGpio::EPin::P3, CGpio::EPin::P4};
		const char* channels[] = {"CH1:", "CH2:", "CH3:", "CH4:", " P1:", " P2:", " P3:", " P4:"};
		
		for (int i=0; i<COUNT(mConfig); i++)
		{
			TRow& row = mConfig[i];
			row.pin = pins[i];
			row.id = channels[i];
			row.enabled = i >= 4;
			if (i<4)
				row.dir = TRow::OnlyInput;
			else
				row.dir = (i < 8) ? TRow::Input : TRow::Output;
			row.logic = 0;
			row.prev = 0;
		}
	}
	
	void DrawToggler(int x, int y, const char* str0, const char* str1, int selected, int focus)
	{
		const char* shLeft = selected == 1 ? CShapes_toggle_left : CShapes_toggle_fullleft;
		const char* shLeftBar = selected == 1 ? CShapes_toggle_bar : CShapes_toggle_full;
		const char* shRight = selected == 1 ? CShapes_toggle_fullright : CShapes_toggle_right;
		const char* shRightBar = selected == 1 ? CShapes_toggle_full : CShapes_toggle_bar;
		const char* shPipe = selected == 1 ? CShapes_toggle_pipe1 : CShapes_toggle_pipe2;

		x += LCD::Draw(x, y, gControl, gBackground, shLeft);
		int _x = x;
		for (int i=0; i<strlen(str0); i++)
			x += LCD::Draw(x, y, gControl, gBackground, shLeftBar);
		
		if (focus == 0)
			LCD::Print(_x, y, selected == 0 ? RGB565(000000) : gControl, RGB565(ffffff), str0);
		else
			LCD::Print(_x, y, selected == 0 ? RGB565(ffffff) : gControl, RGBTRANS, str0);

		x += LCD::Draw(x, y, gControl, gBackground, shPipe);
		_x = x;
		for (int i=0; i<strlen(str1); i++)
			x += LCD::Draw(x, y, gControl, gBackground, shRightBar);
		
		if (focus == 1)
			LCD::Print(_x, y, selected == 1 ? RGB565(000000) : gControl, RGB565(ffffff), str1);
		else
			LCD::Print(_x, y, selected == 1 ? RGB565(ffffff) : gControl, RGBTRANS, str1);
		x += LCD::Draw(x, y, gControl, gBackground, shRight);
	}
	
	void DrawSingleToggler(int x, int y, const char* str0)
	{
		const char* shLeft = CShapes_toggle_left;
		const char* shLeftBar = CShapes_toggle_bar;
		const char* shRight = CShapes_toggle_right;
		
		x += LCD::Draw(x, y, gControl, gBackground, shLeft);
		int _x = x;
		for (int i=0; i<strlen(str0); i++)
			x += LCD::Draw(x, y, gControl, gBackground, shLeftBar);
		LCD::Print(_x, y, gControl, RGBTRANS, str0);
		x += LCD::Draw(x, y, gControl, gBackground, shRight);
	}

	void DrawLine(int _x, int _y, const TRow& row, bool selected)
	{
		if (selected && (mCol == 0 || !row.enabled || row.dir == TRow::OnlyInput))
			LCD::Print(_x, _y, RGB565(000000), RGB565(FFFFFF), row.id);
		else
			LCD::Print(_x, _y, row.enabled ? RGB565(d0d0d0) : RGB565(808080), gBackground, row.id);
		
		if (!row.enabled)
		{
			LCD::Bar(_x+40, _y, _x+260, _y+14, gBackground);
			return;
		}

		if (row.dir == TRow::OnlyInput)
			DrawSingleToggler(_x+40, _y, "Input");
		else
			DrawToggler(_x+40, _y, "In", "Out", row.dir == TRow::Input ? 0 : 1, selected ? mCol - 1 : -1);
		
		//LCD::Rectangle(CRect(_x+180, _y+1, _x+180+80, _y+14-1), gControl);
		
		_x += 128;
		if (row.dir != TRow::Output)
		{
			_x += LCD::Print(_x, _y, RGB565(ffffff), gBackground, row.logic ? " 1 " : " 0 ");
		} else
		{
			if (selected && mCol == 3)
			{
				_x += LCD::Draw(_x, _y, RGB565(ffffff), gBackground, CShapes_toggle_fullleft);
				_x += LCD::Printf(_x, _y, RGB565(000000), RGB565(ffffff), row.logic ? "1" : "0");
				_x += LCD::Draw(_x, _y, RGB565(ffffff), gBackground, CShapes_toggle_fullright);
			}
			else
			{
				_x += LCD::Draw(_x, _y, gControl, gBackground, CShapes_toggle_left);
				LCD::Draw(_x, _y, gControl, gBackground, CShapes_toggle_bar);
				_x += LCD::Printf(_x, _y, RGB565(ffffff), RGBTRANS, row.logic ? "1" : "0");
				_x += LCD::Draw(_x, _y, gControl, gBackground, CShapes_toggle_right);
			}
		}
	}
	
	void DrawLine(int i)
	{
		int _x = m_rcClient.left + 8;
		int _y = m_rcClient.top + 4 + 16 + i*18+18;
		
		DrawLine(_x, _y, mConfig[i], HasFocus() && i==mRow);
	}
	
	void DrawLevel(int i)
	{
		int _x = m_rcClient.left + 8;
		int _y = m_rcClient.top + 4 + 16 + i*18+18;

		TRow& row = mConfig[i];
		
		_x += 128;
		_x += LCD::Print(_x, _y, RGB565(ffffff), gBackground, row.logic ? " 1 " : " 0 ");
	}
	
	void ScrollLevel(int i)
	{
		int _x = m_rcClient.left + 8;
		int _y = m_rcClient.top + 4 + 16 + i*18+18;
		CRect rcLevel(_x+180+1, _y+1+1, _x+180+80-1, _y+14-1-1);
		
		TRow& row = mConfig[i];

		constexpr uint16_t clr0 = RGB565(404040);
		constexpr uint16_t clr1 = RGB565(ffffff);
		constexpr uint16_t clr2 = RGB565(606060);

		static const uint16_t bufferOff[10] = {clr1, clr0, clr0, clr0, clr0, clr0, clr0, clr0, clr0, clr0};
		static const uint16_t bufferOn[10] =  {clr2, clr2, clr2, clr2, clr2, clr2, clr2, clr2, clr2, clr1};
		static const uint16_t bufferSw[10] =  {clr1, clr1, clr1, clr1, clr1, clr1, clr1, clr1, clr1, clr1};

		LCD::BufferBegin(CRect(rcLevel.right-1, rcLevel.top, rcLevel.right, rcLevel.bottom));

		if (row.prev != row.logic)
			LCD::BufferWrite((uint16_t*)bufferSw, 10);
		else if (row.logic)
			LCD::BufferWrite((uint16_t*)bufferOn, 10);
		else
			LCD::BufferWrite((uint16_t*)bufferOff, 10);
		LCD::BufferEnd();

		row.prev = row.logic;

		uint16_t buffer[80];
		for (int y=rcLevel.top; y<rcLevel.bottom; y++)
		{
			LCD::BufferBegin(CRect(rcLevel.left+1, y, rcLevel.right, y+1));
			LCD::BufferRead(buffer, rcLevel.Width()-1);
			LCD::BufferEnd();
			LCD::BufferBegin(CRect(rcLevel.left, y, rcLevel.right-1, y+1));
			LCD::BufferWrite(buffer, rcLevel.Width()-1);
			LCD::BufferEnd();
		}

	}
	
	virtual void OnPaint() override
	{
		CWindow::OnPaint();

		int _x = m_rcClient.left + 8;
		int _y = m_rcClient.top + 4 + 16;
		LCD::Bar(_x, _y+12, m_rcClient.right-8, _y+12+1, RGB565(505050));
		LCD::Print(_x, _y, RGB565(808080), RGBTRANS, "Pin  Direction  Logic level");
		
		for (int i=0; i<COUNT(mConfig); i++)
			DrawLine(i);
	}
	
	virtual void OnKey(int nKey) override
	{
		if (nKey == BIOS::KEY::Left && mCol > 0)
		{
			if (mConfig[mRow].dir == TRow::OnlyInput)
				return;
			mCol--;
			DrawLine(mRow);
		}
		if (nKey == BIOS::KEY::Right && mCol < 3)
		{
			if (mConfig[mRow].dir == TRow::OnlyInput)
				return;
			if (mConfig[mRow].dir == TRow::Input && mCol == 2)
				return;
			
			mCol++;
			DrawLine(mRow);
		}
		if (nKey == BIOS::KEY::Up && mRow > 0)
		{
			mRow--;

			if (mConfig[mRow].dir == TRow::Input && mCol == 3)
				mCol = 2;

			DrawLine(mRow+1);
			DrawLine(mRow);
			return;
		}
		if (nKey == BIOS::KEY::Down && mRow < COUNT(mConfig)-1)
		{
			mRow++;

			if (mConfig[mRow].dir == TRow::Input && mCol == 3)
				mCol = 2;

			DrawLine(mRow-1);
			DrawLine(mRow);
			return;
		}
		if (nKey == BIOS::KEY::Enter)
		{
			TRow& row = mConfig[mRow];
			if (!row.enabled || row.dir == TRow::OnlyInput)
			{
				row.enabled = !row.enabled;
				mGpio.SetDirection(row.pin, row.GetDirection());
			}
			else
			{
				if (mCol == 0)
				{
					row.enabled = !row.enabled;
					mGpio.SetDirection(row.pin, row.GetDirection());
					if (row.dir == TRow::Output)
						mGpio.Write(row.pin, row.logic);
				}
				if (mCol == 1)
				{
					row.dir = TRow::Input;
					mGpio.SetDirection(row.pin, row.GetDirection());
				}
				if (mCol == 2)
				{
					row.dir = TRow::Output;
					mGpio.SetDirection(row.pin, row.GetDirection());
					mGpio.Write(row.pin, row.logic);
				}
				if (mCol == 3)
				{
					row.logic = !row.logic;
					mGpio.Write(row.pin, row.logic);
				}
			}
			DrawLine(mRow);
		}
		CWnd::OnKey(nKey);
	}

	virtual void OnTimer() override
	{
		for (int i=0; i<COUNT(mConfig); i++)
		{
			TRow& row = mConfig[i];
			if (row.enabled)
			{
				if (row.dir != TRow::Output)
				{
					row.logic = mGpio.Read(row.pin);
					DrawLevel(i);
				}
				ScrollLevel(i);
			}
		}
	}
	
	virtual void WindowMessage(int nMsg, int nParam) override
	{
		if (nMsg == CWnd::WmWillShow)
		{
			SetTimer(50);
			for (int i=0; i<COUNT(mConfig); i++)
			{
				TRow& row = mConfig[i];
				mGpio.SetDirection(row.pin, row.GetDirection());
				if (row.dir == TRow::Output)
					mGpio.Write(row.pin, row.logic);
			}
		}
		
		if (nMsg == CWnd::WmWillHide)
		{
			for (int i=0; i<COUNT(mConfig); i++)
			{
				TRow& row = mConfig[i];
				mGpio.SetDirection(row.pin, CGpio::Disabled);
			}
			KillTimer();
		}
		
		CWnd::WindowMessage(nMsg, nParam);
	}

};
