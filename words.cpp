
#include <words.h>

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
    for (auto& el : jf.items())
    {
        std::cout << "key: " << el.key() << ", value:" << el.value() << '\n';
    }
}
char* Word::getword(){
    return currentword;
}

char* Word::getcategory(){
    return category;
}
