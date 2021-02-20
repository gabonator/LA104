class CPage
{
protected:
    CApplication& mApp;
    
public:
    CPage(CApplication& app) : mApp(app) {}
    virtual const char* Name() = 0;
    virtual void Draw(Layout::Render& r) = 0;
    virtual void OnKey(int key) = 0;
    virtual void OnTick() {}

protected:
    const char* Format(int n)
    {
        static char temp[16];
        sprintf(temp, "%d", n);
        return temp;
    }
};

