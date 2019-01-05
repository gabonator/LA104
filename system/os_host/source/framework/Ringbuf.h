struct AtomicNone
{
  static void Lock() 
  {
  }
  static void Unlock()
  {
  }
};

template <typename Type, size_t MaxElements, typename AtomicPolicy = AtomicNone>
class RingBuffer
{
public:
  RingBuffer()
  {
       AtomicPolicy::Lock();
       {
           _numElements = 0;
           _head = 0;
       }
       AtomicPolicy::Unlock();
  }

  void empty()
  {
       AtomicPolicy::Lock();
       {
           _numElements = 0;
           _head = 0;
       }
       AtomicPolicy::Unlock();
  }

  bool push(Type obj)
  {
      bool ret = false;
      AtomicPolicy::Lock();
      {
          if (!isFull()) 
          {
              _buf[_head] = obj;
              _head = (_head + 1)%MaxElements;
              _numElements++;

              ret = true;
          }
      }
      AtomicPolicy::Unlock();

      return ret;
  }

  Type pull()
  {
      size_t tail;

      AtomicPolicy::Lock();
      {
          if (size() > 0) {
              tail = getTail();
              Type dest = _buf[tail];
              _numElements--;
              return dest; // TODO: ATOMIC?
          }
      }
      AtomicPolicy::Unlock();

      return -1;
  }

  Type peek()
  {
      size_t tail;

      AtomicPolicy::Lock();
      {
          if (size() > 0) {
              tail = getTail();
              return _buf[tail]; // TODO: ATOMIC?
          }
      }
      AtomicPolicy::Unlock();

      return -1;
  }

  bool isFull()
  {
      bool ret;

      AtomicPolicy::Lock();
      {
          ret = _numElements >= MaxElements;
      }
      AtomicPolicy::Unlock();

      return ret;
  }

  size_t size()
  {
      size_t ret;

      AtomicPolicy::Lock();
      {
          ret = _numElements;
      }
      AtomicPolicy::Unlock();

      return ret;
  }

  size_t available()
  {
      size_t ret;

      AtomicPolicy::Lock();
      {
          ret = MaxElements-_numElements;
      }
      AtomicPolicy::Unlock();

      return ret;
  }

protected:
  size_t getTail()
  {
      return (_head + (MaxElements - _numElements))%MaxElements;
  }

  Type _buf[MaxElements];

  size_t _head;
  size_t _numElements;
};
