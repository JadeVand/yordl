#include <urand.h>

URand::URand() :
    generator(std::random_device()()){
        generateuuid4seed();
}

void URand::generateuuid4seed(){
    std::uniform_int_distribution<uint64_t> distribution(0,std::numeric_limits<uint64_t>::max());
    seed[0] = distribution(generator);
    seed[1] = distribution(generator);
}
uint32_t URand::getu32rand(){
    std::uniform_int_distribution<uint32_t> distribution(0,std::numeric_limits<uint32_t>::max());
    return distribution(generator);
}
uint64_t URand::xorshift128plus(uint64_t* s){
    /* http://xorshift.di.unimi.it/xorshift128plus.c */
    uint64_t s1 = s[0];
    const uint64_t s0 = s[1];
    s[0] = s0;
    s1 ^= s1 << 23;
    s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);
    return s[1] + s0;
}
void URand::uuid4_makeword(union Uuidv4u* u){
    u->u.high = xorshift128plus(seed);
    u->u.low = xorshift128plus(seed);
}
void URand::uuid4_makestring(union Uuidv4u* u,char* dst){
    static const char *temp = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
    static const char *chars = "0123456789abcdef";
    const char *p;
    int i, n;
    p = temp;
    i = 0;
    while (*p) {
      n = u->b[i >> 1];
      n = (i & 1) ? (n >> 4) : (n & 0xf);
      switch (*p) {
        case 'x'  : *dst = chars[n];              i++;  break;
        case 'y'  : *dst = chars[(n & 0x3) + 8];  i++;  break;
        default   : *dst = *p;
      }
      dst++, p++;
    }
    *dst = '\0';
}
void URand::uuid4_generate(char *dst) {
  static const char *temp = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
  static const char *chars = "0123456789abcdef";
  union { unsigned char b[16]; uint64_t word[2]; } s;
  const char *p;
  int i, n;
  /* get random */
  s.word[0] = xorshift128plus(seed);
  s.word[1] = xorshift128plus(seed);
  /* build string */
  p = temp;
  i = 0;
  while (*p) {
    n = s.b[i >> 1];
    n = (i & 1) ? (n >> 4) : (n & 0xf);
    switch (*p) {
      case 'x'  : *dst = chars[n];              i++;  break;
      case 'y'  : *dst = chars[(n & 0x3) + 8];  i++;  break;
      default   : *dst = *p;
    }
    dst++, p++;
  }
  *dst = '\0';
}
