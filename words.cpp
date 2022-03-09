

#include "words.h"

Word::Word(URand* ur){
    this->ur = ur;
}
Word::~Word(){
    
}
void Word::mask(uint16_t* s, uintptr_t index){
    switch(index){
        case 0:
            *s^=1;
            break;
        case 1:
            *s^=2;
            break;
        case 2:
            *s^=4;
            break;
        case 3:
            *s^=8;
            break;
        case 4:
            *s^=0x10;
            break;
        case 5:
            *s^=0x20;
            break;
        case 6:
            *s^=0x40;
            break;
        case 7:
            *s^=0x80;
            break;
        case 8:
            *s^=0x100;
            break;
        case 9:
            *s^=0x200;
            break;
        case 10:
            *s^=0x400;
            break;
        case 11:
            *s^=0x800;
            break;
        case 12:
            *s^=0x1000;
            break;
        case 13:
            *s^=0x2000;
            break;
        case 14:
            *s^=0x4000;
            break;
        case 15:
            *s^=0x8000;
            break;
    }
}
void Word::checkword(uint32_t* result,const std::string& guess,const std::string& correct){
    if(guess.length()!=correct.length()){
        return;
    }
    size_t s = guess.length();
    uint16_t high = 0;
    uint16_t low = 0;
    for(uintptr_t index = 0; index < s; ++index){
        
        if(guess[index]==correct[index]){
            mask(&high,index);
            mask(&low,index);
        }else{
            if(correct.find(guess[index])!=std::string::npos){
                mask(&low,index);
            }
        }
    }
    *result|=high<<16;
    *result|=low;
}
uint8_t Word::getrowsforlength(uint64_t length){
    if(length < 3)
        return 0;
    if(length<5)
        return 3;
    if(length<8)
        return 6;
    if(length<11)
        return 7;
    if(length<17)
        return 8;
    return 0;
}
bool Word::isvalidword(std::string s){
    std::ifstream ifs("yordl-champions.json");
    nlohmann::json jf = nlohmann::json::parse(ifs);
    ifs.close();
    bool found = false;
    std::vector<std::string> vec;
    
    for (auto& [key, val] : jf.items())
    {
        if(!s.compare(key)){
            found = true;
            break;
        }
        for(auto& [k,v]:val.items()){
            if(!s.compare(v)){
                found = true;
                break;
            }
        }
    }
    return found;
}
#define SECONDS_IN_DAY 86400
bool Word::neednewword(time_t servertime,time_t now){
    
    time_t diff = now - servertime;
    diff/=SECONDS_IN_DAY;//this is our day, we check if this file currently exists, if not we make a new file with our new word
    
    std::ifstream ifs("yordl-currentword.json");
    assert(ifs.good());
    nlohmann::json jf = nlohmann::json::parse(ifs);
    ifs.close();
    time_t day = jf.at("day");
    if(diff==day){
        return false;
    }
    
    //read the history state
    
    FILE* f = fopen("yordl-currentstate","rb");
    assert(f);
    
    uint64_t attempt = 0;
    uint64_t success = 0;
    fread(&attempt,sizeof(uint64_t),1,f);
    fread(&success,sizeof(uint64_t),1,f);
    
    
    
    //read the json history file so we can edit the attempts and success
    std::ifstream ifhistory("yordl-history.json");
    assert(ifhistory.good());
    nlohmann::json jshistory = nlohmann::json::parse(ifhistory);
    ifhistory.close();
    
    //fix attempts and success
    jshistory[currentword]["attempts"] = attempt;
    jshistory[currentword]["successful"] = success;
    
    //rewrite history to file
    std::ofstream ofhistory;
    assert(ofhistory.good());
    ofhistory << jshistory;
    ofhistory.close();
    
    
    
    
    //destroy the state
    attempt = 0;
    success = 0;
    fseek(f,0,SEEK_SET);
    fwrite(&attempt,sizeof(uint64_t),1,f);
    fwrite(&success,sizeof(uint64_t),1,f);
    fclose(f);
    
    return true;
}
void Word::sethistory(std::string currentword,std::string category,time_t day){
    std::ifstream ifhistory("yordl-history.json");
    assert(ifhistory.good());
    nlohmann::json jshistory = nlohmann::json::parse(ifhistory);
    ifhistory.close();
    std::time_t t = std::time(nullptr);
    std::tm *const pTInfo = std::localtime(&t);
    union LeagueLTime ltl = {0};
    ltl.p.day = (uint8_t)pTInfo->tm_mday;
    ltl.p.month = (uint8_t)pTInfo->tm_mon;
    ltl.p.year = (uint16_t)(pTInfo->tm_year+1900);
    jshistory[currentword] = nlohmann::json::object();
    jshistory["category"] = category;
    jshistory[currentword]["day"] = day;
    jshistory[currentword]["date"] = ltl.t;
    jshistory[currentword]["attempts"] = 0;
    jshistory[currentword]["successful"] = 0;
    std::ofstream ofhistory("yordl-history.json");
    assert(ofhistory.good());
    ofhistory << jshistory;
    ofhistory.close();
}
void Word::setcurrentword(std::string currentword,std::string category, time_t day){
    this->currentword.assign(currentword);
    this->category.assign(category);
    std::ofstream ofs("yordl-currentword.json");
    
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
    std::ifstream ifs("yordl-assignedword.json");
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
void Word::gethistory(std::vector<std::string>& vechistory){
    std::ifstream ifshistory("yordl-history.json");
    assert(ifshistory.good());
    nlohmann::json jf = nlohmann::json::parse(ifshistory);
    ifshistory.close();
    
    for(auto& [key,val] : jf.items()){
        vechistory.push_back(key);
    }
}
void Word::getnewchampword(time_t day){
    //how do I pick a random key from the json file?
    
    //because nlohmann json doesn't support range based iterators I have to read each key into a vector
    //then deal with the randomness from the vector
    std::ifstream ifs("yordl-champions.json");
    
    assert(ifs.good());
    
    nlohmann::json jf = nlohmann::json::parse(ifs);
    
    ifs.close();
    
    std::vector<std::string> vec;
    for (auto& [key, val] : jf.items())
    {
        vec.push_back(key);
    }
    
    
    
    //erase already used(history) keys from the vector
    std::vector<std::string> vechistory;
    gethistory(vechistory);
    //iterate the history find the matching and remove them
    for(auto& k : vechistory){
        vec.erase(std::remove(vec.begin(), vec.end(), k), vec.end());
    }
    uint32_t r = ur->getu16rand();
    r%=vec.size();
    std::string assignedword = vec.at(r);
    std::string assignedcategory = "champion";
    
    setcurrentword(assignedword,assignedcategory,day);
    sethistory(assignedword,assignedcategory,day);
}
void Word::getnewabilityword(time_t day){
    //how do I pick a random ability from each key's value?
    
    
    std::ifstream ifs("yordl-champions.json");
    nlohmann::json jf = nlohmann::json::parse(ifs);
    ifs.close();
    std::vector<std::string> vec;
    
    for (auto& [key, val] : jf.items())
    {
        for(auto& [k,v]:val.items()){
            vec.push_back(v);
        }
    }
    
    
    //erase already used(history) keys from the vector
    std::vector<std::string> vechistory;
    gethistory(vechistory);
    //iterate the history find the matching and remove them
    for(auto& k : vechistory){
        vec.erase(std::remove(vec.begin(), vec.end(), k), vec.end());
    }
    uint32_t r = ur->getu16rand();
    r%=vec.size();
    
    std::string assignedword = vec.at(r);
    std::string assignedcategory = "ability";
    
    setcurrentword(assignedword,assignedcategory,day);
    sethistory(assignedword,assignedcategory,day);
}
const std::string& Word::getword(){
    return currentword;
}

const std::string& Word::getcategory(){
    return category;
}
void Word::incrementstateattempt(){
    FILE* f = fopen("yordl-currentstate","rb+");
    assert(f);
    
    uint64_t attempt = 0;
    
    fread(&attempt,sizeof(uint64_t),1,f);
    attempt++;
    fseek(f,0,SEEK_SET);
    fwrite(&attempt,sizeof(uint64_t),1,f);
    fclose(f);
}
void Word::incrementstateattemptandsuccess(){
    FILE* f = fopen("yordl-currentstate","rb+");
    assert(f);

    uint64_t attempt = 0;
    uint64_t success = 0;
    
    fread(&attempt,sizeof(uint64_t),1,f);
    fread(&success,sizeof(uint64_t),1,f);
    
    attempt++;
    success++;
    
    fseek(f,0,SEEK_SET);
    fwrite(&attempt,sizeof(uint64_t),1,f);
    fwrite(&success,sizeof(uint64_t),1,f);
    fclose(f);
}
