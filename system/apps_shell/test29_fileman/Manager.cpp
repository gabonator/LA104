#include "Manager.h"

#include "../../os_host/source/gui/Gui.h"
#include "../../os_host/source/framework/Utils.h"
#include "../../os_host/source/framework/BufferedIo.h"
#include "../../os_host/source/framework/Serialize.h"

#define MaxLines 14

int nSelected = 0;
int nScroll = 0;

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

void CWndUserManager::InitFileList()
{
	m_arrFiles.Init( m_arrFilesData, COUNT(m_arrFilesData) );
}

bool CWndUserManager::LoadFileList(char* strPath)
{
	if ( BIOS::FAT::OpenDir(strPath) != BIOS::FAT::EOk )
	{
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
	strcpy(mExtraArgument, "");
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
	if ( fileInfo.nAtrib & BIOS::FAT::EHidden || stricmp( strExt, "tmp" ) == 0)
		clr = RGB565(008080);
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

	if ( nKey == BIOS::KEY::F4 )
        {
          strcpy(mExtraArgument, m_strCurrentPath);
          strcat(mExtraArgument, "/");
          strcat(mExtraArgument, m_arrFiles[nSelected].strName);
//          BIOS::OS::SetArgument(m_arrFiles[nSelected].strName);
	  m_wndMessage.Show(this, "Manager", "File selected as argument", RGB565(00FF00));
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
		LoadFileList(m_strCurrentPath);
		SortFileList();
		return;
	}
}

void CWndUserManager::Exec(char* strPath, char* strFile, int nLength)
{
	char strFullName[128];
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
		EExe,
		ETmp
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
		else if ( stricmp( strSuffix, "tmp" ) == 0 )
			eType = ETmp;
	}

	if ( eType != EElf )
	{
		m_wndMessage.Show(this, "Manager", "Unknown file suffix", RGB565(FF0000));
		return;
	}

	if (strlen(mExtraArgument) > 0)
	{
	  strcat(strFullName, " ");
	  strcat(strFullName, mExtraArgument);
	}
        BIOS::OS::SetArgument(strFullName);

/*
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
*/
}
