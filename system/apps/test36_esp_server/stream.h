class CDataStream
{
public:
    virtual CDataStream& operator <<(const char*) = 0;
    virtual CDataStream& operator <<(int n)
    {
        char temp[16];
        sprintf(temp, "%d", n);
        return *this << temp;
    }
};

class CStreamCounter : public CDataStream
{
    int mCounter{0};
    
public:
    int Count()
    {
        return mCounter;
    }
    
    CDataStream& operator <<(const char* data) override
    {
        mCounter += strlen(data);
        return *this;
    }
};

class CStreamSerial : public CDataStream
{
public:
    virtual CDataStream& operator <<(const char* data) override
    {
        while (*data)
            GPIO::UART::Write(*data++);
        return *this;
    }
};
