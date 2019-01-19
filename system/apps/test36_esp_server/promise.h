class CPromiseInternal;

class CPromise
{
public:
    using TPHandler = CPromise(*)(void);
    using TWorker = void (*) (void(*)(void), void(*)(void));
    
private:
    CPromiseInternal* pThis{nullptr};
    CPromiseInternal* pCurrent{nullptr};
    bool bRejected{false};
    
public:
    CPromise() = default;
    CPromise(CPromiseInternal*, CPromise*);
    CPromise(TWorker w);;
    CPromise Then(TPHandler h);
    CPromise Catch(TPHandler h);
    
    static CPromise Reject();
    static CPromise Resolve();

    bool Process();
    bool Finished();

    void Dump();
};

template <typename T>
class CAllocator
{
    static constexpr int mMaxInstances{16};
    T mAllocatorStorage[mMaxInstances];
    CArray<T> mAllocator;
    
public:
    CAllocator()
    {
        mAllocator.Init(mAllocatorStorage, mMaxInstances);
        mAllocator.SetSize(mMaxInstances);
    }
    
    T* New()
    {
        for (int i=0; i<mMaxInstances; i++)
        {
            if (!mAllocator[i])
                return &mAllocator[i];
        }
        _ASSERT(!"Out of allocator storage memory");
        return nullptr;
    }
    
    bool Empty()
    {
        for (int i=0; i<mMaxInstances; i++)
            if (mAllocator[i])
                return false;

        return true;
    }
};

class CPromiseInternal
{
public:
    using TPHandler = CPromise::TPHandler;
    using TWorker = CPromise::TWorker;
    
public:
    const char* id;
    TWorker mWorker;
    TPHandler mThenProc;
    TPHandler mCatchProc;
    CPromiseInternal* mNext{nullptr};
    
    static CAllocator<CPromiseInternal>& Allocator()
    {
        static CAllocator<CPromiseInternal> allocator;
        return allocator;
    }
    
    static CPromiseInternal* New()
    {
        return Allocator().New();
    }
    
    void Dispose()
    {
        mWorker = nullptr;
        mThenProc = nullptr;
        mCatchProc = nullptr;
        mNext = nullptr;
    }
    
    operator bool()
    {
        return (mWorker || mThenProc || mCatchProc);
    }
    
    CPromiseInternal* Resolve()
    {
        // move to next
        CPromiseInternal* p = this;
        CPromiseInternal* pOld = nullptr;
        while (p)
        {
            p = p->mNext;
            if (pOld)
            {
                pOld->Dispose();
                pOld = nullptr;
            }
            if (p && p->mCatchProc)
            {
                pOld = p;
                continue;
            }
            return p;
        }
        return nullptr;
    }
    
    CPromiseInternal* Reject()
    {
        // move to nearest catch, propagate error
        CPromiseInternal* p = this;
        while (p)
        {
            CPromiseInternal* pOld = p;
            p = p->mNext;
            pOld->Dispose();

            if (p && p->mCatchProc)
            {
                return p;
            }
        }
        // unhandled rejection
        return nullptr;
    }
    
    CPromiseInternal* GetLast()
    {
        CPromiseInternal* p = this;
        while (p->mNext)
            p = p->mNext;
        return p;
    }
};

CPromise::CPromise(CPromiseInternal::TWorker w)
{
    pThis = CPromiseInternal::New();
    pThis->mWorker = w;
    pCurrent = pThis;
}

CPromise::CPromise(CPromiseInternal* p, CPromise* parent)
{
    pThis = p;
    pCurrent = parent->pCurrent;
}

CPromise CPromise::Then(CPromiseInternal::TPHandler h)
{
    CPromiseInternal* pNew = CPromiseInternal::New();
    pNew->mThenProc = h;
    pThis->mNext = pNew;
    return CPromise(pNew, this);
}

CPromise CPromise::Catch(CPromiseInternal::TPHandler h)
{
    CPromiseInternal* pNew = CPromiseInternal::New();
    pNew->mCatchProc = h;
    pThis->mNext = pNew;
    return CPromise(pNew, this);
}

void CPromise::Dump()
{
    auto Attach = [](CPromiseInternal* p0, CPromiseInternal* p1)
    {
        _ASSERT(p0);
        while (p0->mNext)
            p0 = p0->mNext;
        p0->mNext = p1;
    };
    
    auto dump = [](CPromiseInternal* p){ fprintf(stdout, "(%s) ", p->id); };
    CPromiseInternal* p = pCurrent;
    while (p)
    {
        dump(p);
        if (p->mWorker)
        {
            fprintf(stdout, "resolve: ");
            // rovno resolvujme
            p = p->Resolve();
        } else if (p->mThenProc)
        {
            fprintf(stdout, "\nthen: ");
            
            CPromiseInternal* pNext = p->mNext;
            p = p->mThenProc().pCurrent;
            
            if (!p)
                p = pNext;
            else
                Attach(p, pNext);
        }
    }
}

bool CPromise::Finished()
{
    return !pCurrent;
}

bool CPromise::Process()
{
    if (!pCurrent)
        return false;

    if (bRejected && pCurrent->mCatchProc)
    {
        CPromiseInternal* pNext = pCurrent->mNext;
        CPromiseInternal* pOld = pCurrent;
        pCurrent = pCurrent->mCatchProc().pThis;

        pOld->Dispose();
        if (!pCurrent)
            pCurrent = pNext;
        else
            pCurrent->mNext = pNext;
        bRejected = false;
        return true;
    }

    if (pCurrent->mCatchProc)
    {
        CPromiseInternal* pOld = pCurrent;
        pCurrent = pCurrent->mNext;

        pOld->Dispose();
        return true;
    }

    if (pCurrent->mThenProc)
    {
        CPromiseInternal* pNext = pCurrent->mNext;
        CPromiseInternal* pOld = pCurrent;
        pCurrent = pCurrent->mThenProc().pCurrent; // TODO: pCurrent asi vsade! pThis je posledny element
        
        pOld->Dispose();
        if (!pCurrent)
            pCurrent = pNext;
        else
            pCurrent->GetLast()->mNext = pNext; // co ak ma current nejaky next!? mali by sme appendnut na last
        return true;
    }
    
    if (pCurrent->mWorker)
    {
        static CPromise* scope;
        static bool resolved;
        resolved = false;
        
        scope = this;
        pCurrent->mWorker([]()
        {
            if (resolved)
                return;
            resolved = true;
            
            CPromiseInternal* pOld = scope->pCurrent;
            scope->pCurrent = scope->pCurrent->Resolve();
            pOld->Dispose();
        }, []()
        {
            if (resolved)
                return;            
            resolved = true;
            
            CPromiseInternal* pOld = scope->pCurrent;
            scope->pCurrent = scope->pCurrent->Reject();

            pOld->Dispose();
            if (!scope->pCurrent)
            {
                _ASSERT(!"Unhandled rejection");
            }
            scope->bRejected = true;
        });
        return true;
    }
    return false;
}

CPromise CPromise::Reject()
{
    return CPromise([](auto resolve, auto reject){ reject(); });
}

CPromise CPromise::Resolve()
{
    return CPromise([](auto resolve, auto reject){ resolve(); });
}
