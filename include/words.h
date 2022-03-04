#ifndef _word_h
#define _word_h

#include <App.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <string>
#include <random>
#include <list>
#include <uuid4.h>
#include <iostream>
#include <istream>
#include <fstream>
#define LEAGUE_WORD_LENGTH 16
struct LeagueLDate{
    uint8_t day;
    uint8_t month;
    uint16_t year;
};
struct LeagueLHistory{
    struct LeagueLDate d;
    uint32_t allowedattempts;
};
class Word{
private:
    char currentword[LEAGUE_WORD_LENGTH];
    char category[LEAGUE_WORD_LENGTH];
public:
    Word();
    ~Word();
    void getnewword(time_t servertime, time_t now);
    void getnewchampword(time_t day);
    void getnewabilityword(time_t day);
    char* getword();
    char* getcategory();
    bool neednewword(time_t servertime,time_t now);
private:
protected:
};
#endif
