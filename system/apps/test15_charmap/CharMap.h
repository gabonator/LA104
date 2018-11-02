#include <library.h>

class CWndCharMap : public CWnd
{
public:
	virtual void Create(CWnd *pParent, ui16 dwFlags);
	virtual void OnPaint();
	virtual void OnMessage(CWnd* pSender, ui16 code, ui32 data);
	virtual void OnKey(ui16 nKey);
};
