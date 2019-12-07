class CEvaluator 
{
  CRect mInternalRect;
  CRect& TakeRect(char*& s)
  {     
    CRect& rc = mInternalRect;

    s = strstr(s, "(");
    _ASSERT(s);
    s++;
    rc.left = TakeInt(s);
    rc.top = TakeInt(s);
    rc.right = TakeInt(s);
    rc.bottom = TakeInt(s);
    s = strstr(s, ")");
    _ASSERT(s);
    s++;

      while (*s == ' ' || *s == ',')
        s++;

    return rc;
  }

  void* TakePtr(char*& s)
  {
    return (void*)TakeInt(s);
  }

  uint32_t TakeHex(char*& s)
  {
    uint32_t value = 0;
    do {
      char c = *s;
      if (c >= '0' && c <= '9')
      {
        value *= 16;
        value += c - '0';
        s++;
        continue;
      }
      if (c >= 'a' && c <= 'f')
      {
        value *= 16;
        value += c - 'a' + 10;
        s++;
        continue;
      }
      if (c >= 'A' && c <= 'F')
      {
        value *= 16;
        value += c - 'a' + 10;
        s++;
        continue;
      }
      break;
    } while (1);

    _ASSERT(s[0] == ' ' || s[0] == ',' || s[0] == ')');
      while (*s == ' ' || *s == ',')
        s++;

    return value;
  }

  uint32_t TakeInt(char*& s)
  {
    while (*s == ' ')
      s++;

    if (s[0] == '0' && s[1] == 'x')
    {
      s+=2;
      return TakeHex(s);
    }

    uint32_t value = 0;
    while (s[0] >= '0' & s[0] <= '9')
    {
      value *= 10;
      value += *s++ - '0';
    }
    _ASSERT(s[0] == ' ' || s[0] == ',' || s[0] == ')');

      while (*s == ' ' || *s == ',')
        s++;

    return value;
  }

  float TakeFloat(char*& s)
  {
    while (*s == ' ')
     s++;

    _ASSERT(0);
    float f = 0.0f; //atof(s);
    while ((*s >= '0' && *s < '9') || *s == '.')
      s++;
    _ASSERT(s[0] == ' ' || s[0] == ',' || s[0] == ')');

    while (*s == ' ' || *s == ',')
      s++;

    return f;
  }

  char* TakeString(char*& s)
  {
    while (*s == ' ')
     s++;

    _ASSERT(*s == '\"');

    s++;

    char* ptr = s;
    while (*s != '\"' && *s)
      s++;

    _ASSERT(*s == '\"');
    *s = 0;
    s++;

    _ASSERT(s[0] == ' ' || s[0] == ',' || s[0] == ')');

      while (*s == ' ' || *s == ',')
        s++;

    return ptr;
  }

  char GetArgumentSignature(char*& s)
  {
    if (!*s || *s == ')')
      return 0;

    while (*s == ' ')
     s++;

    if (s[0] == 'C' && s[1] == 'R' && s[2] == 'e' && s[3] == 'c' && s[4] == 't')
    {   
      s = strstr(s, ")");
      _ASSERT(s);
      s++;
      return 'R'; 
    }

    if (*s == '\"')
    {
      s++;
      while (*s != '\"' && *s)
        s++;
      _ASSERT(s[0] == '\"');
      s++;
      _ASSERT(s[0] == ' ' || s[0] == ',' || s[0] == ')');

      while (*s == ' ' || *s == ',')
        s++;

      return 's';
    }

    if (s[0] == '0' && s[1] == 'x')
    {
      s += 2;
      TakeHex(s);
      return 'i';
    }
    // int or float

    do 
    {
      if (*s == '.')
        return 'f';
      if (*s >= '0' && *s <= '9')
      {
        s++;
        continue;
      }
      break;
    } while (1);

    if (s[0] == ' ' || s[0] == ',' || s[0] == ')')
    {
      while (*s == ' ' || *s == ',')
        s++;

      return 'i';
    }

    _ASSERT(0);
    return 0; 
  }

  char* GetArgumentsSignature(char* s)
  {
    static char signature[16] = {0};
    int sigI = 0;

    char argSig;
    do {
      argSig = GetArgumentSignature(s);
      signature[sigI++] = argSig;
    } while (argSig);

    signature[sigI] = 0;

    return signature;
  }

public:
  uint32_t Evaluate(char* command)
  {
//BIOS::DBG::Print("cmd='%s'\n", command);
    char* args = strstr(command, "(");
    *args++ = 0;

//BIOS::DBG::Print("func='%s'\n", command);
//BIOS::DBG::Print("args='%s'\n", args);

    uint32_t functionPtr = RPC::GetProcAddress(command);

    if (!functionPtr)
    {
      BIOS::DBG::Print("Function '%s' not found.", command);
      return false;
    }
    void* function = (void*)(functionPtr|1);
    char* argInterface = GetArgumentsSignature(args);

//    BIOS::DBG::Print("signature=%s\n", argInterface);

    // foreign function interface
    if (strcmp(argInterface, "") == 0)
      return ((int(*)())(function)) ();
    if (strcmp(argInterface, "i") == 0)
    {
      int a = TakeInt(args);
      return ((int(*)(int))(function)) (a);
    }
    if (strcmp(argInterface, "ii") == 0)
    {
      int a = TakeInt(args), b = TakeInt(args);
      return ((int(*)(int, int))(function)) (a, b);
    }
    if (strcmp(argInterface, "iii") == 0)
    {
      int a = TakeInt(args), b = TakeInt(args), c = TakeInt(args);
      return ((int(*)(int, int, int))(function)) (a, b, c);
    }
    if (strcmp(argInterface, "iiii") == 0)
    {
      int a = TakeInt(args), b = TakeInt(args), c = TakeInt(args), d = TakeInt(args);
      return ((int(*)(int, int, int, int))(function)) (a, b, c, d);
    }
    if (strcmp(argInterface, "iiiii") == 0)
    {
      int a = TakeInt(args), b = TakeInt(args), c = TakeInt(args), d = TakeInt(args), e = TakeInt(args);
      return ((int(*)(int, int, int, int, int))(function)) (a, b, c, d, e);
    }
    if (strcmp(argInterface, "pi") == 0)
    {
      void* a = TakePtr(args); int b = TakeInt(args);
      return ((int(*)(void*, int))(function)) (a, b);
    }
    if (strcmp(argInterface, "si") == 0)
    {
      char* a = TakeString(args); int b = TakeInt(args);
      return ((int(*)(void*, int))(function)) (a, b);
    }
    if (strcmp(argInterface, "f") == 0)
    {
      float a = TakeFloat(args);
      return ((int(*)(float))(function)) (a);
    }
    if (strcmp(argInterface, "s") == 0)
    {
      char* a = TakeString(args);
      return ((int(*)(char*))(function)) (a);
    }
    if (strcmp(argInterface, "is") == 0)
    {
      int a = TakeInt(args); char* b = TakeString(args);
      return ((int(*)(int, char*))(function)) (a, b);
    }
    if (strcmp(argInterface, "R") == 0)
    {
      CRect a = TakeRect(args);
      return ((int(*)(CRect&))(function)) (a);
    }
    if (strcmp(argInterface, "Ri") == 0)
    {
      CRect a = TakeRect(args); int b = TakeInt(args);
      return ((int(*)(CRect&, int))(function)) (a, b);
    }
    if (strcmp(argInterface, "Rii") == 0)
    {
      CRect a = TakeRect(args); int b = TakeInt(args), c = TakeInt(args);
      return ((int(*)(CRect&, int, int))(function)) (a, b, c);
    }
    if (strcmp(argInterface, "iiiis") == 0)
    {
      int a = TakeInt(args); int b = TakeInt(args); int c = TakeInt(args); int d = TakeInt(args); char* e = TakeString(args);
      return ((int(*)(int, int, int, int, char*))(function))(a, b, c, d, e);
    }
 
    BIOS::DBG::Print("Calling interface '%s' not supported.", argInterface);
    return 0;
  }

};
