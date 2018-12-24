#pragma once
#include "../../os_host/source/framework/Wnd.h"
#include "../../os_host/source/gui/MessageBox.h"

class CWndUserManager : public CWnd
{
	char m_strCurrentPath[40];
	BIOS::FAT::TFindFile m_arrFilesData[40];
	CArray<BIOS::FAT::TFindFile> m_arrFiles;
	CWndMessageBox		m_wndMessage;	
	uint32_t		m_dwExecuteAddress;

public:
	CWndUserManager();
	virtual void Create(CWnd *pParent, ui16 dwFlags);
	virtual void OnPaint();
	virtual void OnTimer();
	virtual void OnMessage(CWnd* pSender, ui16 code, ui32 data);
	virtual void OnKey(ui16 nKey);
	uint32_t GetExecutionEntry();

private:
	void InitFileList();
	bool LoadFileList(char* strPath);
	void SortFileList();
	static int CompareFile( BIOS::FAT::TFindFile& fA, BIOS::FAT::TFindFile& fB );
	void DrawLine( BIOS::FAT::TFindFile& fileInfo, int y, bool bSelected );
	bool FixScrollPosition();
	void DrawLine(int nLine, bool bHighlight);
	void Exec(char* strPath, char* strFile, int nLength);
	void DrawProgress();
	void SelectFile(char* strName);
};
