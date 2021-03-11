#ifdef __cplusplus
extern "C" void _HandleAssertion(const char* file, int line, const char* cond);
#else
extern void _HandleAssertion(const char* file, int line, const char* cond);
#endif
#define assert(cond) { if (!(cond)) _HandleAssertion(__FILE__, __LINE__, NULL); }
