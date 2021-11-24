class CTokenizer
{
  char* mString;

public:
  CTokenizer(char* str)
  {
    mString = str;
  }

  void GetToken(char* out, int maxLength)
  {
    for (int i=0; i<maxLength; i++)
    {
      out[i] = mString[i];
      if (mString[i] == 0)
      {
        mString += i;
        return;
      }
      if (mString[i] == ' ')
      {
        out[i] = 0;
        mString += i;
        mString++;
        return;
      }
    }
    mString += maxLength;
    out[maxLength] = 0;
  }
};
