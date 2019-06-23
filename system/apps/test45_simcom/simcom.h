// Efficient and reliable nonblocking state machine based SIM900 gprs driver for arduino
// Analyze and debug AT commands & responses easily
// Suitable for cooperative multitasking applications
//
// tinysine SIM900 Quad-band GSM/GPRS Shield for Arduino UNO/MEGA/Leonardo
//
// See sample code at bottom of this file
//

#include "assert.h"

#define millis() ((long)BIOS::SYS::GetTick())

void Serial_print(char c)
{
    CONSOLE::Print("%c", c);
}
void Serial_print(long l)
{
    CONSOLE::Print("%d", l);
}
void Serial_print(int n)
{
    CONSOLE::Print("%d", n);
}
void Serial_print(char* p)
{
    CONSOLE::Print("%s", p);
}
void Serial_print(const char* p)
{
    CONSOLE::Print("%s", p);
}

#define Error_print CONSOLE::Color(RGB565(ff0000)); Serial_print
#define Error_printnocrlf(msg) _gprs_printnocrlf(msg)

// Verbose output:
//#define Debug_print CONSOLE::Color(RGB565(b0b0b0)); Serial_print
//#define Debug_printnocrlf Error_printnocrlf

#define Debug_print(x) void()
#define Debug_printnocrlf(x) void()

#define Info_print CONSOLE::Color(RGB565(00ff00)); Serial_print


enum 
{
    ResponseReaderBuffer = 64, // will fit whole line of header
    RequestArgumentsLength = 128, // could be reduced to 1 when no arguments are sent    
    AtBufferLength = 32, // optimal AT parsing buffer
};

typedef void(*funcPrint_t)(char);
typedef void(*funcPower_t)(bool);

funcPrint_t __gprs_print = [](char){};
funcPower_t _gprs_power = [](bool){};

void _gprs_printnocrlf(char c)
{
    Debug_print((c != 0x0d && c != 0x0a) ? c : ' ');
}

void _gprs_printnocrlf(const char* msg)
{
    while (*msg)
        _gprs_printnocrlf(*msg++);
}

class CGprsStats
{
public:
  static long mRxBytes;
  static long mTxBytes;  
  static void rx(int n)
  {
    mRxBytes += n;    
  }
  static void tx(int n)
  {
    mTxBytes += n;  
  }
};

long CGprsStats::mRxBytes = 0;
long CGprsStats::mTxBytes = 0;

class CLineReader
{
    char mBuffer[ResponseReaderBuffer];
    uint8_t mBufferPos{0};
    bool mHasLine{false};
    bool mOverflow{false};
    bool mClearOverflow{false};
    
public:
    void operator <<(char c)
    {
        if (mClearOverflow)
        {
          mOverflow = false;
          mClearOverflow = false;
        }
        
        if (c==0x0a)
        {
            mClearOverflow = true;
            mHasLine = true;
        }
        
        _ASSERT(!isBufferFull());
        mBuffer[mBufferPos++] = c;
    }
    
    bool isBufferFull()
    {
        return mBufferPos == sizeof(mBuffer)-1;
    }
    
    bool wasOverflow()
    {
        return mOverflow;
    }
    
    bool hasLine()
    {
        return mHasLine || isBufferFull();
    }
    
    int GetLength()
    {
        return mBufferPos;
    }
    
    char* GetLine()
    {
        mOverflow |= isBufferFull();
        
        mHasLine = false;
        mBuffer[mBufferPos] = 0;
        mBufferPos = 0;
        return mBuffer;
    }
};

class CHttpResponse
{
    CLineReader mLineReader;
    long mLastCharReceived{0};
    bool mHasHead{false};
    bool mHasBreak{false};
    bool mHasPause{false};
    bool mDone{false};

public:
    virtual void Reset()
    {
        mLastCharReceived = 0;
        mLineReader.GetLine();
        mDone = false;
        mHasHead = false;
        mHasBreak = false;
        mHasPause = false;
    }

    void Terminate()
    {
      if (mDone)
        return;
        
      if (!mHasHead)
          OnHead((char*)"");

      mDone = true;
      OnFinished();
    }
    
    void operator <<(char c)
    {
        if (mDone)
            return;
        
        long now = millis();
     
        if (!mHasPause && mLastCharReceived != 0 && now >= mLastCharReceived + 1000)
        {
            int len = mLineReader.GetLength();
            char* pAll = mLineReader.GetLine();
            if (pAll && *pAll)
                OnBody(pAll, len);

            mHasPause = true;
        }
        
        mLastCharReceived = now;
        mLineReader << c;
        CGprsStats::rx(1);

        if (mLineReader.hasLine())
        {
            int len = mLineReader.GetLength();
            char* pLine = mLineReader.GetLine();

            if (mHasPause && strcmp(pLine, "\r\n")==0)
            {
              // wait for CLOSED\r\n
              return;
            }
            
            if (mHasPause && strcmp(pLine, "CLOSED\r\n")==0) 
            {
              CGprsStats::rx(-8-2);              
              OnFinished();
              mDone = true;
              return;              
            }
            
            if (mHasHead && !mHasBreak && !mLineReader.wasOverflow() && strcmp(pLine, "\r\n") == 0)
            {
                mHasBreak = true;
                return;
            }

            if (mHasHead && !mHasBreak)
            {
                pLine[len-2] = 0; // remove crlf
                OnHeader(pLine);
            }
            
            if (!mHasHead)
            {
                pLine[len-2] = 0; // remove crlf
                OnHead(pLine);
                mHasHead = true;
            }
            
            if (mHasBreak)
            {
                OnBody(pLine, len);
            }
        }
    }
    
public:
    virtual void OnHead(char* line)
    {
        // "HTTP/1.1 200 OK\r\n"
        if (strncmp(line, "HTTP/1.1 ", 9) != 0)
        {
            OnHttpCode(-1);
            return;
        }
        OnHttpCode(atoi(line+9));
    }
    
    virtual void OnHttpCode(int code)
    {
    }
    
    virtual void OnHeader(char* header)
    {
    }
    
    virtual void OnBody(char* body, int length)
    {
        // zero terminated string, or binary buffer of size length
    }
    
    virtual void OnFinished()
    {
    }
};

class CAtStream
{
public:
    virtual CAtStream& operator <<(const char*) = 0;
    //virtual CAtStream& operator <<(const __FlashStringHelper *) = 0;
    virtual CAtStream& operator <<(long n)
    {
        if (n < 0)
            return *this << "-" << (-n);
        
        int l = 0;
        char temp[16];
        
        while (n > 0)
        {
            int digit = n % 10;
            n = n / 10;
            temp[l++] = '0' + digit;
        }
        temp[l] = 0;
        
        for (int i=0; i<l/2; i++)
        {
            char t = temp[i];
            temp[i] = temp[l-1-i];
            temp[l-1-i] = t;
        }
        
        return *this << temp;
    }
};

class CAtStreamCounter : public CAtStream
{
    uint16_t mCounter{0};

public:
    uint16_t Count()
    {
        return mCounter;
    }

    CAtStream& operator <<(const char* data) override
    {
        mCounter += strlen(data);
        return *this;
    }
/*
    virtual CAtStream& operator <<(const __FlashStringHelper *data) override
    {
      typedef char PROGMEM prog_char;
      const prog_char *p = (const prog_char *)data;
      
      while (pgm_read_byte(p++)) 
        mCounter++;

      return *this;
    }
*/
};

class CAtStreamGprs : public CAtStream
{
public:
    virtual CAtStream& operator <<(const char* data) override
    {
        while (*data)
        {
            Debug_printnocrlf(*data);
            __gprs_print(*data++);
        }
        return *this;
    }
/*
    virtual CAtStream& operator <<(const __FlashStringHelper *data) override
    {
      typedef char PROGMEM prog_char;
      const prog_char *p = (const prog_char *)data;
      char c;
      
      while ((c = pgm_read_byte(p++)) != 0) 
      {
        Debug_printnocrlf(c);
        __gprs_print(c);
      }
      return *this;
    }
*/
} _gprs_stream;

class CHttpRequest
{
protected:
    // caller must ensure that these pointers are valid
    const char* mProtocol;
    const char* mHost;
    const char* mPath;
    int mPort;

public:
    virtual const char* Protocol()
    {
        return mProtocol;
    }

    virtual const char* Host()
    {
        return mHost;
    }
    
    virtual int Port()
    {
        return mPort;
    }

    virtual CAtStream& Request(CAtStream& s) = 0;
    
    virtual int Length()
    {
        CAtStreamCounter counter;
        Request(counter);
        
        return counter.Count();
    }
};

class CHttpRequestGet : public CHttpRequest
{
    char mArguments[RequestArgumentsLength];

public:
    CHttpRequestGet(const char* host, const char* path)
    {
        mProtocol = "TCP";
        mHost = host;
        mPath = path;
        mPort = 80;
        strcpy(mArguments, "");
    }
    
    void SetArguments(char *args)
    {
        _ASSERT(strlen(args) < sizeof(mArguments)-1);
        strcpy(mArguments, args);
    }
    
    virtual CAtStream& Request(CAtStream& s)
    {
      if (strlen(mArguments) > 0 )
          return s
            << "GET " << mPath << "?" << mArguments << " HTTP/1.0\r\n"
            << "Host: " << mHost << "\r\n"
            << "User-Agent: sim900 on esp8266 by valky.eu\r\n"
            << "\r\n";
        else
          return s
            << "GET " << mPath << " HTTP/1.0\r\n"
            << "Host: " << mHost << "\r\n"
            << "User-Agent: sim900 on esp8266 by valky.eu\r\n"
            << "\r\n";
    }
};

class CHttpRequestPostStream : public CHttpRequest
{
public:
    CHttpRequestPostStream(const char* host, const char* path)
    {
        mProtocol = "TCP";
        mHost = host;
        mPath = path;
        mPort = 80;
    }
    
    virtual CAtStream& Request(CAtStream& s)
    {
        CAtStreamCounter counter;
        GetArguments(counter);
      
        s << "POST " << mPath << " HTTP/1.0\r\n"
        << "Host: " << mHost << "\r\n"
        << "User-Agent: sim800L on LA104 by valky.eu\r\n"
        << "content-type: application/x-www-form-urlencoded\r\n"
        << "content-length: " << counter.Count() << "\r\n"
        << "\r\n";
        GetArguments(s);
        s << "\r\n";
        return s;
    }

    virtual void GetArguments(CAtStream& s)
    {
        _ASSERT(!"Missing GetArguments implementation");
    }
};

class CHttpRequestPost : public CHttpRequestPostStream
{
    char mArguments[RequestArgumentsLength];

public:    
    CHttpRequestPost(const char* host, const char* path) : CHttpRequestPostStream(host, path)
    {
        strcpy(mArguments, "");      
    }
    
    void SetArguments(char *args)
    {
        _ASSERT(strlen(args) < sizeof(mArguments)-1);
        strcpy(mArguments, args);
    }
    
    virtual void GetArguments(CAtStream& s)
    {
      s << mArguments;
    }      
};

class CHttpRequestJsonStream : public CHttpRequest
{
public:
    CHttpRequestJsonStream(const char* host, const char* path)
    {
        mProtocol = "TCP";
        mHost = host;
        mPath = path;
        mPort = 80;
    }
    
    virtual CAtStream& Request(CAtStream& s)
    {
        CAtStreamCounter counter;
        GetArguments(counter);
      
        s << "POST " << mPath << " HTTP/1.0\r\n"
        << "Host: " << mHost << "\r\n"
        << "User-Agent: iot-endpoint-valky-2018-1 (sim800L on LA104 by valky.eu built " __DATE__ " " __TIME__ ")\r\n"
        << "content-type: application/json\r\n"
        << "content-length: " << counter.Count() << "\r\n"
        << "\r\n"; 
        GetArguments(s);
        s << "\r\n";
        return s;
    }

    virtual void GetArguments(CAtStream& s)
    {
        _ASSERT(!"Missing GetArguments implementation");
    }    
};

class CHttpRequestJson : public CHttpRequestJsonStream
{
    char mArguments[RequestArgumentsLength];
    
public:    
    CHttpRequestJson(const char* host, const char* path) : CHttpRequestJsonStream(host, path)
    {
        strcpy(mArguments, "");      
    }
    
    void SetArguments(char *args)
    {
        _ASSERT(strlen(args) < sizeof(mArguments)-1);
        strcpy(mArguments, args);
    }
    
    virtual void GetArguments(CAtStream& s)
    {
      s << mArguments;
    }        
};


class CProgram
{
  protected:
    int8_t mPc{0};
    int8_t mNewPc{0};
    
    CProgram* mChild{nullptr};

    bool mHasReturnCode{false};
    bool mReturnCode{false};
    bool mThisFrameJumped{false};
    bool mPipe{false};
    bool mAnyReceived{false};
    bool mStreamed{false};

    long mLastTimeout{0};

    long mFrozenTill{0};
    long mLastReceivedTime{0};

protected:
    int8_t mLastTimeoutLabel{-99}; // todo: temp
    char mBuffer[AtBufferLength];
    uint8_t mBufferPos{0};
    
public:
    void Reset()
    {
        mHasReturnCode = false;
        mPc = 0;
        mLastTimeoutLabel = -99;
    }
    
    void Pipe(bool b)
    {
        mPipe = b;
    }
    
    bool isPipe()
    {
        return mPipe;
    }
    
    void operator << (char c)
    {
        mAnyReceived = true;
        if (mPipe)
        {
            static bool flushOnChar = false;
            if (c == 0x0d || c == 0x0a)
            {
                flushOnChar = true;
                //mBufferPos = 0;
            } else
            {
                if (flushOnChar)
                {
                    mBufferPos = 0;
                    flushOnChar = false;
                }
            }
            // do no proess raw text/binary stream
            if (mBufferPos >= sizeof(mBuffer)-2)
                return;
        }

        // notify only once one char earlier before reaching full buffer
        if (mBufferPos == sizeof(mBuffer)-2)
            Error("Buffer overflow");
            
        if (mBufferPos >= sizeof(mBuffer)-1)
            return;
            
        mBuffer[mBufferPos++] = c;
        mBuffer[mBufferPos] = 0;
    }
    
    int Pc()
    {
        return mPc;
    }
    
    virtual const char* Name()
    {
        return "Unnamed";
    }
    
    virtual void Program()
    {
        Error_print("No program defined\n");
    }
    
    void PrintLabel()
    {
        Debug_print(Name());
        Debug_print("[");
        Debug_print(Pc());
        Debug_print("] ");
    }
    
    void Sleep(long x)
    {
        mFrozenTill = millis() + x; //  7aug: TODO:NIE NIE NIE!!!!!
        PrintLabel();
        Debug_print("Sleep ");
        Debug_print(x);
        Debug_print("\n");
    }
    
    void DigitalWrite(bool v)
    {
        PrintLabel();
        Debug_print("DigitalWrite ");
        Debug_print(v);
        Debug_print("\n");
        _gprs_power(v);
        Next();
    }

    void Flush()
    {
        mBufferPos = 0;
        mBuffer[0] = 0;      
    }
    
    CAtStream& Stream()
    {
        Flush();    
        mStreamed = true;
        PrintLabel();
        Debug_print("Send '");
        Next();
        
        return _gprs_stream;
    }

    int BufferLength()
    {
        return (int)mBufferPos;
    }
    
    char BufferAt(int i)
    {
        //assert(i>=0 && i<mBufferPos);
        return mBuffer[i];
    }
    
    bool Expect(const char* p)
    {
        bool found = (strstr(mBuffer, p) != nullptr);
        
        PrintLabel();
        Debug_print("Expect '");
        Debug_printnocrlf(p);
        Debug_print("' Got '");
        Debug_printnocrlf(mBuffer);
        Debug_print("' = ");
        Debug_print(found ? "yes" : "no");
        Debug_print("\n");
        if (found)
        {
            mBufferPos = 0;
            Next();
        }
        return found;
    }
    
    bool ReceiveTimeout(int n)
    {
      return (long)millis() - mLastReceivedTime > n;
    }
    
    bool Timeout(long n)
    {
        if (mThisFrameJumped)
            return false;
        
        bool passed = false;
        long now = millis();
        if (mLastTimeoutLabel != Pc())
        {
            mLastTimeoutLabel = Pc();
            mLastTimeout = now;
        } else
        {
            if (now > mLastTimeout + n)
                passed = true;
        }
        
        if (passed)
        {
            Error_print("*** GPRS ");
            Error_print(Name());
            Error_print("[");
            Error_print(Pc());
            Error_print("] ");
            Error_print("TIMEOUT: ");
            Error_print(n);
            Error_print(" buffer: '");
            Error_printnocrlf(mBuffer);
            Error_print("'\n");
        } else
        {
            PrintLabel();
            Debug_print("Timeout ");
            Debug_print(mLastTimeout + n - now);
            if (passed) // TODO:
            {
                Debug_print(" !!!TIMEOUT ERROR!!!");
            }
            Debug_print("\n");
        }
        return passed;
    }
    
    void Error(const char* message)
    {
        Error_print("*** GPRS ");
        Error_print(Name());
        Error_print("[");
        Error_print(Pc());
        Error_print("] ");
        Error_print("ERROR: ");
        Error_print(message);
        Error_print(" buffer: '");
        Error_printnocrlf(mBuffer);
        Error_print("'\n");
    }
    
    void Call(CProgram& program)
    {
        PrintLabel();
        Debug_print("Call ");
        Debug_print(program.Name());
        Debug_print("\n");
        
        mChild = &program;
        mChild->Reset();
    }
    void Return(bool b)
    {
        Info_print("[");
        Info_print(Name());
        Info_print("=");
        Info_print(b);
        Info_print("]\n");

        PrintLabel();
        Debug_print("Return ");
        Debug_print(b);
        Debug_print("\n");
        mHasReturnCode = true;
        mReturnCode = b;
    }
    bool HasReturnCode(bool& ret)
    {
        ret = mReturnCode;
        return mHasReturnCode;
    }
    bool Failed()
    {
        return mHasReturnCode && !mReturnCode;
    }
    bool Succeed()
    {
        return mHasReturnCode && mReturnCode;
    }
    void Goto(int label)
    {
        mThisFrameJumped = true;
        mLastTimeoutLabel = -99;
        
        PrintLabel();
        Debug_print("Goto ");
        Debug_print(label);
        Debug_print("\n");
        mNewPc = label;
    }
    void Next()
    {
        if (!mThisFrameJumped)
          mNewPc++;
          
        mThisFrameJumped = true;
        mLastTimeoutLabel = -99;
    }
    void operator()()
    {
        if (mHasReturnCode)
            return;
        
        if (mFrozenTill != 0)
        {
            if (millis() - mFrozenTill < 0)
                return;
            mFrozenTill = 0;
            Next();
            mPc = mNewPc;
            return;
        }
        
        if (mChild)
        {
            (*mChild)();
            bool returnCode;
            if (mChild->HasReturnCode(returnCode))
            {
                Next();
                mPc = mNewPc;
                mChild = nullptr;
            }
            return;
        }
        
        mNewPc = mPc;        
        mThisFrameJumped = false;
        if (mAnyReceived)
        {
          mLastReceivedTime = millis();
          mAnyReceived = false;
        }
        
        mStreamed = false;
        Program();
        mPc = mNewPc;
        if (mStreamed)
        {
            Debug_print("'\n");
        }
    }
};

class CProgToggleSwitch : public CProgram
{
public:
    virtual const char* Name()
    {
        return "ToggleSwitch";
    }
    virtual void Program()
    {
        // 900 reset sequence
        /*
        switch (Pc())
        {
            case 0: DigitalWrite(0); break;
            case 1: Sleep(1000); break;
            case 2: DigitalWrite(1); break;
            case 3: Sleep(2000); break;
            case 4: DigitalWrite(0); break;
            case 5: Sleep(3000); break;
            default: Return(true);
        }*/
        
        // 800L reset sequence
        switch (Pc())
        {
            case 0: DigitalWrite(1); break;
            case 1: Sleep(2000); break;
            case 2: DigitalWrite(0); break;
            case 3: Flush();
                    if (Timeout(5000)) Next(); break;
            default: Return(true);
        }
    }
};

bool mLongReboot = false;


class CProgInit : public CProgram
{
    CProgToggleSwitch mProgToggleSwitch;
    
public:
    virtual const char* Name()
    {
        return "Init";
    }

    void Reboot()
    {
      Error_print("****** Reboot\n");
      Reset();
      Goto(-3);
      mPc = mNewPc;
    }
    
    virtual void Program()
    {
        switch (Pc())
        {
            case -3: Call(mProgToggleSwitch); break;
            case -2: Sleep(mLongReboot ? 30 * 1000 : 0); // TODO: remove
                     mLongReboot = false; 
                     break;
            case -1: Call(mProgToggleSwitch); break;
                    // at response
            case 0: Stream() << "AT\r\n"; break;
            case 1: if (Expect("OK\r\n")) Goto(4);
                    if (Expect("NORMAL POWER DOWN")) Goto(-1);
                    if (Timeout(1000)) Next();
                    break;
                    // at response - second try
            case 2: Stream() << "AT\r\n"; break;
            case 3: if (Expect("OK\r\n")) Goto(4);
                    if (Expect("NORMAL POWER DOWN")) Goto(-1);
                    if (Timeout(1000)) { Error("no AT response"); Goto(-1); } break;
                    // disable echo
            case 4: Stream() << "ATE0\r\n"; break;
            case 5: Expect("OK\r\n"); if (Timeout(1000)) Goto(0); break;
            case 6: Stream() << "AT+CMEE=1\r\n"; break;
            case 7: Expect("OK\r\n"); if (Timeout(1000)) Goto(0); break;
            case 8: Stream() << "AT+CFUN=1\r\n"; break;
            case 9: Expect("OK\r\n"); if (Timeout(1000)) Goto(0); break;
            case 10: Stream() << "AT+CPIN?\r\n"; break;
            case 11: Expect("+CPIN: READY\r\n");
                     if (Timeout(2000)) { Error("wrong PIN response"); Goto(0); } break;
            case 12: Expect("OK\r\n");
                     if (Timeout(2000)) Next(); break;
//            case 13: Sleep(1000);
//                     break; // mozno zbytocne
            default: Return(true); break;
        }
    }
};

class CProgAttachApn : public CProgram
{
    const char* mApn;
    CProgToggleSwitch mProgToggleSwitch;
    
    
public:
    virtual const char* Name()
    {
        return "AttachApn";
    }
    
    void SetApn(const char* apn)
    {
        mApn = apn;
    }
    
    bool ReadIpAddress()  // todo: wildcard matching?
    {
        bool begin = true;
        uint8_t addressBytes = 0;
        for (int i=0; i<BufferLength(); i++)
        {
            char c = BufferAt(i);
            if (begin)
            {
                if (c == 0x0d || c == 0x0a)
                    continue;
                begin = false;
            }
            
            if (c == 0x0d || c == 0x0a)
            {
                if (addressBytes > 5)
                {
                    Debug_print("Assigned IP address: ");
                    Debug_printnocrlf(mBuffer);
                    Debug_print("\n");
                    return true;
                }
                Error("Invalid IP address");
                return false;
            }
            
            if (c == '.' || (c >= '0' && c <= '9'))
            {
                addressBytes++;
                continue;
            }
            break;
        }
        return false;
    }

    virtual void Program()
    {
        switch (Pc())
        {
            case 0: _ASSERT(strlen(mApn)>0);
                    Stream() << "AT+CSTT=\"" << mApn << "\",\"\",\"\"\r\n";
                    break;
            case 1: Expect("OK\r\n");
                    if (Expect("ERROR")) Return(false);
                    if (Timeout(25000)) Return(false);
                break;
            case 2: Stream() << "AT+CIICR\r\n"; break;
            case 3: Expect("OK\r\n");
                    if (Timeout(15000)) Next(); break;
            case 4: mLastTimeoutLabel = -99;// TODO: wtF? 
                    Stream() << "AT+CIFSR\r\n"; break;
            case 5: if (ReadIpAddress()) Next();
                    if (Timeout(10000)) Return(false); break; // longer?
            default: Return(true);
        }
    }
};

class CProgRequest : public CProgram
{
    bool mRetry{false};
    CHttpRequest* mpRequest{nullptr};
    
public:
    virtual const char* Name()
    {
        return "Request";
    }
    
    void Start(CHttpRequest* pRequest)
    {
        mpRequest = pRequest;
        Reset();
        mRetry = false;
    }
    
    void Retry(int label)
    {
        if (mRetry)
        {
            Error("Retry failed");
            Return(false);
        }
        else
        {
            //Debug_print("Retrying...\n");
            mRetry = true;
            Goto(label);
        }
    }
    
    virtual void Program()
    {
        _ASSERT(mpRequest);
        switch (Pc())
        {
            case 0: Stream() << "AT+CIPSTART=\"" << mpRequest->Protocol() << "\",\"" << mpRequest->Host() 
                      << "\"," << mpRequest->Port() << "\r\n";
                    break;
            case 1: if (Expect("OK\r\n")) Flush();
                    if (Timeout(5000)) Return(false); 
                    if (Expect("ERROR")) Return(false);
                    break;
            case 2: Expect("CONNECT OK\r\n"); // TODO: nove, OK pride, ale connect ok nie, po 30 sekundach CIP STATUS??
            // inak CME3 po restarte!!!
                    Expect("ALREADY CONNECTED");
                    //if (Expect("STATE: TCP CLOSED\r\n")) {Flush(); Goto(1);}
                    //if (Expect("STATE: IP STATUS\r\n")) {Flush(); Goto(2);}
                    if (Expect("CONNECT FAIL")) Retry(0);
                    if (Expect("ERROR")) Retry(0);
                    if (Timeout(50000)) Return(false); 
                    break; // retry?
            case 3: {
                      int nLength = mpRequest->Length();
                      CGprsStats::tx(nLength);
                      Stream() << "AT+CIPSEND=" << nLength << "\r\n"; break;
                    }
            case 4: Expect(">");
                    if (Expect("ERROR")) Return(false); // NASTAVA!!!! 
                    if (Timeout(5000)) Return(false); break;
            case 5: mpRequest->Request(Stream()); break;
            case 6: if (Expect("SEND OK\r\n")) { Flush(); Pipe(true);} // pipe&retry?
                    if (Expect("CLOSED\r\n")) Retry(3); // asi nepomaha
                    if (Expect("SEND FAIL")) {Flush(); Goto(6);} // CLOSED follows
                    if (Timeout(30000)) Return(false); break;//zbytocne vela, casto je request prijaty ale tuto ho utne
                    break;
            case 7: if (Expect("CLOSED\r\n")) Pipe(false); // preco iba 25 vidim v logu???
                    if (ReceiveTimeout(5000) && Timeout(80000)) { Pipe(false); Return(false);} break;  // 40 000 je malo
            case 8: Stream() << "AT+CIPSTATUS\r\n"; break;
            case 9: if (Expect("OK\r\n")) Flush();
                    if (Expect("TCP CLOSED\r\n")) Goto(13); // 7aug
                    if (Timeout(5000)) Return(false); break;
            case 10: if (Expect("CONNECTED\r\n")) Goto(11);
                if (Expect("TCP CLOSED\r\n")) Goto(13); // 7aug PROBLEM!!!! mame iba : ': TCP CLOSED  '
                if (Timeout(25000)) Return(false); break;
            case 11: Stream() << "AT+CIPCLOSE\r\n"; break;
            case 12: Expect("CLOSE OK\r\n");
                if (Timeout(5000)) Return(false); break;
            case 13: Stream() << "AT+CIPSHUT\r\n"; break;
            case 14: Expect("SHUT OK\r\n");
                if (Timeout(5000)) Return(false); break;
            default: Return(true); break;
        }
    }
};

class CProgRssi : public CProgram
{
    int mRssi{-1};
    bool mActive{false}; // mActive by nemala byt sucast programu?
    
public:
    virtual const char* Name()
    {
        return "RequestRssi";
    }

    int GetRssi()
    {
      return mRssi;
    }

    virtual void Reset()
    {
      mRssi = -1;
      mActive = true;
      CProgram::Reset();
    }

    bool Active()
    {
      if (mActive)
      {
        if (Failed() || Succeed())
          mActive = false;
      }
      return mActive;
    }
    
    virtual void Program()
    {
        switch (Pc())
        {
            case 0: Stream() << "AT+CSQ\r\n";
                    break;
            case 1: // '  +CSQ: 27,0    OK  '
                    if (Expect("+CSQ: ") && Expect(","))
                    {
                      mRssi = atoi(strstr(mBuffer, "+CSQ: ")+6); 
                      Goto(2);
                    }
                    else
                    {
                      Goto(1);
                    }
                    if (Expect("ERROR")) { Error("Error getting rssi"); Return(false);}
                    if (Timeout(1000)) Return(false);
                    break;
            case 2: Expect("OK\r\n");
                    if (Timeout(3000)) Next();
                    break;
            case 3: Return(mRssi != -1); break;
        }
    }
};

class CProgPhoneNumber : public CProgram
{
    char phone[32];
    bool mActive{false}; // mActive by nemala byt sucast programu?
    
public:
    virtual const char* Name()
    {
        return "RequestPhoneNumber";
    }

    char* GetPhoneNumber()
    {
      return phone;
    }

    virtual void Reset()
    {
      strcpy(phone, "");
      mActive = true;
      CProgram::Reset();
    }

    bool Active()
    {
      if (mActive)
      {
        if (Failed() || Succeed())
          mActive = false;
      }
      return mActive;
    }
    
    virtual void Program()
    {
        switch (Pc())
        {
            case 0: Stream() << "AT+CNUM\r\n";
                    break;
            case 1: // '  +CNUM:"","15902020353",129,7,4    OK  '
                    if (Expect("+CNUM: ") && Expect("\",\""))
                    {
                      char* begin = strstr(mBuffer, "\",\"")+3;
                      char* end = strstr(begin, "\",");
                      if (end)
                      {
                        int len = end - begin;
                        if (len < 31)
                        {
                          memcpy(phone, begin, len);
                          phone[len] = 0;
                          Goto(2);
                          return;
                        }
                      }
                      Goto(1);                      
                    }
                    if (Expect("ERROR")) { Error("Error getting phone number"); Return(false);}
                    if (Timeout(10000)) Return(false);
                    break;
            case 2: Expect("OK\r\n");
                    if (Timeout(2000)) Next();
                    break;
            case 3: Return(strlen(phone) > 0); break;
        }
    }
};


class CProgImei : public CProgram
{
    char imei[32];
    bool mActive{false}; // mActive by nemala byt sucast programu?
    
public:
    virtual const char* Name()
    {
        return "RequestImei";
    }

    char* GetImei()
    {
      return imei;
    }

    virtual void Reset()
    {
      strcpy(imei, "");
      mActive = true;
      CProgram::Reset();
    }

    bool Active()
    {
      if (mActive)
      {
        if (Failed() || Succeed())
          mActive = false;
      }
      return mActive;
    }
    
    virtual void Program()
    {
        switch (Pc())
        {
            case 0: Stream() << "AT+CGSN\r\n";
                    break;
            case 1: // '  869988012018905    OK  '
                    if (Expect("\r\n"))
                    {
                      if (strlen(mBuffer) > 8 && strlen(mBuffer) < 31)
                      {
                        char* buf = mBuffer;
                        while (buf[0] == 0x0d || buf[0] == 0x0a)
                          buf++;
                        strcpy(imei, mBuffer);   
                        // TODO:
                        while (imei[strlen(imei)-1] == 0x0d || imei[strlen(imei)-1] == 0x0a)
                          imei[strlen(imei)-1] = 0;
                          
                        Goto(2);
                        return;
                      }
                      Goto(1);                      
                    }
                    if (Expect("ERROR")) { Error("Error getting imei"); Return(false);}
                    if (Timeout(10000)) Return(false);
                    break;
            case 2: Expect("OK\r\n");
                    if (Timeout(2000)) Next();
                    break;
            case 3: Return(strlen(imei) > 0); break;
        }
    }
};


class CGprs
{
    CProgInit mProgInit;
    CProgAttachApn mProgAttachApn;
    CProgRequest mProgRequest;
    bool mUploadProgress{false};
    CHttpResponse* mReceiver{nullptr};
    
public:
    void SetApn(const char* apn)
    {
        mProgAttachApn.SetApn(apn);
    }
    
    void SetReceiver(CHttpResponse* receiver)
    {
        mReceiver = receiver;
    }
    
    void AttachPrint(funcPrint_t p)
    {
        __gprs_print = p;
    }

    void AttachPower(funcPower_t p)
    {
        _gprs_power = p; // todo: name
    }

    bool isReady()
    {
        return mProgInit.Succeed() && mProgAttachApn.Succeed() && !mUploadProgress;
    }
    
    bool lastTransferFailed()
    {
        return mProgRequest.Failed();
    }
    
    bool lastTransferSucceed()
    {
        return mProgRequest.Succeed();
    }
    
    bool isReceiving()
    {
        return mUploadProgress && mProgRequest.isPipe();
    }
    
    void request(CHttpRequest& request)
    {
        if (mReceiver)
            mReceiver->Reset();
        mProgRequest.Start(&request);
        mUploadProgress = true;
    }
    
    void operator()()
    {
        if (!mProgInit.Succeed())
        {
            mProgInit();
            if (mProgInit.Failed())
                mProgInit.Reset();
            return;
        }
        
        if (!mProgAttachApn.Succeed())
        {
            mProgAttachApn();
            if (mProgAttachApn.Failed())
            {
                mLongReboot = true;
                mProgInit.Reboot();
                mProgAttachApn.Reset();
            }
            return;
        }
        
        if (mUploadProgress)
        {
            mProgRequest();
            if (mProgRequest.Succeed())
            {
                if (mReceiver)
                    (*mReceiver).Terminate();
                    
                mUploadProgress = false;
            }
            if (mProgRequest.Failed())
            {
                if (mReceiver)
                    (*mReceiver).Terminate();

                mLongReboot = true;
                mProgInit.Reboot();
                mProgAttachApn.Reset();
                mUploadProgress = false;
            }
        }
    }
    
    void operator <<(char c)
    {
        if (isReceiving() && mReceiver)
            (*mReceiver) << c;
        
        if (!mProgInit.Succeed())
            mProgInit << c;
        else
            if (!mProgAttachApn.Succeed())
                mProgAttachApn << c;
            else
                if (mUploadProgress)
                    mProgRequest << c;
                else
                {
                    Debug_print("unprocessed:");
                    Debug_print(c);
                }
    }
};

#if 0
#define pinTX 3 // TX: pin D3 of ESP8266 connected to pin D2 of SIM900
#define pinRX 2 // RX: pin D2 of ESP8266 connected to pin D3 of SIM900
#define pinPower 4 // PWRKEY: pin D4 of ESP8266 connected to pin D8 of SIM900

#include "SoftwareSerial.h"
#include "gprs.h"

class CMyHttpReceiver : public CHttpResponse
{
public:
  virtual void OnHttpCode(int code) override
  {
    if (code == 200)
      Serial.print("HTTP Response: ");
    else
    {
      Serial.print("HTTP Error: ");
      Serial.print(code);
      Serial.print("\n");
    }
  }
  
  virtual void OnBody(char* body, int length) override
  {
    Serial.print(body);
    Serial.print("\n");
  }
};

CGprs gprs;
CHttpRequestGet request("api.gabo.guru", "/test.php");
CMyHttpReceiver response;

bool shouldProcess = false;
SoftwareSerial soft(pinTX, pinRX);

void setup()
{
    pinMode(pinPower, OUTPUT);
    
    Serial.begin(115200);
    soft.begin(9600);
    
    Serial.print("\nGPRS Initialization\n");

    //gprs.SetApn("o2internet"); //O2
    gprs.SetApn("internet"); // 4a
    gprs.SetReceiver(&response);
    gprs.AttachPrint([](char c){ soft.print(c); });      
    gprs.AttachPower([](bool value){ digitalWrite(pinPower, value); });
}

void comm_yield()
{
  while (soft.available())
  {
    char c = soft.read();
    gprs << c;
    if (c == '\n')
      shouldProcess = true;
  }
}

void gprs_yield()
{
  static long last = 0;
  long now = millis();
  if (shouldProcess || now > last + 1000)
  {
    shouldProcess = false;
    last = now;
    gprs();
  }
}

void loop()
{
  comm_yield();
  gprs_yield();
  
  if (gprs.isReady())
  {
    response.Reset();
    request.SetArguments("value=17&something=nice");
    gprs.request(request);
  }
}
#endif


