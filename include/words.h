#ifndef _word_h
#define _word_h
#include <urand.h>
#include <App.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <string>
#include <fstream>
#include <algorithm>
#define LEAGUE_WORD_LENGTH 17
enum class WordValidation : uint32_t{
    kWordOk,
    kWordBadLength,
    kWordNonExist
};
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
uint8_t getrowsforlength(uint64_t length);
class Word{
private:
    std::string currentword;
    std::string category;
    URand* ur;
public:
    Word(URand* ur);
    ~Word();
    void getnewword(time_t servertime, time_t now);
    
    const std::string& getword();
    const std::string& getcategory();
    bool neednewword(time_t servertime,time_t now);
    void gethistory(std::vector<std::string>& vechistory);
    void incrementstateattempt();
    void incrementstateattemptandsuccess();
    static bool indictionary(std::string s);
    static WordValidation checkword(uint32_t* result,const std::string& guess,const std::string& correct);
    static uint8_t getrowsforlength(size_t length);
    static bool isvalidlength(const std::string& str);
private:
protected:
    void appendhistory(time_t day);
    void getnewchampword(time_t day);
    void getnewabilityword(time_t day);
    void setcurrentword(std::string currentword,std::string category, time_t day);
    static void mask(uint16_t* s, uintptr_t index);
    
    
    void removeapos(std::string& str);
};
#endif
