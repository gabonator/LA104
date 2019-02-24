class CCodec
{
public:
    virtual const char* Id() = 0;
    virtual bool Decode(uint16_t* data, int count) { return false; }
    virtual bool Encode(uint16_t* data, int& count) { return false; }
    virtual void GetShortInfo(char* info) { strcpy(info, ""); }
    virtual void DisplayShortInfo(const CRect& rcRect) {}
    virtual void DisplayFullInfo(const CRect& rcRect) {}
    virtual void GetAttributes(char* json) { _ASSERT(0); }
    virtual void SetAttributes(char* json) { _ASSERT(0); }
};
