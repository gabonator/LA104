class CLayoutPlay : public CWnd
{
    virtual void OnPaint()
    {
        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
        BIOS::LCD::Print(m_rcClient.Center().x - 15*4, m_rcClient.Center().y-4,
                         RGB565(ffffff), RGBTRANS, "Not implemented");
    }
};
