#ifndef _urand_h
#define _urand_h
#include <random>
#include <blowfish.h>
#define UID128LENGTH 33
struct Uid128{
    uint64_t high;
    uint64_t low;
};
union Uid128u{
    struct Uid128 u;
    uint8_t b[16];
};
class URand{
private:
    std::mt19937 generator;
    BLOWFISH_CTX bctx;
public:
    URand();
    uint16_t getu16rand();
    void getu128rand(union Uid128u* u);
    uint64_t getu64rand();
    static void getu128string( union Uid128u* u,char* str);
    static uint32_t get32high(uint64_t v);
    static uint32_t get32low(uint64_t v);
    static uint64_t makeu64(uint32_t high,uint32_t low);
    static void makeu128bytes(union Uid128u* u,const char* str);
    void encryptu128(union Uid128u* u);
    void decryptu128(union Uid128u* u);
    
private:
};

#endif
