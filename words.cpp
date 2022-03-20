

#include "words.h"

Word::Word(URand* ur){
    this->ur = ur;
    std::ifstream ifs("yordl-currentword.json");
    if(ifs.good()){
        try{
            nlohmann::json j = nlohmann::json::parse(ifs);
            currentword.assign(j.at("word"));
            category.assign(j.at("category"));
        }catch(nlohmann::json::exception& e){
            assert(0);
        }
        ifs.close();
    }
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

WordValidation Word::checkword(uint32_t* result,const std::string& guess,const std::string& correct){
    
    if(guess.length()!=correct.length()){
        return WordValidation::kWordBadLength;
    }
    if(!Word::isvalidlength(guess)){
        return WordValidation::kWordBadLength;
    }
    //check if word exists
    if(!Word::indictionary(guess)){
        //  return WordValidation::kWordNonExist
        return WordValidation::kWordNonExist;
    }
    
    std::string temp = correct;
    size_t s = guess.length();
    uint16_t high = 0;
    uint16_t low = 0;
    std::string res(s,'0');
    
    for(size_t index = 0; index < s; ++index){
        for(size_t inner = 0; inner < s; ++inner){
            if(guess[index]==temp[inner]){
                res.insert(index,sizeof(char),guess[index]);
                temp[inner] = 0;
                
                break;
            }
        }
    }

    for(size_t index = 0; index < res.length(); ++index){
        if(res[index]==correct[index]){
            mask(&high,index);
            //mask(&low,index);
        }else if(res[index]=='0'){
            
        }else{
            mask(&low,index);
        }
    }
    
    *result|=high<<16;
    *result|=low;
    
    return WordValidation::kWordOk;
}
uint8_t Word::getrowsforlength(size_t length){
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
bool Word::indictionary(std::string s){
    std::ifstream ifs("yordl-champions.json");
    nlohmann::json jf = nlohmann::json::parse(ifs);
    ifs.close();
    bool found = false;
    std::vector<std::string> vec;
    
    for (auto& [key, val] : jf.items())
    {
        std::string kval = key;
        std::transform(kval.begin(), kval.end(),kval.begin(), ::toupper);
        removeapos(kval);
        if(!s.compare(kval)){
            
            found = true;
            break;
        }
        for(auto& [k,v]:val.items()){
            std::string vval = v;
            std::transform(vval.begin(), vval.end(),vval.begin(), ::toupper);
            if(!s.compare(vval)){
                found = true;
                break;
            }
        }
    }
    return found;
}
#define SECONDS_IN_DAY 86400
time_t Word::getday(time_t servertime,time_t now){
    time_t diff = now - servertime;
    diff/=SECONDS_IN_DAY;//this is our day, we check if this file currently exists, if not we make a new file with our new word
    return diff;
}

bool Word::neednewword(time_t servertime,time_t now){
    
    
    time_t diff = getday(servertime,now);
    std::ifstream ifs("yordl-currentword.json");
    if(ifs.good()){
        nlohmann::json jf = nlohmann::json::parse(ifs);
        ifs.close();
        time_t day = 0;
        try{
             day = jf.at("day");
        }catch(nlohmann::json::exception& e){
            assert(0);
        }
        
        if(diff==day){
            return false;
        }
    }else{
        return true;
        
    }
    
    
    //read the history state
    
    FILE* f = fopen("yordl-currentstate","r+");
    uint64_t attempt = 0;
    uint64_t success = 0;
    if(!f){
        f = fopen("yordl-currentstate","wb");
        assert(f);
    }else{
        fread(&attempt,sizeof(uint64_t),1,f);
        fread(&success,sizeof(uint64_t),1,f);
    }
    
    
    //read the json history file so we can edit the attempts and success
    std::ifstream ifhistory("yordl-history.json");
    assert(ifhistory.good());
    nlohmann::json jshistory = nlohmann::json::parse(ifhistory);
    ifhistory.close();
    
    //fix attempts and success
    jshistory[currentword]["attempts"] = attempt;
    jshistory[currentword]["successful"] = success;
    
    //rewrite history to file
    std::ofstream ofhistory("yordl-history.json");
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
void Word::appendhistory(time_t day){
    std::ifstream ifhistory("yordl-history.json");
    nlohmann::json jshistory;
    if(ifhistory.good()){
        jshistory = nlohmann::json::parse(ifhistory);
        ifhistory.close();
    }
    
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

    //https://stackoverflow.com/a/735215/8350647
    std::transform(currentword.begin(), currentword.end(),currentword.begin(), ::toupper);
    //
    
    this->currentword.assign(currentword);
    this->category.assign(category);
    std::ofstream ofs("yordl-currentword.json");
    
    assert(ofs.good());
    
    nlohmann::json j ;
    //j[currentword] = nlohmann::json::object();
    j["word"] = currentword;
    j["category"] = category;
    j["day"] = day;
    ofs << j;
    ofs.close();
}
bool Word::isvalidlength(const std::string& str){
    return str.length()<LEAGUE_WORD_LENGTH?true:false;
}
//https://stackoverflow.com/a/20326507/8350647
void Word::removeapos(std::string& str){
    str.erase(std::remove(str.begin(), str.end(), '\''), str.end());
}
void Word::getnewword(time_t servertime,time_t now){
    
    
    std::string assignedword;
    std::string assignedcategory;
    time_t diff = now - servertime;
    diff/=SECONDS_IN_DAY;//this is our day, we check if this file currently exists, if not we make a new file with our new word
    std::ifstream ifs("yordl-assignedword.json");
    if(ifs.good()){
        nlohmann::json jf = nlohmann::json::parse(ifs);
        ifs.close();
        for (auto& [key, val] : jf.items())
        {
            time_t day = 0;
            try{
                day = val.at("day");
            }catch(nlohmann::json::exception& e){
                assert(0);
            }
            
            
            if(day==diff){
                std::string c;
                try{
                    c = val.at("category");
                }
                catch(nlohmann::json::exception& e){
                    assert(0);
                }
                assert(!c.empty());
                
                assignedword.assign(key);
                assignedcategory.assign(c);
                break;
            }
        }
    }
    
    if(!assignedword.empty()){
        setcurrentword(assignedword,assignedcategory,diff);
        appendhistory(diff);
        return;
    }
    
    uint16_t r = ur->getu16rand();
    r%=1;//temporarely only reading champion names
    //abilities havent been established yet
    switch(r){
        case 0://champ
            getnewchampword(diff);
            break;
        case 1://ability
            getnewchampword(diff);
            break;
        case 2:
            //items not yet established
            break;
    }
    
}
void Word::gethistory(std::vector<std::string>& vechistory){
    std::ifstream ifshistory("yordl-history.json");
    if(ifshistory.good()){
        nlohmann::json jf = nlohmann::json::parse(ifshistory);
        ifshistory.close();
        
        for(auto& [key,val] : jf.items()){
            vechistory.push_back(key);
        }
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
    uint16_t r = ur->getu16rand();
    r%=vec.size();
    bool found = false;
    std::string assignedword;
    do{
        assignedword = vec.at(r);
        removeapos(assignedword);
        found = isvalidlength(assignedword);
    }while(!found);
    
    std::string assignedcategory = "champion";
    
    setcurrentword(assignedword,assignedcategory,day);
    appendhistory(day);
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
    uint16_t r = ur->getu16rand();
    bool found = false;
    std::string assignedword;
    do{
        assignedword = vec.at(r);
        removeapos(assignedword);
        found = isvalidlength(assignedword);
    }while(!found);
    
    std::string assignedcategory = "ability";
    
    setcurrentword(assignedword,assignedcategory,day);
    appendhistory(day);
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

uint8_t Word::getrowcount(){
    uint8_t l = Word::getrowsforlength(currentword.length());
    return l;
}
size_t Word::getwordlength(){
    return currentword.length();
}
