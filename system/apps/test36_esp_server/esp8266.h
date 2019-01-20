class CEsp8266 : public CATDevice
{
    using TRequestHandler = void (*)(char*, char*);
    static CEsp8266* pInstance;
    
    TRequestHandler mRequestHandler{nullptr};

public:
    using TInquiryHandler = void (*)(char* ssid, int rssi, bool open);
    using TWebHandler = void (*)(int id, char* method, char* path, char* query);
    using TMessageGenerator = void (*)(CDataStream&);
    
public:
    CEsp8266() : CATDevice()
    {
        pInstance = this;
    }
    
    static CEsp8266& Instance()
    {
        _ASSERT(pInstance);
        return *pInstance;
    }
    
    virtual void OnRequest(char* key, char* payload)
    {
        if (mRequestHandler)
            mRequestHandler(key, payload);
    }

    CPromise Verify()
    {
        Send("AT\r\n");
        return CPromise([](auto resolve, auto reject)
        {
        if (Instance().Expect("OK\r\n"))
             resolve();
         if (Instance().Expect("ERROR") || Instance().Timeout(1000))
             reject();
        });
    }
    
    CPromise WaitFor(const char* msg, int nTimeout)
    {
        static int _nTimeout;
        static const char* _msg;
        _nTimeout = nTimeout;
        _msg = msg;
        return CPromise([](auto resolve, auto reject)
        {
            if (Instance().Expect(_msg))
                resolve();
            if (Instance().Expect("ERROR"))
                reject();
            if (Instance().Timeout(_nTimeout))
            {
                DBG::Print("WaitFor '%s' failed.\n", _msg);
                reject();
            }
        });
    }
    CPromise WaitOnlyFor(const char* msg, int nTimeout)
    {
        static int _nTimeout;
        static const char* _msg;
        _msg = msg;
        _nTimeout = nTimeout;
        return CPromise([](auto resolve, auto reject)
        {
            if (Instance().Expect(_msg))
                resolve();
            if (Instance().Timeout(_nTimeout))
            {
                DBG::Print("WaitOnlyFor '%s' failed.\n", _msg);
                reject();
            }
        });
    }
    
    CPromise SendAtCommand(char* cmd)
    {
        Send(cmd);
        return WaitFor("OK\r\n", 5000);
    }

    CPromise Wait(int milliseconds)
    {
        static long lNow;
        static long lInterval;
        lNow = SYS::GetTick();;
        lInterval = milliseconds;
        
        return CPromise([](auto resolve, auto reject)
        {
            if ((long)SYS::GetTick() - lNow > lInterval)
                resolve();
        });
    }
    
    CPromise Reset()
    {
        IgnoreGarbage(true);
        Send("\r\n\r\nAT+RST\r\n");
        return WaitOnlyFor("ready\r\n", 3000)
        .Then([](){
            Instance().IgnoreGarbage(false);
            return Instance().Verify();
        })
        .Then([](){ return Instance().NoEcho(); });
    }
    
    CPromise NoEcho()
    {
        return SendAtCommand((char*)"ATE0\r\n");
    }
    
    CPromise SetCwMode(int mode)
    {
        char command[16];
        sprintf(command, "AT+CWMODE=%d\r\n", mode);
        return SendAtCommand(command);
    }
    
    CPromise SetModeClient()
    {
        return SetCwMode(1);
    }

    CPromise SetModeAp()
    {
        return SetCwMode(2);
    }

    CPromise SetModeClientAndAp()
    {
        return SetCwMode(3);
    }
    
    CPromise SetMultiplexing(bool enable)
    {
        char command[16];
        sprintf(command, "AT+CIPMUX=%d\r\n", enable ? 1 : 0);
        return SendAtCommand(command);
    }
    
    CPromise WifiJoin(const char* ssid, const char* password)
    {
        /*
         AT+CWJAP="*****","*****"
         WIFI DISCONNECT
         WIFI CONNECTED
         WIFI GOTIP
         
         OK
         */
        char command[64];
        sprintf(command, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password);
        Send(command);
        return WaitFor("OK\r\n", 15000);
    }
    
    CPromise WifiCreate(const char* ssid, const char* password)
    {
        char command[64];
        sprintf(command, "AT+CWSAP=\"%s\",\"%s\",1,4\r\n", ssid, password);
        return SendAtCommand(command);
    }

    CPromise ServerCreate(TWebHandler handler)
    {
        static TWebHandler _handler;
        _handler = handler;
        
        mRequestHandler = [](char* key, char* payload)
        {
            //+IPD,0,414:GET /?sranda=123 HTTP/1.1

            if (strncmp(key, "+IPD,", 5) != 0)
            {
                fprintf(stdout, "Invalid IPD request: '%s'\n", key);
                return;
            }
            
            // skip buffer overflow warnings during header transmission
            //Instance().IgnoreGarbage(true);
            
            int _id = atoi(key+5);
            char* p = strstr(key+5, ",");
            if (!p)
            {
                fprintf(stdout, "Invalid IPD request: '%s'\n", key);
                return;
            }
            p++;
            
            int _length = atoi(p);
            
            Instance().IgnoreBytes(_length - strlen(payload) - 2 - 4); // TODO: not correcly calculated
//            DBG::Print("skipp %d b> ", _length);
            
            p = payload;

            char* _method = p;
            char* _path = nullptr;
            char* _query = nullptr;
            
            p = strstr(p, " ");
            if (p)
            {
                *p = 0;
                payload = p+1;
            } else
            {
                fprintf(stdout, "Invalid IPD request: '%s'\n", payload);
                return;
            }
            
            p = strstr(payload, "?");
            if (!p)
            {
                // no query: "/ HTTP/1.1"
                p = strstr(payload, " ");
                _path = payload;
                if (p)
                {
                    *p = 0;
                    payload = p+1;
                } else
                {
                    fprintf(stdout, "Invalid IPD request: '%s'\n", payload);
                    return;
                }
            } else
            {
                // with query: "/?sranda=123 HTTP/1.1"
                _path = payload;
                *p = 0;
                payload = p+1; // "sranda=123 HTTP/1.1"

                p = strstr(payload, " ");
                if (p)
                {
                    *p = 0;
                    _query = payload;
                } else
                {
                    fprintf(stdout, "Invalid IPD request: '%s'\n", payload);
                    return;
                }
            }
//            DBG::Print("id:%d, method:%s, path:%s, query:%s\n", _id, _method, _path, _query ? _query : "*null*");

            _handler(_id, _method, _path, _query);
        };
        
        return SetMultiplexing(true)
        .Then([](){ return Instance().SendAtCommand((char*)"AT+CIPSERVER=1,80\r\n"); });
    }
    
    CPromise ServerDestroy()
    {
        Instance().IgnoreGarbage(false);
        mRequestHandler = nullptr;
        return SendAtCommand((char*)"AT+CIPSERVER=0,80");
    }

    CPromise ServerResponse(int id, TMessageGenerator generator)
    {
        static int _id;
        static TMessageGenerator _generator;
        _id = id;
        _generator = generator;
        
        char cipsend[32];
        CStreamCounter counter;
        generator(counter);
        sprintf(cipsend, "AT+CIPSEND=%d,%d\r\n", id, counter.Count());
        Send(cipsend);
        // returns "\r\ntoo long\r\n\ERROR" when buffer is too big        

        return CPromise::Resolve()
        .Then([](){ Instance().ProcessAll(true); return Instance().WaitFor(">", 2000); })
        .Then([](){
            Instance().ProcessAll(false);
            _generator(Instance().Stream());
            return Instance().WaitFor("SEND OK\r\n", 3000); }) // missing CRLF??
        .Then([](){
            char msg[16];
            sprintf(msg, "AT+CIPCLOSE=%d\r\n", _id);
            return Instance().SendAtCommand(msg);
        });
    }

    CPromise SetStationMode(const char* ssid, const char* password)
    {
        static const char* _ssid;
        static const char* _password;
        _ssid = ssid;
        _password = password;
        
         return Reset()
        .Then([](){ return Instance().SetModeClient(); })
        .Then([](){ return Instance().WifiJoin(_ssid, _password); })
        .Then([](){ return Instance().SetMultiplexing(true); });
    }

    CPromise GetClients()
    {
        /*
         AT+CWLIF
         192.168.4.2,8:85:90:79:76:c9
         
         OK
         */
        return SendAtCommand((char*)"AT+CWLIF");
    }

    CPromise SetApMode(const char* ssid, const char* password)
    {
        static const char* _ssid;
        static const char* _password;
        _ssid = ssid;
        _password = password;
        
        return Reset()
        .Then([](){ return Instance().Reset(); })
        .Then([](){ return Instance().SetModeAp(); })
        .Then([](){ return Instance().WifiCreate(_ssid, _password); })
        .Then([](){ return Instance().SetMultiplexing(true); });
    }

    CPromise Inquiry(TInquiryHandler handler)
    {
        static TInquiryHandler _handler;
        _handler = handler;
        
        mRequestHandler = [](char* key, char* payload)
        {
            if (payload[0] != '(' || payload[strlen(payload)-1] != ')')
            {
                fprintf(stdout, "Invalid CWLAP response: '%s''n", payload);
                return;
            }
            
            // skip parentheses
            payload++;
            payload[strlen(payload)-1] = 0;

            static char* _tokens;
            _tokens = payload;
            
            auto getToken = []() -> char* {
                char* p = strstr(_tokens, ",");
                if (!p)
                    return nullptr;
                
                char* aux = _tokens;
                *p = 0;
                _tokens = p+1;
                return aux;
            };
            
            char* strEcn = getToken();
            if (strlen(strEcn) < 1)
            {
                fprintf(stdout, "Invalid CWLAP response: '%s''n", payload);
                return;
            }
            
            int ecn = atoi(strEcn);

            char* ssid = getToken();
            if (!ssid || strlen(ssid) < 3 || ssid[0] != '"' || ssid[strlen(ssid)-1] != '"')
            {
                fprintf(stdout, "Invalid CWLAP response: '%s''n", payload);
                return;
            }
            
            // remove quotes
            ssid++;
            ssid[strlen(ssid)-1] = 0;
            
            char* strRssi = getToken();
            if (!strRssi || strlen(strRssi) < 1)
            {
                fprintf(stdout, "Invalid CWLAP response: '%s''n", payload);
                return;
            }

            int rssi = atoi(strRssi);

            _handler(ssid, rssi, ecn == 0);
        };
        
        return SetModeClient()
        .Then([](){
            Instance().Send("AT+CWLAP\r\n");
            return Instance().WaitFor("OK\r\n", 5000)
            .Catch([]()
            {
                Instance().mRequestHandler = nullptr;
                return CPromise::Reject();
            })
            .Then([]()
            {
                Instance().mRequestHandler = nullptr;
                return CPromise::Resolve();
            });
        });
    }
};

CEsp8266* CEsp8266::pInstance{nullptr};
