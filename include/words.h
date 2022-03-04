#ifndef _word_h
#define _word_h

#include <App.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <string>
#include <random>
#include <list>
#include <uuid4.h>
struct LeagueLDate{
    uint8_t day;
    uint8_t month;
    uint16_t year;
};
struct History{
    struct LeagueLDate d;
    uint32_t allowedattempts;
};
class Word{
private:
public:
private:
protected:
};
#endif
