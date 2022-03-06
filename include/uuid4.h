#ifndef uuid4_h
#define uuid4_h
#define UUID4_VERSION "1.0.0"
#define UUID4_LEN 37
#include <urand.h>

#if defined(_WIN32)
#include <windows.h>
#include <wincrypt.h>
#endif

enum {
  UUID4_ESUCCESS =  0,
  UUID4_EFAILURE = -1
};
struct Uuidv4{
    uint64_t high;
    uint64_t low;
};
union Uuidv4u{
    struct Uuidv4 u;
    uint8_t b[16];
};
void uuid4_makeword(union Uuidv4u* u);
void uuid4_makestring(union Uuidv4u* u,char* dst);

int  uuid4_init(void);
void uuid4_generate(char *dst);
#endif

