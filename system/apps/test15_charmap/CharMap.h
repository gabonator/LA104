#include <library.h>

class CWndCharMap : public CWnd
{
public:
	virtual void Create(CWnd *pParent, ui16 dwFlags);
	virtual void OnPaint() override;
	virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override;
	virtual void OnKey(int nKey) override;
};
