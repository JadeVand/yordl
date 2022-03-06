#include <urand.h>

uint16_t URand::getrandom(uint16_t* r){
    FILE* f = fopen("/dev/urandom","rb");
    assert(f);
    fread(r,sizeof(uint16_t),1,f);
    fclose(f);
}
