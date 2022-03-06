#ifndef _urand_h
#define _urand_h
#include <random>
#include <iostream>
#include <fstream>
#include <assert.h>
class URand{
public:
   static void getrandom(uint32_t* r);
};

#endif
