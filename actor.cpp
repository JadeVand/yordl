#include <actor.h>
Actor::Actor(uWS::WebSocket<true,true,PerSocketData>* connection){
    this->connection = connection;
    memset(uuid,0,UUID4_LEN);
    f = nullptr;
}
uint64_t Actor::getid()
{
    return uint64_t(this);
}
uWS::WebSocket<true,true,PerSocketData>* Actor::getconnection(){
    return connection;
}
void Actor::setuuid(char* uuid){
    memcpy(this->uuid,uuid,UUID4_LEN);
}
void Actor::openfile(){
    f = fopen(uuid,"rb");
}
void Actor::readheader(struct PlayerHeader* header){
    if(f){
        setfileoffsetfrombeg(0);
        fread(header,sizeof(struct PlayerHeader),1,f);
    }
}
void Actor::setfileoffsetfrombeg(int32_t offset){
    if(f){
        fseek(f,offset,SEEK_SET);
    }
}
void Actor::setfileoffsetfromend(int32_t offset){
    if(f){
        fseek(f,offset,SEEK_END);
    }
}
void Actor::Actor::setfileoffsetfromcur(int32_t offset){
    if(f){
        fseek(f,offset,SEEK_CUR);
    }
}
Actor::~Actor(){
    if(f){
        fclose(f);
    }
}

/*
 char b[80] = {0};
 for(char c = 10; c < 90; ++c){
     b[c-10] = c;
 }
 typedef char arr2d[10][8] ;
 arr2d* b2d = (arr2d*)b;
 printf("%d\n",(*b2d)[1][0]);
 */
