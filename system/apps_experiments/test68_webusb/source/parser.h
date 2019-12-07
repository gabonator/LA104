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
    _ASSERT(strlen(buffer) + strlen(add) < sizeof(buffer)-1);
    strcat(buffer, add);
  }
  bool isFull()
  {
    return strstr(buffer, ");") != nullptr;
//    int len = strlen(buffer);
//    return len > 4 && buffer[len-2] == ')' && buffer[len-1] == ';';
  }
  char* get()
  {
//    char *terminator = str(buffer, ");");    
    return buffer;
  }
  void reset()
  {
    buffer[0] = 0;
  }
};

