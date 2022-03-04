
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
}
char* Word::getword(){
    return currentword;
}

char* Word::getcategory(){
    return category;
}
