#ifndef __APPLE__
#undef fprintf
#define fprintf(f, ...) (void)(0)
#endif

// prevent using complex implementation of strstr and locale
char* strstr(char *haystack, const char *needle)
{
    if (haystack == NULL || needle == NULL) {
        return NULL;
    }
    
    for ( ; *haystack; haystack++) {
        // Is the needle at this point in the haystack?
        const char *h, *n;
        for (h = haystack, n = needle; *h && *n && (*h == *n); ++h, ++n) {
            // Match is progressing
        }
        if (*n == '\0') {
            // Found match!
            return haystack;
        }
        // Didn't match here.  Try again further along haystack.
    }
    return NULL;
}

bool isdigit(char c)
{
    return c >= '0' && c <= '9';
}

int atoi(const char *c)
{
    int value = 0;
    int sign = 1;
    if( *c == '+' || *c == '-' )
    {
        if( *c == '-' ) sign = -1;
        c++;
    }
    while (isdigit(*c))
    {
        value *= 10;
        value += (int) (*c-'0');
        c++;
    }
    return (value * sign);
}

char tolower(char c)
{
    if (c>='A' && c <= 'Z')
        return c-'A'+'a';
    return c;
}
