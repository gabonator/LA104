#ifdef __cplusplus
extern "C" void _HandleAssertion(const char* file, int line, const char* cond);
#else
void _HandleAssertion(const char* file, int line, const char* cond);
#endif
#define assert(e) if (!(e)) { _HandleAssertion(__FILE__, __LINE__, ""); }
