#pragma once
#include <framework/Wnd.h>
#include <gui/MessageBox.h>

class CString
{
    char mData[40];
public:
    CString()
    {
        mData[0] = 0;
    }
    CString(char* str)
    {
        strcpy(mData, str);
    }
    CString(const char* str)
    {
        strcpy(mData, str);
    }
    void operator = (char* str)
    {
        strcpy(mData, str);
    }
    operator const char* () const
    {
        return mData;
    }
};

class CLayout
{
public:
    virtual const CArray<CString>& GetPath() = 0;
    virtual const CArray<CString>& GetElements() = 0;
    virtual bool HasDetails() = 0;
    virtual const CArray<CString>& GetDetails(int row) = 0;
    virtual CLayout& Enter(int row) = 0;
    virtual CLayout& Leave(int& row) = 0;
    virtual void DrawHeading(const CRect& rc) = 0;
    virtual void DrawElement(const CRect& rc, int index, bool focus) = 0;
    virtual int Hint(const char* hint) = 0;
};

class CFolder 
{                         
	uint32_t* mpFlashReadRange{nullptr};
	uint32_t* mpFlashWriteRange{nullptr};
	uint32_t* mpFlashAlertRange{nullptr};
	uint32_t mCurrentFolderMin{(uint32_t)-1};
	uint32_t mCurrentFolderMax{0};
	bool mAlertSet{false};

public:
	void Init();
	void BeginRead();
	void EndRead();
	bool CheckModification();
	void Test();
};

struct Position
{
    int mIndex;
    int mOffset;
};

class CWndManager : public CWnd
{
    enum { MaxLines = (BIOS::LCD::Height-16*2)/14+1 };
    CLayout* mpLayout{nullptr};
    const CArray<CString>* mpItems{nullptr};
    Position mPositionData[8];
    CArray<Position> mPosition;
    
public:
    CWndManager();
	virtual void Create(CWnd *pParent, ui16 dwFlags);
	virtual void OnPaint() override;
	virtual void OnTimer() override;
	virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override;
	virtual void OnKey(int nKey) override;
private:
    void DrawProgress();
    void DrawLine(int i, bool bSelected, bool empty = false);
    void DrawLineShort(int i, bool bSelected);
    void DrawDetails();
    void Select(CLayout& layout);
};


class CWndUserManager : public CWnd, CFolder
{
    char m_strCurrentPath[80];
    BIOS::FAT::TFindFile m_arrFilesData[80];
    CArray<BIOS::FAT::TFindFile> m_arrFiles;
    CWndMessageBox        m_wndMessage;
    char mExtraArgument[64];
    CFolder            m_folder;

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
    void Exec(char* strPath, char* strFile);
    void DrawProgress();
    void SelectFile(char* strName);
};

