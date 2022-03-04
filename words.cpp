

#include "words.h"

Word::Word(){
    memset(currentword,0,LEAGUE_WORD_LENGTH);
    memset(category,0,LEAGUE_WORD_LENGTH);
}
Word::~Word(){
    
}
bool Word::neednewword(time_t servertime,time_t now){
    time_t diff = now - servertime;
    diff/=86400;//this is our day, we check if this file currently exists, if not we make a new file with our new word
    
    std::ifstream ifs("currentword.json");
    if(ifs.good()){
        nlohmann::json jf = nlohmann::json::parse(ifs);
        time_t day = jf.at("day");
        if(diff==day){
            return false;
        }
    }
    memset(currentword,0,LEAGUE_WORD_LENGTH);
    memset(category,0,LEAGUE_WORD_LENGTH);
    return true;
}
void Word::getnewword(time_t servertime,time_t now){
    time_t diff = now - servertime;
    diff/=86400;//this is our day, we check if this file currently exists, if not we make a new file with our new word
    std::ifstream ifs("assignedword.json");
    nlohmann::json jf = nlohmann::json::parse(ifs);
    for (auto& [key, val] : jf.items())
    {
        time_t day = val.at("Day");
        if(day==diff){
            //manually set word
            std::string c = val.at("Category");
            
            strncpy(category,c.c_str(),LEAGUE_WORD_LENGTH-1);
            strncpy(currentword,key.c_str(),LEAGUE_WORD_LENGTH-1);
            //save the word in history
            //set current word in json
            return;
        }
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
    char c[] = "champion";
    strncpy(category,c,LEAGUE_WORD_LENGTH-1);
    std::ifstream ifs("leaguechamps.json");
    nlohmann::json jf = nlohmann::json::parse(ifs);
    for (auto& [key, val] : jf.items())
    {
        //std::cout << "key: " << key << ", value:" << val << '\n';
    }
    ifs.close();
}
void Word::getnewabilityword(time_t day){
    char c[] = "ability";
    std::ifstream ifs("leaguechamps.json");
    nlohmann::json jf = nlohmann::json::parse(ifs);
    for (auto& [key, val] : jf.items())
    {
        for(auto& [k,v]:val.items()){
            strncpy(category,c,LEAGUE_WORD_LENGTH-1);
            //std::cout << "key: " << k << ", value:" << v << '\n';
        }
    }
    ifs.close();
}
char* Word::getword(){
    return currentword;
}

char* Word::getcategory(){
    return category;
}
