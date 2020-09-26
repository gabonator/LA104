#define isdigit(c) (c >= '0' && c <= '9')

float _atof(char*& s)
{
  // This function stolen from either Rolf Neugebauer or Andrew Tolmach. 
  // Probably Rolf.
  float a = 0.0;
  int e = 0;
  int c;
  while ((c = *s++) != '\0' && isdigit(c)) {
    a = a*10.0f + (c - '0');
  }
  if (c == '.') {
    while ((c = *s++) != '\0' && isdigit(c)) {
      a = a*10.0f + (c - '0');
      e = e-1;
    }
  }
  if (c == 'e' || c == 'E') {
    int sign = 1;
    int i = 0;
    c = *s++;
    if (c == '+')
      c = *s++;
    else if (c == '-') {
      c = *s++;
      sign = -1;
    }
    while (isdigit(c)) {
      i = i*10 + (c - '0');
      c = *s++;
    }
    e += i*sign;
  }

  s--; // return one char

  while (e > 0) {
    a *= 10.0f;
    e--;
  }
  while (e < 0) {
    a *= 0.1f;
    e++;
  }
  return a;
}