#include <urand.h>

URand::URand() :
generator(std::random_device()()){
    FILE* f = fopen("keyfile","rb");
    assert(f);
    uint8_t b[sizeof(uint64_t)*2] = {0};
    fread(b,sizeof(b),1,f);
    fclose(f);
    Blowfish_Init(&bctx,b,sizeof(b));
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

uint32_t URand::get32high(uint64_t v){
    return (v>>32)&0x00000000FFFFFFFF;
}
uint32_t URand::get32low(uint64_t v){
    return v&0x00000000FFFFFFFF;
}
uint64_t URand::makeu64(uint32_t high, uint32_t low){
    uint64_t v = 0;
    v = (v | high) << 32;
    v |= low;
    return v;
}
void URand::makeu128bytes(union Uid128u* u,const char* str){
    for(uintptr_t i = 0; i < sizeof(struct Uid128);++i){
        sscanf(str + 2*i, "%02x", &u->b[i]);
    }
}
void URand::encryptu128(union Uid128u* u){
    uint32_t hhigh = URand::get32high(u->u.high);
    uint32_t hlow = URand::get32low(u->u.high);
    uint32_t lhigh = URand::get32high(u->u.low);
    uint32_t llow = URand::get32low(u->u.low);
    Blowfish_Encrypt(&bctx,&hhigh,&hlow);
    Blowfish_Encrypt(&bctx,&lhigh,&llow);
    uint64_t mu64h = URand::makeu64(hhigh,hlow);
    uint64_t mu64l = URand::makeu64(lhigh,llow);
    u->u.high = mu64h;
    u->u.low = mu64l;
}
void URand::decryptu128(union Uid128u* u){
    uint32_t hhigh = URand::get32high(u->u.high);
    uint32_t hlow = URand::get32low(u->u.high);
    uint32_t lhigh = URand::get32high(u->u.low);
    uint32_t llow = URand::get32low(u->u.low);
    Blowfish_Decrypt(&bctx,&hhigh,&hlow);
    Blowfish_Decrypt(&bctx,&lhigh,&llow);
    uint64_t mu64h = URand::makeu64(hhigh,hlow);
    uint64_t mu64l = URand::makeu64(lhigh,llow);
    u->u.high = mu64h;
    u->u.low = mu64l;
}
