#ifndef __WND_H__
#define __WND_H__

#include <library.h>

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
		ui32		m_nInterval;
		ui32		m_nNext;
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

		// Window style
		WsHidden = 0,
		WsVisible = 1,
		WsNoActivate = 2,
		WsNeedRedraw = 4,
		WsModal = 8,
		WsTick = 16,
		WsListener = 32,
		
		SwShow = 1,
		SwHide = 0
	};

public:
	static CWnd*	m_pTop;					
	static CWnd*	m_pFocus;
	static ui16		m_nInstances;

	static CTimer	m_arrTimers_[16];
	static CModal	m_arrModals_[8];
	static CArray<CTimer>	m_arrTimers;
	static CArray<CModal> m_arrModals;
	static CRect m_rcOverlay;
	static CRect m_rcOverlayStack;

	CRect	m_rcClient;						// 8
	CWnd*	m_pParent;						// 4
	CWnd*	m_pFirst;						// 4
	CWnd*	m_pNext;						// 4
	ui16	m_dwFlags;						// 4
	const char* m_pszId;					// 4
	// Total 28 bytes per window (+5 virtuals)

	CWnd();
	CWnd* GetLast();
	CWnd* GetPrev();
	CWnd* GetParent();
	CWnd* GetFocus();

	void Destroy();
	void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent );
	virtual void OnPaint();
	virtual void OnKey(ui16 nKey);
	virtual void OnMessage(CWnd* pSender, ui16 code, ui32 data);
	virtual void WindowMessage(int nMsg, int nParam = 0);
	virtual void OnTimer();

	void SetFocus();
	ui8 HasFocus();
	bool IsWindow();
	bool IsVisible();
	CWnd* GetActiveWindow();
	void Invalidate();
	void SendMessage(CWnd* pTarget, ui16 code, ui32 data);
	void ShowWindow(ui8 sh);
	void SetTimer(ui32 nInterval);
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