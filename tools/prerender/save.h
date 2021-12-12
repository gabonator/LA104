bool SaveScreenshot16(char* strName)
{
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
			uint16_t wPixel = BIOS::LCD::GetPixel( x, y );

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
				uint16_t wPixel = BIOS::LCD::GetPixel( x, y );
				BIOS::LCD::PutPixel( x, y, wPixel^0x18e3);
				// ..... ...... .....
				// ...11 ...111 ...11
			}
		}

	return true;
}
