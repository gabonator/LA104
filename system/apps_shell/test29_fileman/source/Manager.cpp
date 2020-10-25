#include "Manager.h"

#include "../../os_host/source/gui/Gui.h"
#include "../../os_host/source/framework/Utils.h"
#include "../../os_host/source/framework/BufferedIo.h"
#include "../../os_host/source/framework/Serialize.h"

#define MaxLines 14

int nSelected = 0;
int nScroll = 0;
bool bFirstAccess = false;
//bool mCheckAutorun = false;
bool redrawTimer = false;

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

void CFolder::Init()
{
        mpFlashReadRange = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::FlashReadRange);
        mpFlashWriteRange = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::FlashWriteRange);
        mpFlashAlertRange = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::FlashAlertRange);

	if (!mpFlashReadRange)
		return;

	_ASSERT(mpFlashReadRange && mpFlashWriteRange && mpFlashAlertRange);

	mpFlashWriteRange[0] = -1;
	mpFlashWriteRange[1] = 0;
}

void CFolder::BeginRead()
{
	if (!mpFlashReadRange)
		return;

	mpFlashReadRange[0] = -1;
	mpFlashReadRange[1] = 0;
}

void CFolder::EndRead()
{
	if (!mpFlashReadRange)
		return;

	if (mpFlashReadRange[0] <= mpFlashReadRange[1])
	{
		mCurrentFolderMin = mpFlashReadRange[0];
		mCurrentFolderMax = mpFlashReadRange[1];
	} else 
	{
		// nothing was read - probably accessed from cache?
	}

	mpFlashAlertRange[0] = mCurrentFolderMin;
	mpFlashAlertRange[1] = mCurrentFolderMax;
	mAlertSet = mCurrentFolderMin <= mCurrentFolderMax;

	if (bFirstAccess)
	{
		BeginRead();
	}
}


void CFolder::Test()
{
/*
	int y = BIOS::LCD::Height-14;
        CRect rcBack( 0, y, BIOS::LCD::Width, y+14);
	GUI::Background(rcBack, RGB565(404040), RGB565(404040));
	BIOS::LCD::Printf( 4, y, RGB565(808080), RGBTRANS, "Access: RD %d..%d WR %d..%d AL %d..%d", (int)mCurrentFolderMin/BIOS::FAT::SectorSize, (int)mCurrentFolderMax/BIOS::FAT::SectorSize,
(int)mpFlashWriteRange[0]/BIOS::FAT::SectorSize, (int)mpFlashWriteRange[1]/BIOS::FAT::SectorSize,
(int)mpFlashAlertRange[0]/BIOS::FAT::SectorSize, (int)mpFlashAlertRange[1]/BIOS::FAT::SectorSize);

mpFlashReadRange[0] = -1;
mpFlashReadRange[1] = 0;
mpFlashWriteRange[0] = -1;
mpFlashWriteRange[1] = 0;
*/
}

bool CFolder::CheckModification()
{
	if (!mpFlashReadRange)
		return false;

	if (bFirstAccess && (
             (mpFlashReadRange[0] <= mpFlashReadRange[1]) ||
             (mpFlashWriteRange[0] <= mpFlashWriteRange[1])
           ))
	{
		// host os reads of writes fs for the first time - usb device is attached and ready to access
		bFirstAccess = false;
		BIOS::SYS::Beep(20);		
	}

	if (!mAlertSet)
		return false;
	if (mpFlashAlertRange[0] <= mpFlashAlertRange[1])
		return false;
	mAlertSet = false;
	BIOS::SYS::Beep(20);
	return true;
}

void CWndUserManager::InitFileList()
{
	m_arrFiles.Init( m_arrFilesData, COUNT(m_arrFilesData) );
}

bool CWndUserManager::LoadFileList(char* strPath)
{
	CFolder::BeginRead();
	if ( BIOS::FAT::OpenDir(strPath) != BIOS::FAT::EOk )
	{
		CFolder::EndRead();
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
	CFolder::EndRead();
	return true;
}

void CWndUserManager::SortFileList()
{
	m_arrFiles.Sort( CompareFile );
/*
	if (!mCheckAutorun)
		return;
	mCheckAutorun = false;
	// TODO: check duplicity or rename?
	for (int i=0; i<m_arrFiles.GetSize(); i++)
		if (strcmp(m_arrFiles[i].strName, "AUTORUN.ELF") == 0)
		{
			Exec(m_strCurrentPath, m_arrFiles[i].strName);
			return;
		}
*/
}

CWndUserManager::CWndUserManager()
{
	strcpy( m_strCurrentPath, "" );
}

void CWndUserManager::Create(CWnd *pParent, ui16 dwFlags)
{
	CWnd::Create("CWndManager", dwFlags, CRect(0, 16, BIOS::LCD::Width, BIOS::LCD::Height), pParent);
	InitFileList();
	strcpy(mExtraArgument, "");
	CFolder::Init();
        SetTimer(150);
}

/*static*/ int CWndUserManager::CompareFile( BIOS::FAT::TFindFile& fA, BIOS::FAT::TFindFile& fB )
{
	auto copyext = [](char* ext, char* to) 
        { 
        	if (!ext)
		{
			*to = 0;
			return;
		}
		strcpy(to, ext+1);
        };

	auto prefix = [](BIOS::FAT::TFindFile& f)
	{
		if (f.strName[0] == '.')
			return '0';
	        if (f.nAtrib & BIOS::FAT::EDirectory)
			return '1';
		return '2';
	};

	char fileA[8];
	char fileB[8];
	fileA[0] = prefix(fA);
	fileB[0] = prefix(fB);
	copyext(strstr(fA.strName, "."), fileA+1);
	copyext(strstr(fB.strName, "."), fileB+1);

        int diff = strcmp(fileB, fileA);
        if (diff != 0)
          return diff;

	return strcmp(fB.strName, fA.strName);
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
	if ((fileInfo.nAtrib & BIOS::FAT::EHidden) || strncmp(strExt, "TM", 2) == 0)
		clr = RGB565(008080);
	ui16 clrBack = bSelected ? RGB565(00b0b0) : RGB565(0000b0);

        CRect rcBack( 0, y, /*320*/BIOS::LCD::Width-8, y+14);

	if (bSelected)
        {
#ifdef DS213
	  GUI::Background(rcBack, RGB565(008080), RGB565(00b0b0));
#else
	  GUI::Background(rcBack, RGB565(004040), RGB565(007070));
#endif
	} else {
	  GUI::Background(rcBack, RGB565(101010), RGB565(404040));
//  	  BIOS::LCD::Bar( 0, y, /*320*/400-8, y+14, clrBack );
  	}
        clrBack = RGBTRANS;

	if ( strcmp( strExt, "HEX" ) == 0 || strcmp( strExt, "ELF" ) == 0 || 
		 strcmp( strExt, "ADR" ) == 0 || strcmp( strExt, "EXE" ) == 0 )
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

void CWndUserManager::OnKey(int nKey)
{
	if ( nKey == BIOS::KEY::Down ) 
	{
		if ( nSelected + 1 < m_arrFiles.GetSize() )
		{
			nSelected++;
			if ( FixScrollPosition() )
			{
				//KillTimer();
				//SetTimer(150);
				redrawTimer = true;
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
				redrawTimer = true;
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
			Exec(m_strCurrentPath, m_arrFiles[nSelected].strName);
		}
	}
}

void CWndUserManager::OnTimer()
{
        if (CFolder::CheckModification())
	{
		LoadFileList(m_strCurrentPath);
		SortFileList();

//		mCheckAutorun = true;
		redrawTimer = true;
	}

	if (redrawTimer)
	{
		Invalidate();	
		redrawTimer = false;
	}
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


void CWndUserManager::OnMessage(CWnd* pSender, int code, uintptr_t data)
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

void CWndUserManager::Exec(char* strPath, char* strFile)
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
	if (!strSuffix || strcmp(strSuffix, ".ELF") != 0)
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
}
