#ifndef _urand_h
#define _urand_h
#include <random>
#define UUID4_VERSION "1.0.0"
#define UUID4_LEN 37
enum {
  UUID4_ESUCCESS =  0,
  UUID4_EFAILURE = -1
};
struct Uuidv4{
    uint64_t high;
    uint64_t low;
};
union Uuidv4u{
    struct Uuidv4 u;
    uint8_t b[16];
};
class URand{
private:
     std::mt19937 generator;
     uint64_t seed[2];
public:
    URand();
     void uuid4_makeword(union Uuidv4u* u);
     void uuid4_makestring(union Uuidv4u* u,char* dst);
     void uuid4_generate(char *dst);
 private:
     void generateuuid4seed();
     uint64_t xorshift128plus(uint64_t* s);
};

#endif
