
#include <urand.h>
#include <actor.h>
#include <iostream>
#include <string.h>

void mask(uint16_t* s, uint8_t index){
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
void checkword(uint32_t* result,const std::string& guess,const std::string& correct){
    if(guess.length()!=correct.length()){
        return;
    }
    size_t s = guess.length();
    uint16_t high = 0;
    uint16_t low = 0;
    for(size_t index = 0; index < s; ++index){
        
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
void checkresult(uint32_t result){
    uint16_t low = result&0xFFFF;
    uint16_t high = result>>16&0xFFFF;
    printf("%d %d\n",high,low);
    for(uint8_t index = 0; index < 16; ++index ){
        if((high>>index&1)==1){
            printf("correct\n");
        }else if((low>>index&1)==1){
            printf("acceptable\n");
        }else{
            printf("bad\n");
        }
    }
    //result should be
    //bad
    //acceptable
    //correct
    //acceptable
    //correct
}
time_t time_since_epoch()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t( now );
}
int main(){
    time_t timestamp = time_since_epoch();
    FILE* f = fopen("timestamp","rb");
    if(f){
        printf("reading timestamp\n");
        fread(&timestamp,sizeof(time_t),1,f);
        fclose(f);
    }else{
        f = fopen("timestamp","wb");
        assert(f);
        fwrite(&timestamp,sizeof(time_t),1,f);
        fclose(f);
        printf("creating timestamp");
    }
    URand ur ;
    Word wordle(&ur);
    if(wordle.neednewword(timestamp,time_since_epoch()+(86400*3))){
        std::cout << "we need new word" << std::endl;
        wordle.getnewword(timestamp,time_since_epoch()+(86400*3));
    }
    return 0;
}
