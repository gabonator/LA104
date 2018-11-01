#pragma once
#include "framework/Wnd.h"
#include "gui/MessageBox.h"

class CWndUserManager : public CWnd
{
	struct TLoadedModule 
	{
		char strFileName[16]; // 13 is enough but the packing will resize it to 16
		ui32 dwFileLength;
		ui32 dwEntry;
		ui32 dwBegin;
		ui32 dwEnd;
	};
	char m_strCurrentPath[32];
	CArray<BIOS::FAT::TFindFile> m_arrFiles;
	CArray<TLoadedModule> m_arrLoaded;

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
	bool ExecTxt(char* strFile);
	void DrawProgress();
	void SelectFile(char* strName);
	void LoadModuleList();
	void SaveModuleList();
	
	bool CheckModule( char* strName, int nLength, char* strLoaded );
	void AddModule( char* strName, int nLength, ui32 dwEntry, ui32 dwBegin, ui32 dwEnd );
	bool IsModuleLoaded( char* strName, int nLength, ui32 dwEntry, ui32 dwBegin, ui32 dwEnd );
	bool CheckModuleConflict( ui32 dwBegin, ui32 dwEnd );

	// elf
	bool ElfGetInfo( char* strName, ui32& dwEntry, ui32& dwBegin, ui32& dwEnd );
	bool ElfLoad( char* strName );

	bool ElfGetInterpreter( char* strName, char* strInterpreter );
	uint32_t ElfExecute( char* strName );

	// hex
	bool HexGetInfo( char* strName, ui32& dwEntry, ui32& dwBegin, ui32& dwEnd );
	bool HexLoad( char* strName );

	// fpga
	bool FpgaGetInfo(char* strFile, ui32& dwBegin, ui32& dwEnd);
	bool FpgaLoad( char* strName );
};
