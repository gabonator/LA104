#include "Manager.h"

#include "gui/Gui.h"
#include "framework/Utils.h"
#include "framework/BufferedIo.h"
#include "framework/Serialize.h"
#include "library/ihex.h"
#include "library/elf.h"

#define MaxLines 14

const char CShapes_sel_left[] =
"\x0e"
"              "
"              "
"              "
"              "
"  ..........  "
" ............ "
".............."
"..............";

const char CShapes_sel_right[] =
"\x0e"
".............."
".............."
" ............ "
"  ..........  "
"              "
"              "
"              "
"              ";

char tolower(char c)
{
  if(c >=65 && c<=90)
    return c+32;
  return c;
}

int stricmp(const char* s1, const char* s2) {
  _ASSERT(s1 != NULL);
  _ASSERT(s2 != NULL);

  while (tolower((unsigned char) *s1) == tolower((unsigned char) *s2)) {
    if (*s1 == '\0')
      return 0;
    s1++; s2++;
  }

  return (int) tolower((unsigned char) *s1) -
    (int) tolower((unsigned char) *s2);
}

int nSelected = 0;
int nScroll = 0;

const int commonBufferSize = 1024*2;
uint8_t commonBuffer[commonBufferSize];

void CWndUserManager::InitFileList()
{
	ui8* pCommonBuffer = commonBuffer;
	BIOS::FAT::TFindFile* arrFiles = (BIOS::FAT::TFindFile*)pCommonBuffer;
	int nDataLength = commonBufferSize;
	int nMaxFiles = nDataLength / 2 / sizeof(BIOS::FAT::TFindFile);
	m_arrFiles.Init( arrFiles, nMaxFiles );

	TLoadedModule* arrModules = (TLoadedModule*)(pCommonBuffer+nDataLength/2);
	int nMaxModules = nDataLength / 2 / sizeof(TLoadedModule);
	m_arrLoaded.Init( arrModules, nMaxModules );
}

void CWndUserManager::LoadModuleList()
{
/*
	CBufferedReader2 fMods;
	if ( fMods.Open((char*)"modules.dat") )
	{
		ui32 dwCheck;
		fMods >> dwCheck;
		if ( dwCheck != ToDword('M', 'L', 'S', '1') )
		{
			fMods.Close();
			return;
		}
		int nCount = 0;
		fMods >> nCount;
		if ( nCount < 0 || nCount > m_arrLoaded.GetMaxSize() )
		{
			fMods.Close();
			return;
		}
		for ( int i=0; i<nCount; i++ )
		{
			TLoadedModule mod;
			fMods >> CStream( &mod, sizeof(TLoadedModule) );
			_ASSERT( !(mod.dwBegin & 0x80000000) );
			m_arrLoaded.Add( mod );
		}
		fMods.Close();
	}
*/
}

void CWndUserManager::SaveModuleList()
{
/*
	CBufferedWriter2 fMods;
	if ( fMods.Open((char*)"modules.dat") )
	{
		ui32 dwCheck = ToDword('M', 'L', 'S', '1');
		fMods << dwCheck;
		int nCount = m_arrLoaded.GetSize();
		fMods << nCount;
		for ( int i=0; i<nCount; i++ )
		{
			_ASSERT( !(m_arrLoaded[i].dwBegin & 0x80000000) );
			fMods << CStream( &m_arrLoaded[i], sizeof(TLoadedModule) );
		}
		fMods.Close();
	}
*/
}

bool CWndUserManager::LoadFileList(char* strPath)
{
	if ( BIOS::FAT::OpenDir(strPath) != BIOS::FAT::EOk )
	{
//		BIOS::DBG::Print("error open\n");
		return false;
	}

	m_arrFiles.RemoveAll();

	BIOS::FAT::TFindFile curFile;
	if ( strPath[0] != 0 )
	{
		strcpy( curFile.strName, ".." );
		curFile.nAtrib = BIOS::FAT::EDirectory;
		curFile.nDate = 0;
		curFile.nTime = 0;
		curFile.nFileLength = 0;
		m_arrFiles.Add( curFile );
	}

	while ( BIOS::FAT::FindNext( &curFile ) == BIOS::FAT::EOk )
	{
		if ( curFile.strName[0] == '.' )
			continue;
		m_arrFiles.Add( curFile );
		if ( m_arrFiles.GetSize() >= m_arrFiles.GetMaxSize() )
			break;
	}
	return true;
}

void CWndUserManager::SortFileList()
{
	m_arrFiles.Sort( CompareFile );
}

CWndUserManager::CWndUserManager()
{
	strcpy( m_strCurrentPath, "" );
}

void CWndUserManager::Create(CWnd *pParent, ui16 dwFlags)
{
	CWnd::Create("CWndManager", dwFlags, CRect(0, 16, BIOS::LCD::Width, BIOS::LCD::Height), pParent);
	InitFileList();
	m_dwExecuteAddress = 0;
}

/*static*/ int CWndUserManager::CompareFile( BIOS::FAT::TFindFile& fA, BIOS::FAT::TFindFile& fB )
{
	int nTest = (fA.nAtrib & BIOS::FAT::EDirectory) - (fB.nAtrib & BIOS::FAT::EDirectory);
	if ( nTest != 0 )
		return nTest;
	if ( strcmp(fA.strName, "..") == 0 )
		return 1;
	if ( strcmp(fB.strName, "..") == 0 )
		return -1;
	char* strExtA = strstr( fA.strName, "." );
	char* strExtB = strstr( fB.strName, "." );
	if ( strExtA != NULL && strExtB != NULL )
	{
		nTest = strcmp( strExtA, strExtB );
		if ( nTest != 0 )
			return nTest;
	} else
	if ( strExtA == NULL && strExtB == NULL )
	{
	} else
	if ( strExtA == NULL )
		return 1;
	nTest = strcmp( fA.strName, fB.strName );
	return nTest;
}

void DrawDelimLines(int y, ui16 clr)
{
	CRect rcLine(3, y, 5, y+14);
	rcLine.Offset(13*8+4, 0);
	BIOS::LCD::Bar( rcLine, clr );
	rcLine.Offset(8*8, 0);
	BIOS::LCD::Bar( rcLine, clr );
	rcLine.Offset(10*8, 0);
	BIOS::LCD::Bar( rcLine, clr );
}

void CWndUserManager::OnPaint()
{
	if ( HasFocus() && nSelected == -1 )
		nSelected = 0; // enters the list

	CRect rcTop(0, 0, BIOS::LCD::Width, 16);
	GUI::Background(rcTop, RGB565(101010), RGB565(404040));

        int x = 0, y = 0;
        x += BIOS::LCD::Print(x, 0, RGB565(000000), RGB565(b0b0b0), " File Manager");
        x += BIOS::LCD::Draw( x, 0, RGB565(b0b0b0), RGBTRANS, CShapes_sel_right);

        x += 8;
        x += BIOS::LCD::Draw( x, 0, RGB565(b0b0b0), RGBTRANS, CShapes_sel_left);
        x += BIOS::LCD::Print(x, 0, RGB565(000000), RGB565(b0b0b0), "/");
        x += BIOS::LCD::Print(x, 0, RGB565(000000), RGB565(b0b0b0), m_strCurrentPath);
        x += BIOS::LCD::Draw( x, 0, RGB565(b0b0b0), RGBTRANS, CShapes_sel_right);

//	CRect rcBack( m_rcClient.left, m_rcClient.top, m_rcClient.right-8, m_rcClient.top + 20);
//	Background(rcBack, RGB565(404040), RGB565(101010));
//	Background(rcBack, RGB565(404040), RGB565(404040));
//	BIOS::LCD::Bar( m_rcClient.left, m_rcClient.top, m_rcClient.right-8, m_rcClient.top + 20, RGB565(0000b0) );
	y = 16;

	CRect rcHeading(0, y, BIOS::LCD::Width-8, y+16);
	GUI::Background(rcHeading, RGB565(202020), RGB565(202020));
	BIOS::LCD::Print( 4, y, RGB565(ffff00), RGBTRANS, "     Name       Size     Date    Time");
	DrawDelimLines(y, RGB565(0fffff));
	y += 14;

	if ( nSelected-nScroll >= MaxLines )
		nScroll = nSelected - MaxLines + 1;
	if ( nSelected != -1 && nSelected-nScroll < 0 )
		nScroll = nSelected;

	int i;
	for ( i = 0; i < MaxLines; i++)
	{
		if ( i + nScroll >= m_arrFiles.GetSize() )
			break;
		
		BIOS::FAT::TFindFile& fileInfo = m_arrFiles[i+nScroll];
		DrawLine( fileInfo, y, (nSelected == i+nScroll) );
		y += 14;
	}
	for ( ; i < MaxLines; i++)
	{
//		BIOS::LCD::Bar( 0, y, /*320*/400-8, y+14, RGB565(0000b0) );
          CRect rcBack( 0, y, /*320*/BIOS::LCD::Width-8, y+14);

	  GUI::Background(rcBack, RGB565(101010), RGB565(404040));

		DrawDelimLines(y, RGB565(0fffff));
		y += 14;
	}
	DrawProgress();

        CRect rcBack( 0, y, /*320*/BIOS::LCD::Width, y+14);
	GUI::Background(rcBack, RGB565(404040), RGB565(404040));
	BIOS::LCD::Print( 4, y, RGB565(808080), RGBTRANS, "Built: " __DATE__ " " __TIME__);
}

void CWndUserManager::DrawProgress()
{
	// update scroll pos
	CRect rcProgress(BIOS::LCD::Width-8, m_rcClient.top, BIOS::LCD::Width, m_rcClient.top + (1+MaxLines)*14);
	GUI::Background(rcProgress, RGB565(101010), RGB565(404040));
//	BIOS::LCD::Bar( rcProgress, RGB565(0000b0) );
	int nPercentTop = 0;
	int nPercentBottom = 1024;
	ui16 clr = RGB565(808080);
	if ( m_arrFiles.GetSize() > MaxLines )
	{
		nPercentTop = nScroll * 1024 / m_arrFiles.GetSize();
		nPercentBottom = (nScroll + MaxLines) * 1024 / m_arrFiles.GetSize();
		clr = RGB565(ffffff);
	} 
	CRect rcRange;
	rcRange.left = rcProgress.left + 2;
	rcRange.right = rcProgress.right - 2;
	rcRange.top = rcProgress.top + rcProgress.Height() * nPercentTop / 1024;
	rcRange.bottom = rcProgress.top + rcProgress.Height() * nPercentBottom / 1024;
	BIOS::LCD::Bar( rcRange, clr );
//	BIOS::LCD::PutPixel( rcRange.left, rcRange.top, RGB565(0000b0) );
//	BIOS::LCD::PutPixel( rcRange.right-1, rcRange.top, RGB565(0000b0) );
//	BIOS::LCD::PutPixel( rcRange.left, rcRange.bottom-1, RGB565(0000b0) );
//	BIOS::LCD::PutPixel( rcRange.right-1, rcRange.bottom-1, RGB565(0000b0) );
}

void CWndUserManager::DrawLine(int nLine, bool bHighlight)
{
	BIOS::FAT::TFindFile& fileInfo = m_arrFiles[nLine];
	DrawLine( fileInfo, 30+(nLine-nScroll)*14, bHighlight );
}

bool CWndUserManager::FixScrollPosition()
{
	if ( nSelected-nScroll >= MaxLines )
	{
		nScroll = nSelected - MaxLines + 1;
		return true;
	}
	if ( nSelected != -1 && nSelected-nScroll < 0 )
	{
		nScroll = nSelected;
		return true;
	}
	return false;
}

void CWndUserManager::DrawLine( BIOS::FAT::TFindFile& fileInfo, int y, bool bSelected )
{
	char strFile[13], strExt[4];
	char *pComma = strstr(fileInfo.strName, ".");

	bool bDir = (fileInfo.nAtrib & BIOS::FAT::EDirectory) ? true : false;
	bool bUp = false;
	//bool bSelected = (nSelected == i+nScroll);

	if ( strcmp(fileInfo.strName, "..") == 0 )
	{
		strcpy(strFile, fileInfo.strName);
		strcpy(strExt, "");
		bUp = true;
	} else
	if ( pComma )
	{
		_ASSERT( pComma );
		strcpy(strFile, fileInfo.strName);
		strFile[pComma - fileInfo.strName] = 0;

		memcpy(strExt, pComma+1, 3);
		strExt[3] = 0;
	} else
	{
		strcpy(strFile, fileInfo.strName);
		strcpy(strExt, "");
	}

	ui16 clr = bSelected ? RGB565(000000) : RGB565(00ffff);
	if ( bDir )
		clr = RGB565(ffffff);
	if ( fileInfo.nAtrib & BIOS::FAT::EHidden )
		clr = RGB565(00b0b0);
	ui16 clrBack = bSelected ? RGB565(00b0b0) : RGB565(0000b0);

        CRect rcBack( 0, y, /*320*/BIOS::LCD::Width-8, y+14);

	if (bSelected)
        {
	  GUI::Background(rcBack, RGB565(004040), RGB565(007070));
	} else {
	  GUI::Background(rcBack, RGB565(101010), RGB565(404040));
//  	  BIOS::LCD::Bar( 0, y, /*320*/400-8, y+14, clrBack );
  	}
        clrBack = RGBTRANS;

	if ( stricmp( strExt, "hex" ) == 0 || stricmp( strExt, "elf" ) == 0 || 
		 stricmp( strExt, "adr" ) == 0 || stricmp( strExt, "exe" ) == 0 )
	{
		clr = RGB565(00ff00);
	}
	if (bDir)
          clr = RGB565(ffffff);
	
	BIOS::LCD::Print( 4, y, clr, clrBack, strFile);
	if ( strExt[0] )
		BIOS::LCD::Print( 4+9*8+4, y, clr, clrBack, strExt);

	char strAux[16];
	if ( bUp )
		strcpy(strAux, "  Up");
	else if ( bDir )
		strcpy(strAux, "Folder");
	else if ( fileInfo.nFileLength < 1000000 )
		sprintf( strAux, "%6d", fileInfo.nFileLength );
	else 
		sprintf( strAux, "%5dk", fileInfo.nFileLength/1024 );

 	BIOS::LCD::Print( 4+15*8-4, y, clr, clrBack, strAux);
	const ui16 dwJpaDate = (2000 - 1980) << 9 | (1 << 5) | (1 << 0);

	if ( fileInfo.nDate != 0 && fileInfo.nDate != dwJpaDate )
	{
		sprintf( strAux, "%02d/%02d/%02d", (fileInfo.nDate>>5)&15, (fileInfo.nDate)&31, (fileInfo.nDate>>9)-20);
		BIOS::LCD::Print( 4+22*8+4, y, clr, clrBack, strAux);
		sprintf( strAux, "%02d:%02d", (fileInfo.nTime>>11), (fileInfo.nTime>>5)&63);
		BIOS::LCD::Print( 4+32*8+4, y, clr, clrBack, strAux);
	}
	
	DrawDelimLines(y, clr);
}

void CWndUserManager::OnKey(ui16 nKey)
{
	if ( nKey == BIOS::KEY::Down ) 
	{
		if ( nSelected + 1 < m_arrFiles.GetSize() )
		{
			nSelected++;
			if ( FixScrollPosition() )
			{
				KillTimer();
				SetTimer(150);
				DrawLine( nSelected, true );
				DrawProgress();
			}
			else
			{
				DrawLine( nSelected-1, false );
				DrawLine( nSelected, true );
			}
		}
	}
	if ( nKey == BIOS::KEY::Up ) 
	{
/*
		nSelected--;
		if ( nSelected == -1 )
			CWnd::OnKey( nKey );
		else
*/
		if (nSelected > 0)
		{
			nSelected--;
			if ( FixScrollPosition() )
			{
				KillTimer();
				SetTimer(150);
				DrawLine( nSelected, true );
				DrawProgress();
			}
			else
			{
				DrawLine( nSelected+1, false );
				DrawLine( nSelected, true );
			}
		}
	}
	if ( nKey == BIOS::KEY::Enter )
	{
		if ( m_arrFiles[nSelected].nAtrib & BIOS::FAT::EDirectory )
		{
			char strBack[16];
			strcpy(strBack, "");
			if ( m_strCurrentPath[0] == 0 )
				strcpy(m_strCurrentPath, m_arrFiles[nSelected].strName);
			else
			{
				if ( strcmp(m_arrFiles[nSelected].strName, "..") == 0 )
				{
					char* delim = strrchr( m_strCurrentPath, '/' );
					if ( delim )
					{
						strcpy(strBack, delim+1);
						*delim = 0;
					}
					else
					{
						strcpy(strBack, m_strCurrentPath);
						strcpy(m_strCurrentPath, "");
					}
				} else
				{
					strcat(m_strCurrentPath, "/");
					strcat(m_strCurrentPath, m_arrFiles[nSelected].strName);
				}
			}
			nSelected = 0;
			nScroll = 0;
			LoadFileList(m_strCurrentPath);
			SortFileList();
			if ( strlen(strBack) != 0 )
				SelectFile( strBack );
			Invalidate();
		} else
		{
			Exec(m_strCurrentPath, m_arrFiles[nSelected].strName, m_arrFiles[nSelected].nFileLength);
		}
	}
}

void CWndUserManager::OnTimer()
{
	Invalidate();
	KillTimer();
}

void CWndUserManager::SelectFile(char* strName)
{
	for (int i=0; i<m_arrFiles.GetSize(); i++)
		if ( strcmp( strName, m_arrFiles[i].strName ) == 0 )
		{
			// found!
			nSelected = i;
			FixScrollPosition();
			return;
		}
	_ASSERT(0);
}


void CWndUserManager::OnMessage(CWnd* pSender, ui16 code, ui32 data)
{
	// LAYOUT ENABLE/DISABLE FROM TOP MENU BAR
	if (code == ToWord('L', 'D') )
	{
		return;
	}

	if (code == ToWord('L', 'E') )
	{
		//Settings.Trig.Sync = CSettings::Trigger::_None; // disable sampling
		//BIOS::ADC::Enable( false );

		//test: CCookies::SetCookie( "gui.manager.last", "pmos129/appl2.elf" ); 
/*
		char* pLastFile = CCookies::GetCookie( (char*)"gui.manager.last" ); 
		char strLastFile[32];
		if ( pLastFile )
		{
			char* pEnd = strstr(pLastFile, ";");
			_ASSERT( pEnd );
			int nLen = pEnd - pLastFile;
			_ASSERT( nLen < 32 );
			memcpy( strLastFile, pLastFile, nLen );
			strLastFile[nLen] = 0;

			CCookies::SetCookie( (char*)"gui.manager.last", (char*)"" ); 

			char* pPathEnd = strrchr( strLastFile, '/' );
			if ( pPathEnd == NULL )
			{
				pLastFile = strLastFile;
				strcpy(m_strCurrentPath, "");
			}
			else
			{
				memcpy(m_strCurrentPath, strLastFile, pPathEnd - strLastFile);
				m_strCurrentPath[pPathEnd - strLastFile] = 0;
				pLastFile = pPathEnd+1;
			}
		}*/
		LoadFileList(m_strCurrentPath);
		SortFileList();
		LoadModuleList();
/*
		if ( pLastFile )
		{
			SelectFile( (char*)pLastFile );
			SetFocus();
		}

*/		return;
	}
}

void CWndUserManager::Exec(char* strPath, char* strFile, int nLength)
{
	char strFullName[64];
	if ( strPath[0] == 0 )
	{
		strcpy(strFullName, "");
	} else
	{
		strcpy(strFullName, strPath);
		strcat(strFullName, "/");
	}
	strcat(strFullName, strFile);

	char* strSuffix = strrchr(strFile, '.');
	enum {
		ENone,
		EHex,
		EElf,
		EBmp,
		EWav,
		EAdr,
		ETxt,
		EExe
	} eType = ENone;

	if ( strSuffix )
	{
		strSuffix++;
		if ( stricmp( strSuffix, "hex" ) == 0 )
			eType = EHex;
		else if ( stricmp( strSuffix, "elf" ) == 0 )
			eType = EElf;
		else if ( stricmp( strSuffix, "bmp" ) == 0 )
			eType = EBmp;
		else if ( stricmp( strSuffix, "wav" ) == 0 )
			eType = EWav;
		else if ( stricmp( strSuffix, "adr" ) == 0 )
			eType = EAdr;
		else if ( stricmp( strSuffix, "txt" ) == 0 )
			eType = ETxt;
		else if ( stricmp( strSuffix, "exe" ) == 0 )
			eType = EExe;
	}
	if ( eType == ENone )
	{
		m_wndMessage.Show(this, "Manager", "Unknown file suffix", RGB565(FF0000));
		return;
	}

	if ( eType == EElf )
	{
		m_dwExecuteAddress = ElfExecute( strFullName );
	}
	if ( eType == EHex )
	{
		ui32 dwEntry, dwBegin, dwEnd;
		if ( !HexGetInfo(strFullName, dwEntry, dwBegin, dwEnd) )
		{
			m_wndMessage.Show(this, "Manager", "Failed to load!", RGB565(FF0000));
			return;
		}
		if ( IsModuleLoaded( strFile, nLength, dwEntry, dwBegin, dwEnd ) )
		{
			//CCookies::SetCookie( (char*)"gui.manager.last", strFullName ); 
			//Settings.Save();
			BIOS::SYS::Execute( dwEntry );
			// on win32 it continues...
			Invalidate();
			return;
		}
		if ( CheckModuleConflict( dwBegin, dwEnd ) )
		{
			AddModule( strFile, nLength, dwEntry, dwBegin, dwEnd );
			SaveModuleList();
			if ( !HexLoad( strFullName ) )
			{
				m_wndMessage.Show(this, "Manager", "Failed to execute!", RGB565(FF0000));
			} else
			{
				//CCookies::SetCookie( (char*)"gui.manager.last", strFullName ); 
				//Settings.Save();
BIOS::DBG::Print("Entry=%08x\n", dwEntry);

				BIOS::SYS::Execute( dwEntry );
				// on win32 it continues...
				Invalidate();
			}
		} else
			m_wndMessage.Show(this, "Manager", "Module conflict, won't load", RGB565(FFFF00));
	}
	if ( eType == EAdr )
	{
		ui32 dwBegin, dwEnd;
		FpgaGetInfo(strFullName, dwBegin, dwEnd);
		if ( FpgaLoad(strFullName) )
		{
			m_wndMessage.Show(this, "Manager", "Image flashed successfully", RGB565(FFFF00));
			AddModule( strFile, nLength, (ui32)-1, dwBegin, dwEnd );
			SaveModuleList();
		}
		else
			m_wndMessage.Show(this, "Manager", "Error flashing", RGB565(FF0000));

	}
	if ( eType == EExe )
	{
#ifdef _WINDOWS
		char pszFileName[256];
		GetModuleFileName( NULL, pszFileName, 256 );
		char* strModuleName = strrchr( pszFileName, '\\' );
		strModuleName = strModuleName ? strModuleName + 1 : pszFileName;
		if ( stricmp(strFullName + strlen(strFullName) - strlen(strModuleName), strModuleName ) == 0 )
			m_wndMessage.Show(this, "Manager", "Already running!", RGB565(FF0000));
		else
			ShellExecuteA( NULL, "open", strFullName, NULL, NULL, CWnd::SwShow );
#else
		m_wndMessage.Show(this, "Manager", "Cannot execute Win32 app", RGB565(FF0000));
#endif
	}
}


// duplicate code from Execute.cpp
void FlashRam(uint32_t addr, uint8_t* data, uint32_t len)
{
	ui8* pWriteTo = (ui8*)addr;
	while (len--)
		*pWriteTo++ = *data++;
}

void FlashRom(uint32_t addr, uint8_t* data, uint32_t len)
{
	BIOS::MEMORY::LinearProgram(addr, data, len);
}

bool FlashData(uint32_t addr, uint8_t* data, uint32_t len)
{
	if ( (addr >> 24) == 0x20 )
	{
		FlashRam( addr, data, len );
	} else
	if ( (addr >> 24) == 0x08 )
	{
		FlashRom( addr, data, len );
	} else
		_ASSERT(!!!"Unrecognized memory location");

	return true;
}

bool CWndUserManager::HexLoad(char* strFile)
{
	CBufferedReader2 fw;
	if ( !fw.Open( strFile ) )
		return false;
	IHexRecord irec;	
	uint16_t addressOffset = 0x00;
	uint32_t address = 0x0;
	int ihexError;
	int err;
	BIOS::MEMORY::LinearStart();

	while ((ihexError = Read_IHexRecord(&irec, fw)) == IHEX_OK) 
	{
		switch(irec.type)
		{
			case IHEX_TYPE_00:    /**< Data Record */
				address = (((uint32_t) addressOffset) << 16 )+ irec.address;
				err = !FlashData(address, irec.data, irec.dataLen); //!BIOS::MEMORY::LinearProgram( address, irec.data, irec.dataLen );

				if(err)
				{
					fw.Close();
					return false;
				}
			break;

			case IHEX_TYPE_04:    /**< Extended Linear Address Record */
				addressOffset = (((uint16_t) irec.data[0]) << 8 ) + irec.data[1];		
			break;

			case IHEX_TYPE_01:    /**< End of File Record */
			case IHEX_TYPE_05:    /**< Start Linear Address Record */
			break;

			case IHEX_TYPE_02:    /**< Extended Segment Address Record */
			case IHEX_TYPE_03:    /**< Start Segment Address Record */
				fw.Close();
			return false;
		}

		if ( irec.type == IHEX_TYPE_01 )
			break;
	}

	BIOS::MEMORY::LinearFinish();

	fw.Close();
	return true;
}

bool CWndUserManager::HexGetInfo(char* strFile, ui32& dwEntry, ui32& dwBegin, ui32& dwEnd )
{
	CBufferedReader2 fw;
	if ( !fw.Open( strFile ) )
{
//		BIOS::DBG::Print("open '%s' failed\n", strFile);
		return false;
}

	IHexRecord irec;	
	uint16_t addressOffset = 0x00;
	uint32_t address = 0x0;
	int ihexError;
	ui32 dwAddrLow = (ui32)-1, dwAddrHigh = -1;

	while ((ihexError = Read_IHexRecord(&irec, fw)) == IHEX_OK) 
	{
		switch(irec.type)
		{
			case IHEX_TYPE_00:    /**< Data Record */
				address = (((uint32_t) addressOffset) << 16 )+ irec.address;

				if ( dwAddrLow == (ui32)-1 )
					dwAddrLow = dwAddrHigh = address;
				else
				{
					if ( address < dwAddrLow )
						dwAddrLow = address;
					if ( address > dwAddrHigh )
						dwAddrHigh = address;
				}
			break;

			case IHEX_TYPE_04:    /**< Extended Linear Address Record */
				addressOffset = (((uint16_t) irec.data[0]) << 8 ) + irec.data[1];		
			break;

			case IHEX_TYPE_01:    /**< End of File Record */
			case IHEX_TYPE_05:    /**< Start Linear Address Record */
			break;

			case IHEX_TYPE_02:    /**< Extended Segment Address Record */
			case IHEX_TYPE_03:    /**< Start Segment Address Record */
				fw.Close();
			return false;
		}

		if ( irec.type == IHEX_TYPE_01 )
			break;
	}

	fw.Close();
	dwEntry = dwAddrLow;
	dwBegin = dwAddrLow;
	dwEnd = dwAddrHigh;

	return true;
}

bool CWndUserManager::ElfLoad(char* strFile)
{
	CBufferedReader2 fw;
	if ( !fw.Open( strFile ) )
		return false;

	Elf32_Ehdr elfHeader;
	Elf32_Phdr elfProgram[4];

	fw >> CStream(&elfHeader, sizeof(Elf32_Ehdr));

	_ASSERT( sizeof(Elf32_Phdr) == elfHeader.phentsize );
	_ASSERT( elfHeader.phnum <= (int) COUNT(elfProgram) );

	for ( int i = 0; i < elfHeader.phnum; i++ )
		fw >> CStream(&elfProgram[i], sizeof(Elf32_Phdr));

	int nFileOffset = sizeof(Elf32_Ehdr) + elfHeader.phnum * sizeof(Elf32_Phdr);

	for ( int i = 0; i < elfHeader.phnum; i++ )
	{
		if ( elfProgram[i].offset == 0 )
		{
			// first program in regular file has offset set to 0, don't know why..
			continue;
		}
		BIOS::MEMORY::LinearStart();
		ui8 buffer[32];
		while ( nFileOffset < (int)elfProgram[i].offset )
		{
			int nToRead = elfProgram[i].offset - nFileOffset;
			if ( nToRead > 32 )
				nToRead = 32;
			fw >> CStream( buffer, nToRead );
			nFileOffset += nToRead;
		}
		_ASSERT( nFileOffset == (int)elfProgram[i].offset );

		int nOfs = 0;
		int nAddress = elfProgram[i].paddr;
		for ( nOfs = 0; nOfs < (int)elfProgram[i].filesz; )
		{
			int nToRead = 32;
			if ( nOfs + nToRead > (int)elfProgram[i].filesz )
				nToRead = elfProgram[i].filesz - nOfs;
			fw >> CStream( buffer, nToRead );
			nFileOffset += nToRead;
			nOfs += nToRead;
			//BIOS::DBG::Print("load( 0x%08x ) ", nAddress);

			if ( !BIOS::MEMORY::LinearProgram( nAddress, buffer, nToRead ) )
			{
				fw.Close();
				return false;
			}
			nAddress += nToRead;
		}

		memset( buffer, 0, sizeof(buffer) );

		for ( ; nOfs < (int)elfProgram[i].memsz; )
		{
			int nToRead = 32;
			if ( nOfs + nToRead > (int)elfProgram[i].memsz )
				nToRead = elfProgram[i].memsz - nOfs;
			nOfs += nToRead;
			if ( !BIOS::MEMORY::LinearProgram( nAddress, buffer, nToRead ) )
			{
				fw.Close();
				return false;
			}
		}
		BIOS::MEMORY::LinearFinish();
	}
	fw.Close();
	return true;
}


bool CWndUserManager::FpgaLoad(char* strFile)
{
	char* pExt = strrchr( strFile, '.');
	if ( !pExt )
		return false;
	strcpy(pExt, ".adr");

	CBufferedReader2 fw;
	if ( !fw.Open( strFile ) )
		return false;
	char strLine[32];
	fw >> CStream(strLine, 10);
	fw.Close();
	strLine[10] = 0;
	if ( strLine[0] != '0' || strLine[1] != 'x' )
		return false;
	
	ui32 dwAddr = CUtils::htoi( strLine+2 );
	
	pExt = strrchr( strFile, '.');
	if ( !pExt )
		return false;
	strcpy(pExt, ".bin");

	if ( !fw.Open( strFile ) )
		return false;
	int nLength = fw.GetFileSize();
	ui8 buffer[64];
	BIOS::MEMORY::LinearStart();
	for ( int i=0; i<nLength; )
	{
		int nToLoad = nLength-i;
		if ( nToLoad > 64 )
			nToLoad = 64;
		fw >> CStream(buffer, nToLoad);
		if ( !BIOS::MEMORY::LinearProgram( dwAddr, buffer, nToLoad) )
		{
			fw.Close();
			return false;
		}
		i += nToLoad;
		dwAddr += nToLoad;
	}
	BIOS::MEMORY::LinearFinish();
	fw.Close();
	return true;
}

bool CWndUserManager::FpgaGetInfo(char* strFile, ui32& dwBegin, ui32& dwEnd)
{
	char* pExt = strrchr( strFile, '.');
	if ( !pExt )
		return false;
	strcpy(pExt, ".adr");

	CBufferedReader2 fw;
	if ( !fw.Open( strFile ) )
	{
		BIOS::DBG::Print("Cannot open '%s' !", strFile);
		return false;
	}
	char strLine[32];
	fw >> CStream(strLine, 10);
	fw.Close();
	strLine[10] = 0;
	if ( strLine[0] != '0' || strLine[1] != 'x' )
		return false;
	
	dwBegin = CUtils::htoi( strLine+2 );
	
	pExt = strrchr( strFile, '.');
	if ( !pExt )
		return false;
	strcpy(pExt, ".bin");

	if ( !fw.Open( strFile ) )
	{
		BIOS::DBG::Print("Cannot open '%s' !", strFile);
		return false;
	}
	int nLength = fw.GetFileSize();
	fw.Close();

	dwEnd = dwBegin + nLength;
	return true;
}

bool CWndUserManager::ElfGetInfo(char* strFile, ui32& dwEntry, ui32& dwBegin, ui32& dwEnd )
{
	CBufferedReader2 fw;
	if ( !fw.Open( strFile ) )
		return false;

	Elf32_Ehdr elfHeader;
	Elf32_Phdr elfProgram;

	fw >> CStream(&elfHeader, sizeof(Elf32_Ehdr));

	_ASSERT( sizeof(Elf32_Phdr) == elfHeader.phentsize );

	dwEntry = elfHeader.entry;
	dwBegin = (ui32)-1;

	for ( int i = 0; i < elfHeader.phnum; i++ )
	{
		fw >> CStream(&elfProgram, sizeof(Elf32_Phdr));
		if ( elfProgram.offset == 0 )
			continue;
		if ( dwBegin == (ui32)-1 )
		{
			dwBegin = elfProgram.paddr;
			dwEnd = dwBegin + elfProgram.memsz;
		} else
		{
			if ( elfProgram.paddr < dwBegin )
				dwBegin = elfProgram.paddr;
			if ( elfProgram.paddr + elfProgram.memsz > dwEnd )
				dwEnd = elfProgram.paddr + elfProgram.memsz;
		}
	}
	dwEntry = dwBegin; // ignore entry
	fw.Close();
	return true;
}

bool CWndUserManager::CheckModule( char* strName, int nLength, char* strLoaded )
{
/*
	strcpy( strLoaded, "" );
	for ( int i=0; i < m_arrLoaded.GetSize(); i++)
	{
		TLoadedModule& mod = m_arrLoaded[i]; 
		if ( strcmp( strName, mod.strFileName ) != 0 )
			continue;
		if ( (int)mod.dwFileLength != nLength )
			continue;

		if ( mod.dwEntry == (ui32)-1 )
		{
			char* strExt = strrchr(strName, '.');
			if ( strExt && stricmp(strExt+1, "adr") == 0 )
			{
				strcpy(strLoaded, "Fpga");
			}
			return true;
		}
		if ( mod.dwBegin == 0x08004000 )
		{
			strcpy(strLoaded, "System");
			return true;
		}
		char chSlotBegin = '?';
		char chSlotEnd = '0';
		if ( mod.dwBegin == 0x0800C000 )
			chSlotBegin = '1';
		else if ( mod.dwBegin == 0x08014000 )
			chSlotBegin = '2';
		else if ( mod.dwBegin == 0x0801C000 )
			chSlotBegin = '3';
		else if ( mod.dwBegin == 0x08024000 )
			chSlotBegin = '4';
		
		if ( mod.dwEnd >= 0x0802C000 )
			chSlotEnd = 'X';
		else if ( mod.dwEnd >= 0x08024000 )
			chSlotEnd = '4';
		else if ( mod.dwEnd >= 0x0801C000 )
			chSlotEnd = '3';
		else if ( mod.dwEnd >= 0x08014000 )
			chSlotEnd = '2';
		else if ( mod.dwEnd >= 0x0800C000 )
			chSlotEnd = '1';

		if ( chSlotBegin == chSlotEnd )
			sprintf( strLoaded, "Slot %c", chSlotBegin );
		else if ( chSlotBegin == '?' )
			strcpy( strLoaded, "Loaded" );
		else 
			sprintf( strLoaded, "Slot %c-%c", chSlotBegin, chSlotEnd );
		break;
	}
	return strLoaded[0] != 0;
*/
  return false;
}

void CWndUserManager::AddModule( char* strName, int nLength, ui32 dwEntry, ui32 dwBegin, ui32 dwEnd )
{
/*
	for ( int i=0; i < m_arrLoaded.GetSize(); i++)
		_ASSERT( stricmp(strName, m_arrLoaded[i].strFileName) != 0 );

	TLoadedModule mod;
	memset( mod.strFileName, 0, sizeof(mod.strFileName) );
	strcpy( mod.strFileName, strName );
	mod.dwFileLength = nLength;
	mod.dwBegin = dwBegin;
	mod.dwEnd = dwEnd;
	mod.dwEntry = dwEntry;
	m_arrLoaded.Add( mod );
*/
}

bool CWndUserManager::IsModuleLoaded( char* strName, int nLength, ui32 dwEntry, ui32 dwBegin, ui32 dwEnd )
{
return false;
/*
	for ( int i=0; i < m_arrLoaded.GetSize(); i++)
	{
		TLoadedModule& mod = m_arrLoaded[i]; 
		if ( strcmp(mod.strFileName, strName) == 0 && (int)mod.dwFileLength == nLength &&
			mod.dwBegin == dwBegin && mod.dwEnd == dwEnd && mod.dwEntry == dwEntry )
		{
			return true;
		}
	}
	return false;
*/
}

bool CWndUserManager::CheckModuleConflict( ui32 dwBegin, ui32 dwEnd )
{
return true;
/*
	// check whether not loading into slot1!
	if (!( dwBegin < 0x0800C000 || dwEnd >= 0x08014000 ))
		return false;

	for ( int i=0; i < m_arrLoaded.GetSize(); i++)
	{
		TLoadedModule& mod = m_arrLoaded[i]; 
		if (!( mod.dwEnd < dwBegin || mod.dwBegin > dwEnd ))
		{
			// conflicting record
			// we should ask the user whether he/she is sure to overwrite existing module
			m_arrLoaded.RemoveAt(i--);
		}
	}
	return true; // true means ready for flashing
*/
}

uint32_t CWndUserManager::GetExecutionEntry()
{
	uint32_t temp = m_dwExecuteAddress;
	m_dwExecuteAddress = 0;
	return temp;
}
