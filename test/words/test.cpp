
#include <urand.h>
#include <actor.h>
#include <iostream>
#include <string.h>
#include <blowfish.h>
void printb(uint8_t* b){
    uintptr_t l = 16;
    for(uintptr_t i = 0; i < l ; ++i){
        printf("%02x",b[i]);
    }
    printf("\n");
}
int main(){
    URand ur ;
    union Uid128u u = {0};
    ur.getu128rand(&u);
    printb(u.b);
    u.u.high = 0xDEADBEEFFEEBDAED;
    printb(u.b);
    return 0;
}
