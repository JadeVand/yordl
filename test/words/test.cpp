
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
    char x[120];
    typedef char Word2d[5][5];
    Word2d* w2d = (Word2d*)x;
    char h1[] = "hello";
    char h2[] = "world";
    strncpy(*w2d[0],h1,5);
    strncpy(*w2d[1],h2,5);
    printf("%s\n",*w2d);

    return 0;
}
