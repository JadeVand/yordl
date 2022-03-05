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
class Word{
private:
    std::string currentword;
    std::string category;
public:
    Word();
    ~Word();
    void getnewword(time_t servertime, time_t now);
    void getnewchampword(time_t day);
    void getnewabilityword(time_t day);
    const std::string& getword();
    const std::string& getcategory();
    bool neednewword(time_t servertime,time_t now);
private:
protected:
};
#endif
