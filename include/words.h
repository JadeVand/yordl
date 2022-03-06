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
union LeagueLTime{
    uint32_t t;
    struct Parts{
        uint8_t day;
        uint8_t month;
        uint16_t year;
    } p;
};
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
    
    const std::string& getword();
    const std::string& getcategory();
    bool neednewword(time_t servertime,time_t now);
    
private:
protected:
    void sethistory(std::string currentword,std::string category,time_t day);
    void getnewchampword(time_t day);
    void getnewabilityword(time_t day);
    void setcurrentword(std::string currentword,std::string category, time_t day);
};
#endif
