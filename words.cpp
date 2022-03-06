

#include "words.h"

Word::Word(){
}
Word::~Word(){
    
}
#define SECONDS_IN_DAY 86400
bool Word::neednewword(time_t servertime,time_t now){
    
    time_t diff = now - servertime;
    diff/=SECONDS_IN_DAY;//this is our day, we check if this file currently exists, if not we make a new file with our new word
    
    std::ifstream ifs("league-l-currentword.json");
    assert(ifs.good());
    nlohmann::json jf = nlohmann::json::parse(ifs);
    ifs.close();
    time_t day = jf.at("day");
    if(diff==day){
        return false;
    }
    return true;
}
void Word::sethistory(std::string currentword,std::string category,time_t day){
    std::ifstream ifhistory("league-l-history.json");
    assert(ifhistory.good());
    nlohmann::json jshistory = nlohmann::json::parse(ifhistory);
    ifhistory.close();
    std::time_t t = std::time(nullptr);
    std::tm *const pTInfo = std::localtime(&t);
    union LeagueLTime ltl = {0};
    ltl.p.day = (uint8_t)pTInfo->tm_mday;
    ltl.p.month = (uint8_t)pTInfo->tm_mon;
    ltl.p.day = (uint16_t)(pTInfo->tm_year+1900);
    jshistory[currentword] = nlohmann::json::object();
    jshistory["category"] = category;
    jshistory[currentword]["day"] = day;
    jshistory[currentword]["date"] = ltl.t;
    jshistory[currentword]["attempts"] = 0;
    jshistory[currentword]["successful"] = 0;
    std::ofstream ofhistory("history.json");
    assert(ofhistory.good());
    ofhistory << jshistory;
    ofhistory.close();
}
void Word::setcurrentword(std::string currentword,std::string category, time_t day){
    this->currentword.assign(currentword);
    this->category.assign(category);
    std::ofstream ofs("league-l-currentword.json");
    
    assert(ofs.good());
    
    nlohmann::json j ;
    j[currentword] = nlohmann::json::object();
    j[currentword]["category"] = category;
    j[currentword]["day"] = day;
    ofs << j;
    ofs.close();
}
void Word::getnewword(time_t servertime,time_t now){
    
    
    std::string assignedword;
    std::string assignedcategory;
    time_t diff = now - servertime;
    diff/=SECONDS_IN_DAY;//this is our day, we check if this file currently exists, if not we make a new file with our new word
    std::ifstream ifs("league-l-assignedword.json");
    assert(ifs.good());
    nlohmann::json jf = nlohmann::json::parse(ifs);
    ifs.close();
    for (auto& [key, val] : jf.items())
    {
        time_t day = val.at("day");
        if(day==diff){
            std::string c = val.at("category");
            assert(!c.empty());
            
            assignedword.assign(key);
            assignedcategory.assign(c);
            break;
        }
    }
    if(!assignedword.empty()){
        setcurrentword(assignedword,assignedcategory,diff);
        sethistory(currentword,category,diff);
        return;
    }
    
    
    uint8_t r = 0;
    FILE* f = fopen("/dev/urandom","rb");
    if(f){
        fread(&r,sizeof(uint8_t),1,f);
        fclose(f);
    }
    switch(r){
        case 0://champ
            getnewchampword(diff);
            break;
        case 1://ability
            getnewabilityword(diff);
            break;
    }
    
}
void Word::getnewchampword(time_t day){
    //how do I pick a random key from the json file?
    
    //because nlohmann json doesn't support range based iterators I have to read each key into a vector
    //then deal with the randomness from the vector
    category.assign("champion");
    std::ifstream ifs("league-l-champions.json");
    assert(ifs.good());
    nlohmann::json jf = nlohmann::json::parse(ifs);
    for (auto& [key, val] : jf.items())
    {
        //std::cout << "key: " << key << ", value:" << val << '\n';
    }
    ifs.close();
}
void Word::getnewabilityword(time_t day){
    //how do I pick a random ability from each key's value?
    
    /*
     //category.assign("ability");
     std::ifstream ifs("leaguechamps.json");
     nlohmann::json jf = nlohmann::json::parse(ifs);
     for (auto& [key, val] : jf.items())
     {
     for(auto& [k,v]:val.items()){
     //strncpy(category,c,LEAGUE_WORD_LENGTH-1);
     //std::cout << "key: " << k << ", value:" << v << '\n';
     }
     }
     ifs.close();
     */
    
}
const std::string& Word::getword(){
    return currentword;
}

const std::string& Word::getcategory(){
    return category;
}
