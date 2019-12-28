extern "C" void dbg(const char*id);

class CLineParser
{
  char buffer[256];

public:
  CLineParser()
  {
    buffer[0] = 0;
  }

  void push(char* add)
  {
    if (strlen(buffer) + strlen(add) < sizeof(buffer)-1)
    {
      strcat(buffer, add);
    } else
    {
      dbg("Discarding buffers!");
      buffer[0] = 0;
    }
  }
  bool isFull()
  {
    int len = strlen(buffer);
    return len > 4 && buffer[len-2] == ')' && buffer[len-1] == ';';
  }
  char* get()
  {
    return buffer;
  }
  void reset()
  {
    buffer[0] = 0;
  }
};

