#include <urand.h>


void URand::getrandom(uint64_t* r,uint8_t s){
    FILE* f = fopen("/dev/urandom","rb");
    assert(f);
    
    switch (s) {
        case 8:
            fread(r,sizeof(uint8_t),1,f);
            break;
        case 16:
            fread(r,sizeof(uint16_t),1,f);
            break;
        case 32:
            fread(r,sizeof(uint32_t),1,f);
            break;
        case 64:
            fread(r,sizeof(uint64_t),1,f);
            break;
        default:
            break;
    }
    
    fclose(f);
}
