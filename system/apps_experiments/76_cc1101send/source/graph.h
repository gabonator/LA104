void graphPush(int interval)
{
	static bool clear = true;
	static int x = 0;
	static bool b = 1; // leading zero pulse is removed
	static int row = 0;
	static bool skip = false;

	const int baseY = 16;
	const int height = 12;
	const int k = 10;
	const int rowHeight = 16;
	const int maxRows = 10;

	if (interval == -1)
	{
		clear = true;
		return;
	}

	if (clear)
	{
		for (int i=0; i<=max(1,row); i++)
			BIOS::LCD::Bar(0, baseY+i*rowHeight, BIOS::LCD::Width, baseY + height+i*rowHeight, i==0 ? RGB565(606060) : RGB565(0000b0));

		skip = false;
		clear = false;
		x = 0;
		row = 0;
		b = 1;
	}
	
	if (skip)
		return;

	int nx = x+interval;
	int x1 = x/k;
	int x2 = nx/k+1;
	int curY = baseY+row*rowHeight;

	if (x > 0)
		BIOS::LCD::Bar(x1, curY, x1+2, curY + height, RGB565(ffff00));

	while (x2 > BIOS::LCD::Width)
	{
		if (!b)
			BIOS::LCD::Bar(x1, curY + height-2, BIOS::LCD::Width, curY + height, RGB565(ffff00));
		else
			BIOS::LCD::Bar(x1, curY, BIOS::LCD::Width, curY+ 2, RGB565(ffff00));

		x1 = 0;
		x2 -= BIOS::LCD::Width;
		row++;
		curY += rowHeight;
		nx -= BIOS::LCD::Width*k;
		if (row >= maxRows)
		{
			skip = true;
			return;
		}
		BIOS::LCD::Bar(0, curY, BIOS::LCD::Width, curY + height, RGB565(606060));
	}

	if (!b)
		BIOS::LCD::Bar(x1, curY + height-2, x2, curY + height, RGB565(ffff00));
	else
		BIOS::LCD::Bar(x1, curY, x2, curY + 2, RGB565(ffff00));

	x = nx;
	b = 1-b;
}
