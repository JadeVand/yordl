#ifndef _urand_h
#define _urand_h
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
class URand{
public:
    static void getrandom(uint64_t* r,uint8_t s);
};

#endif
