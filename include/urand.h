#ifndef _urand_h
#define _urand_h
#include <random>
#include <iostream>
#include <fstream>
#include <assert.h>
class URand{
public:
   static uint16_t getrandom(uint16_t* r);
};

#endif
