#pragma once
/*
#define RB_ATOMIC_START disableInterrupts()
#define RB_ATOMIC_END enbleInterrupts()

int u32 lastInterrupts = 0;

void disableInterrupts(void)
{
  __asm volatile (			   \
    "	mrs %0, primask\n" \
    : "=r" (lastInterrupts)  \
    "	mov r0, #1     \n" \
    "	msr primask, r0\n" \
    :::"r0"	 \
    );
}

void enableInterrupts(void)
{
  __asm volatile ( \
    "	msr primask, %0\n" \
    :: "r" (lastInterrupts)  \
    );
}
*/

#define RB_ATOMIC_START
#define RB_ATOMIC_END

template <typename Type, size_t MaxElements>
class RingBuffer
{
public:
  RingBuffer()
  {
       RB_ATOMIC_START
       {
           _numElements = 0;
           _head = 0;
       }
       RB_ATOMIC_END
  }

  void empty()
  {
       RB_ATOMIC_START
       {
           _numElements = 0;
           _head = 0;
       }
       RB_ATOMIC_END
  }

  bool push(Type obj)
  {
      bool ret = false;
      RB_ATOMIC_START
      {
          if (!isFull()) 
          {
              _buf[_head] = obj;
              _head = (_head + 1)%MaxElements;
              _numElements++;

              ret = true;
          }
      }
      RB_ATOMIC_END

      return ret;
  }

  Type pull()
  {
      size_t tail;

      RB_ATOMIC_START
      {
          if (size() > 0) {
              tail = getTail();
              Type dest = _buf[tail];
              _numElemen
ts--;
              return dest; // ATOMIC???
          }
      }
      RB_ATOMIC_END

      return -1;
  }

  Type peek()
  {
      size_t tail;

      RB_ATOMIC_START
      {
          if (size() > 0) {
              tail = getTail();
              return _buf[tail];// ATOMIC???
          }
      }
      RB_ATOMIC_END

      return -1;
  }

  bool isFull()
  {
      bool ret;

      RB_ATOMIC_START
      {
          ret = _numElements >= MaxElements;
      }
      RB_ATOMIC_END

      return ret;
  }

  size_t size()
  {
      size_t ret;

      RB_ATOMIC_START
      {
          ret = _numElements;
      }
      RB_ATOMIC_END

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
