#include <urand.h>

URand::URand() :
    generator(std::random_device()()){

}

uint16_t URand::getu16rand(){
    std::uniform_int_distribution<uint16_t> distribution(0,std::numeric_limits<uint16_t>::max());
    return distribution(generator);
}
uint64_t URand::getu64rand(){
    std::uniform_int_distribution<uint64_t> distribution(0,std::numeric_limits<uint64_t>::max());
    return distribution(generator);
}
void URand::getu128rand(union Uid128u* u){
    u->u.high = getu64rand();
    u->u.low = getu64rand();
}

void URand::getu128string( union Uid128u* u,char* str){
    for(uint64_t index = 0; index < sizeof(struct Uid128); ++index){
        str += sprintf(str,"%02x",u->b[index]);
    }
}
