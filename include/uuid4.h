#ifndef uuid4_h
#define uuid4_h

#include <urand.h>

#if defined(_WIN32)
#include <windows.h>
#include <wincrypt.h>
#endif


void uuid4_makeword(union Uuidv4u* u);
void uuid4_makestring(union Uuidv4u* u,char* dst);

int  uuid4_init(void);
void uuid4_generate(char *dst);
#endif

