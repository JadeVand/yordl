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
