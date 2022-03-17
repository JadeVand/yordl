
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
    char x[][6] = {"hello","world"};
    for(int i = 0; i < 2; ++i){
        printf("%s\n",x[i]);
    }
    return 0;
}
