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
      BIOS::DBG::Print("ERROR: Discarding buffers!\n");
//      BIOS::DBG::Print("'%s'\n", buffer);
//      BIOS::DBG::Print("'%s'\n", add);
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

