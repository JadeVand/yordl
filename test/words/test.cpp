
#include <urand.h>
#include <iostream>
#include <string.h>
char correct[] = "hello";
class Box{
public:
    char c;
    Box(){
        c = 0;
    }
};

#define MAX_BOXES 96 //12x8 max
class Progress{
private:
    Box* b;
    uint8_t x;
    uint8_t y;
    uint8_t index = 0;
public:
    Progress(){
        b = new Box[MAX_BOXES];
        memset(b,0,sizeof(Box)*MAX_BOXES);
        x = 5;
        y = 20;
        
    }
    void push(char* w){
        typedef Box Box2d[x][y];
        Box2d* b2 = (Box2d*)b;
        if(index<5){
            for(uint8_t i = 0; i < x;++i){
                (*b2)[index][i].c = w[i];
            }
            index++;
        }
        
    }
    Box* getb(){
        return b;
    }
    ~Progress(){
        delete[] b;
    }
};
int main(){

    char input[32] = {0};
    fgets(input,sizeof(input),stdin);
    Progress p;
    p.push(input);
    fgets(input,sizeof(input),stdin);
    p.push(input);
    fgets(input,sizeof(input),stdin);
    p.push(input);
    fgets(input,sizeof(input),stdin);
    p.push(input);
    fgets(input,sizeof(input),stdin);
    p.push(input);
    fgets(input,sizeof(input),stdin);
    p.push(input);
    fgets(input,sizeof(input),stdin);
    p.push(input);
    fgets(input,sizeof(input),stdin);
    p.push(input);
    Box* b = p.getb();
    typedef Box Box2d[5][20];
    Box2d* b2 = (Box2d*)b;
    for(int i = 0 ; i < 20; ++i){
        printf("%s\n",(*b2)[i]);
    }
    return 0;
}
