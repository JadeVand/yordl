
#include <urand.h>
#include <actor.h>
#include <iostream>
#include <string.h>
#include <blowfish.h>

int main(){
    URand ur ;
    union Uid128u u = {0};
    BLOWFISH_CTX bctx;
    uint64_t r = ur.getu64rand();
    Blowfish_Init(&bctx,(uint8_t*)&r,sizeof(uint64_t));
    ur.getu128rand(&u);

    uint32_t hhigh = URand::get32high(u.u.high);
    uint32_t hlow = URand::get32low(u.u.high);
    uint32_t lhigh = URand::get32high(u.u.low);
    uint32_t llow = URand::get32low(u.u.low);
    printf("%llx %llx\n",u.u.high,u.u.low);
    printf("%08x%08x %08x%08x\n",hhigh,hlow,lhigh,llow);
    return 0;
}
