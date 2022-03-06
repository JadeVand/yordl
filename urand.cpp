#include <urand.h>

void URand::getrandom(uint32_t* r){
    FILE* f = fopen("/dev/urandom","rb");
    assert(f);
    fread(r,sizeof(uint32_t),1,f);
    fclose(f);
}
