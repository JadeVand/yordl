

#include "words.h"

Word::Word(){
}
Word::~Word(){
    
}
bool Word::neednewword(time_t servertime,time_t now){
    
     time_t diff = now - servertime;
     diff/=86400;//this is our day, we check if this file currently exists, if not we make a new file with our new word
     
     std::ifstream ifs("currentword.json");
     if(ifs.good()){
         nlohmann::json jf = nlohmann::json::parse(ifs);
         ifs.close();
         time_t day = jf.at("day");
         if(diff==day){
             return false;
         }
     }
    
    return true;
}
void Word::getnewword(time_t servertime,time_t now){
    
    time_t diff = now - servertime;
    diff/=86400;//this is our day, we check if this file currently exists, if not we make a new file with our new word
    std::ifstream ifs("assignedword.json");
    if(ifs.good()){
        nlohmann::json jf = nlohmann::json::parse(ifs);
        ifs.close();
        for (auto& [key, val] : jf.items())
        {
            time_t day = val.at("day");
            if(day==diff){
                std::string c = val.at("Category");

                currentword.assign(key);
                category.assign(c);
                
                //create current word file
                //append to history
                
                return;
            }
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
    /*
    char c[] = "champion";
    strncpy(category,c,LEAGUE_WORD_LENGTH-1);
    std::ifstream ifs("leaguechamps.json");
    nlohmann::json jf = nlohmann::json::parse(ifs);
    for (auto& [key, val] : jf.items())
    {
        //std::cout << "key: " << key << ", value:" << val << '\n';
    }
    ifs.close();
     */
}
void Word::getnewabilityword(time_t day){
    /*
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
     */
}
const std::string& Word::getword(){
    return currentword;
}

const std::string& Word::getcategory(){
    return category;
}
