#pragma once
#include "../../os_host/source/framework/Wnd.h"
#include "../../os_host/source/gui/MessageBox.h"

class CWndUserManager : public CWnd
{
	char m_strCurrentPath[80];
	BIOS::FAT::TFindFile m_arrFilesData[80];
	CArray<BIOS::FAT::TFindFile> m_arrFiles;
	CWndMessageBox		m_wndMessage;	
	uint32_t		m_dwExecuteAddress;
	char mExtraArgument[64];

public:
	CWndUserManager();
	virtual void Create(CWnd *pParent, ui16 dwFlags);
	virtual void OnPaint() override;
	virtual void OnTimer() override;
	virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override;
	virtual void OnKey(int nKey) override;
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
