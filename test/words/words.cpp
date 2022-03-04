
#include "words.h"

Word::Word(){
    memset(currentword,0,LEAGUE_WORD_LENGTH);
    memset(category,0,LEAGUE_WORD_LENGTH);
}
Word::~Word(){
    
}
void Word::getnewword(){
    //read json
    //pick a world
    std::ifstream ifs("leaguewords.json");
    nlohmann::json jf = nlohmann::json::parse(ifs);
    for (auto& [key, val] : jf.items())
    {
        //std::cout << "key: " << key << ", value:" << val << '\n';
        if(key.length() == 6){
            std::cout << key << std::endl;
        }
        for(auto& [k,v]:val.items()){
            //std::cout << "key: " << k << ", value:" << v << '\n';
        }
    }
}
char* Word::getword(){
    return currentword;
}

char* Word::getcategory(){
    return category;
}
