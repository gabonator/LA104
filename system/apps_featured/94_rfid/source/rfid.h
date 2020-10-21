MFRC522_SPI dev = MFRC522_SPI(0);
MFRC522 mfrc522 = MFRC522(&dev);

class CDebugStream
{
public:
    CDebugStream& operator <<(const char* msg)
    {
        return *this;
        uint32_t c = CONSOLE::colorFront;
        CONSOLE::Color(RGB565(808080));
        CONSOLE::Print("<%s>", msg);
        CONSOLE::colorFront = c;
        return *this;
    }
} DEBUG;

class CErrorStream
{
public:
    CErrorStream& operator <<(const char* msg)
    {
        return *this;
        uint32_t c = CONSOLE::colorFront;
        CONSOLE::Color(RGB565(ff0000));
        CONSOLE::Print("<%s>", msg);
        CONSOLE::colorFront = c;
        return *this;
    }
} ERROR;

class CRfidBlock
{
public:
    enum {
        Size = 16
    };

private:
    bool mValid{false};
    uint8_t mData[Size];

public:
    CRfidBlock()
    {
    }
    
    CRfidBlock(uint8_t* data) : mValid{true}
    {
        memcpy(mData, data, sizeof(mData));
    }
    
    void operator =(const CRfidBlock& o)
    {
        mValid = o.mValid;
        memcpy(mData, o.mData, sizeof(mData));
    }
    
    operator bool()
    {
        return mValid;
    }
    
    const uint8_t* GetData() const
    {
        return mData;
    }
    
    void ToString(char* p)
    {
        p[0] = 0;
        for (int i = 0; i < Size; i++) {
          char tag[3];
          sprintf(tag, "%02x", mData[i]);
          strcat(p, tag);
        }
    }

};

class CRfidKey
{
public:
    enum {
        Size = MFRC522::MF_KEY_SIZE
    };
    
public:
    MFRC522::MIFARE_Key mKey;

    void Set(uint8_t* data)
    {
        for (int i = 0; i < Size; i++) {
            mKey.keyByte[i] = data[i];
        }
    }
};

class CRfidCard
{
    MFRC522::Uid* mUid;
    MFRC522::StatusCode mLastError{MFRC522::STATUS_OK};
    bool mCrypto{false};
    bool mFirstReadHack{false};
    static int mRefs;
    
public:
    void operator =(const CRfidCard& card)
    {
        if (mUid && --mRefs == 0)
        {
            DEBUG << "Halt";
            mfrc522.PICC_HaltA();
            if (mCrypto)
            {
                mfrc522.PCD_StopCrypto1();
                DEBUG << "StopCrypto";
            }
        }

        mUid = card.mUid;
        if (mUid)
            mRefs++;
    }
    
    CRfidCard(const CRfidCard& card)
    {
        mUid = card.mUid;
        if (mUid)
            mRefs++;
    }
    
    CRfidCard(MFRC522::Uid* uid = nullptr) : mUid{uid}
    {
        if (mUid)
            mRefs++;
    }
    
    ~CRfidCard()
    {
        if (mUid && --mRefs == 0)
        {
            mfrc522.PICC_HaltA();
            DEBUG << "Halt";
            if (mCrypto)
            {
                mfrc522.PCD_StopCrypto1();
                DEBUG << "StopCrypto";
            }
        }
    }
    
    operator bool()
    {
        return !!mUid;
    }
    
    const char* GetType()
    {
        MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mUid->sak);
        return mfrc522.PICC_GetTypeName(piccType);
    }

    uint8_t GetSak()
    {
        _ASSERT(mUid);
        return mUid->sak & 0x7f;
    }
    
    char* GetIdAsString()
    {
        static char temp[32];
        _ASSERT(sizeof(mUid->uidByte)*3+1 < sizeof(temp));
        strcpy(temp, "");
        for (int i = 0; i < mUid->size; i++) {
          char tag[5];
          sprintf(tag, "%s%02x", i ? "-" : "", mUid->uidByte[i]);
          strcat(temp, tag);
        };

        return temp;
    }

    uint32_t GetId()
    {
        _ASSERT(mUid->size >= 4);
        uint32_t aux = 0;
        aux |= mUid->uidByte[0];
        aux <<= 8;
        aux |= mUid->uidByte[1];
        aux <<= 8;
        aux |= mUid->uidByte[2];
        aux <<= 8;
        aux |= mUid->uidByte[3];
        return aux;
    }
    
    CRfidBlock ReadBlock(int n, const CRfidKey& key)
    {
        _ASSERT(mUid);
        /*
        if (!mFirstReadHack)
        {       
            if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
            {
                CONSOLE::Print(F("<firstReadError>"));
                return CRfidBlock();
            }
        	mFirstReadHack = true;
        }
*/
        mCrypto = true;
        mLastError = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, n, &key.mKey, mUid);
        if (mLastError != MFRC522::STATUS_OK)
        {
            ERROR << "Auth" << mfrc522.GetStatusCodeName(mLastError);
            return CRfidBlock();
        }

        uint8_t buffer[CRfidBlock::Size+2];
        uint8_t byteCount = COUNT(buffer);
        mLastError = mfrc522.MIFARE_Read(n, buffer, &byteCount);
        if (mLastError != MFRC522::STATUS_OK)
        {
            ERROR << "Read" << mfrc522.GetStatusCodeName(mLastError);
            return CRfidBlock();
        }
        
        _ASSERT(byteCount == CRfidBlock::Size+2);
        
        return CRfidBlock(buffer);
    }

    bool WriteBlock(int n, const CRfidKey& keyA, const CRfidKey& keyB, const CRfidBlock& block)
    {
        _ASSERT(mUid);
        /*
        if (!mFirstReadHack)
        {
            if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
            {
                CONSOLE::Print(F("<firstReadError>"));
                return false;
            }
            mFirstReadHack = true;
        }
*/
        mCrypto = true;
        mLastError = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, n, &keyA.mKey, mUid);

        if (mLastError != MFRC522::STATUS_OK)
        {
            ERROR << "AuthA" << mfrc522.GetStatusCodeName(mLastError);
            return false;
        }

        mLastError = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, n, &keyB.mKey, mUid);

        if (mLastError != MFRC522::STATUS_OK)
        {
            ERROR << "AuthB" << mfrc522.GetStatusCodeName(mLastError);
            return false;
        }

        mLastError = mfrc522.MIFARE_Write(n, block.GetData(), block.Size);
        
        if (mLastError != MFRC522::STATUS_OK)
        {
            ERROR << "Write" << mfrc522.GetStatusCodeName(mLastError);
            return false;
        }
        
        return true;
    }
    
    bool WriteMagic(int n, const CRfidBlock& block)
    {
        if (!mfrc522.MIFARE_OpenUidBackdoor(true))
        {
            ERROR << "Backdoor";
        }
        
        mLastError = mfrc522.MIFARE_Write(n, block.GetData(), block.Size);
        
        return mLastError == MFRC522::STATUS_OK;
    }
    
    const char* GetLastError()
    {
        if (mLastError == MFRC522::STATUS_OK)
            return nullptr;
        return mfrc522.GetStatusCodeName(mLastError);
    }
};

class CRfid
{
    char mIdentify[32];
    
public:
    bool Init()
    {
        mfrc522.PCD_Init();
        mfrc522.PCD_DumpVersionToSerial(mIdentify);
        return strlen(mIdentify) > 0;
    }
    
    CRfidCard GetNewCard()
    {
#ifdef __APPLE__
        EVERY(2000) { return CRfidCard(&mfrc522.uid); }
        return CRfidCard();
#endif
        if (!mfrc522.PICC_IsNewCardPresent())
            return CRfidCard();
        if (!mfrc522.PICC_ReadCardSerial())
            return CRfidCard();
        DEBUG << "New";
        return CRfidCard(&mfrc522.uid);
    }
    
    char* GetInfo()
    {
        return mIdentify;
    }
};

int CRfidCard::mRefs{0};
