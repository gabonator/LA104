#pragma pack(push)
#pragma pack(2)
	struct BmpHdr 
	{
		ui16 wBfType;
		ui32 dwBfSize;
		ui16 wBfReserved1;
		ui16 wBfReserved2;
		ui32 dwBfOffset;

		ui32 dwBiSize;
		ui32 dwBiWidth;
		ui32 dwBiHeight;
		ui16 wBiPlanes;
		ui16 wBiBitDepth;
		ui32 dwBiCompression;
		ui32 dwBiSizeImage;
		ui32 dwBiXPels;
		ui32 dwBiYPels;

		ui32 dwBiClrUsed;
		ui32 dwBiClrImportant;
	};
#pragma pack(pop)

bool SaveScreenshot16(char* strName)
{
//	char strNameUnique[] = "IMG000  BMP";
	CBufferedWriter writer;
	if (!writer.Open( strName ))
		return false;

	BmpHdr hdr;
	BmpHdr* pHdr = &hdr;
	pHdr->wBfType = 'B' | ('M'<<8);
	pHdr->dwBfSize = sizeof(BmpHdr) + BIOS::LCD::Width * BIOS::LCD::Height * 3;  // no need to align row to multiply of 4
	pHdr->wBfReserved1 = 0;
	pHdr->wBfReserved2 = 0;
	pHdr->dwBfOffset = 0x36;

	pHdr->dwBiSize = 0x28;
	pHdr->dwBiWidth = BIOS::LCD::Width;
	pHdr->dwBiHeight = BIOS::LCD::Height;
	pHdr->wBiPlanes = 1;
	pHdr->wBiBitDepth = 16;
	pHdr->dwBiCompression = 0;
	pHdr->dwBiSizeImage = BIOS::LCD::Width * BIOS::LCD::Height * 3;
	pHdr->dwBiXPels = 0;
	pHdr->dwBiYPels = 0;
	pHdr->dwBiClrUsed = 0;
	pHdr->dwBiClrImportant = 0;

	writer << CStream( &hdr, sizeof(BmpHdr) );

	for ( int y = BIOS::LCD::Height-1; y >= 0; y-- )
		for ( int x = 0; x < BIOS::LCD::Width; x++ )
		{
			ui16 wPixel = BIOS::LCD::GetPixel( x, y );

			if (((x>>2)+(y>>2))&1)
				BIOS::LCD::PutPixel( x, y, wPixel^0x18e3);

			int r = Get565R( wPixel ) >> 3;
			int g = Get565G( wPixel ) >> 3;
			int b = Get565B( wPixel ) >> 3;

			// RGB555
			wPixel = b | (g << 5) | (r << 10);
			writer << wPixel;
		}

	writer.Close();

	for ( int y = BIOS::LCD::Height-1; y >= 0; y-- )
		for ( int x = 0; x < BIOS::LCD::Width; x++ )
		{
			if (((x>>2)+(y>>2))&1)
			{
				ui16 wPixel = BIOS::LCD::GetPixel( x, y );
				BIOS::LCD::PutPixel( x, y, wPixel^0x18e3);
				// ..... ...... .....
				// ...11 ...111 ...11
			}
		}

	return true;
}
