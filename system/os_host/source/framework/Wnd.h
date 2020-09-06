#ifndef __WND_H__
#define __WND_H__

#include "Types.h"
#include "../bios/Bios.h"
#include "Classes.h"

class CWnd
{
public:
	class CTimer 
	{
	public:
		CTimer( CWnd* pWnd, ui32 nInterval ) :
			m_pWnd( pWnd ), m_nInterval( nInterval )
		{
			m_nNext = BIOS::SYS::GetTick() + nInterval;
		}
		CTimer()
		{
		}

	public:
		CWnd*		m_pWnd;		
		int			m_nInterval;
		int			m_nNext;
	};

	class CModal
	{
	public:
		CModal()
		{
			m_pPrevFocus = CWnd::m_pFocus;
			m_rcPrevOverlay = CWnd::m_rcOverlay; 
		}

		CWnd*		m_pPrevFocus;
		CRect		m_rcPrevOverlay;
	};

public:
	enum {
		// Window message
		WmPaint = 1,
		WmKey = 2,
		WmTick = 3,
		WmBroadcast = 4,
//		WmMouse = 8,
        WmWillShow = 16,
        WmWillHide = 32,
        WmWillGetFocus = 64,
        WmWillLoseFocus = 128,

		// Window style
		WsHidden = 0,
		WsVisible = 1,
		WsNoActivate = 2,
		WsNeedRedraw = 4,
		WsModal = 8,
		WsTick = 16,
		WsListener = 32
	};

public:
	static CWnd*	m_pTop;					
	static CWnd*	m_pFocus;
	static int		m_nInstances;

	static CTimer	m_arrTimers_[8];
	static CModal	m_arrModals_[4];
	static CArray<CTimer> m_arrTimers;
	static CArray<CModal> m_arrModals;
	static CRect m_rcOverlay;
	static CRect m_rcOverlayStack;

	CRect	m_rcClient;						// 8
	CWnd*	m_pParent;						// 4
	CWnd*	m_pFirst;						// 4
	CWnd*	m_pNext;						// 4
	int		m_dwFlags;						// 4
	const char* m_pszId;					// 4
	// Total 28 bytes per window (+5 virtuals)

	CWnd();
	CWnd* GetLast();
	CWnd* GetPrev();
	CWnd* GetParent();
	CWnd* GetFocus();

	void Destroy();
	void Create( const char* pszId, int dwFlags, const CRect& rc, CWnd* pParent );
	virtual void OnPaint();
	virtual void OnKey(int nKey); // TODO: use BIOS::KEY::EKey
	virtual void OnMessage(CWnd* pSender, int code, uintptr_t data);
	virtual void WindowMessage(int nMsg, int nParam = 0);
	virtual void OnTimer();

	void SetFocus();
	ui8 HasFocus();
	bool IsWindow();
	bool IsVisible();
	CWnd* GetActiveWindow();
	void Invalidate();
	void SendMessage(CWnd* pTarget, int code, uintptr_t data);
	void ShowWindow(bool bShow);
	void SetTimer(int nInterval);
	void KillTimer();
	void StartModal(CWnd* pwndChildFocus = NULL);
	void StopModal();

	const CRect& GetOverlay();
	const CModal& GetTopModal();
	void PushOverlay();
	void PopOverlay();

private:
	CWnd* _GetNextActiveWindow();
	CWnd* _GetPrevActiveWindow();
	CWnd* _GetLastActiveWindow();
	CWnd* _GetFirstActiveWindow();

	void _UpdateTimers();
};                                 

#endif
