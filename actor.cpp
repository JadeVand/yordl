#include <actor.h>
Actor::Actor(uWS::WebSocket<true,true,PerSocketData>* connection,URand* ur){
    this->connection = connection;
    this->ur = ur;
    memset(&decryptuid,0,sizeof(union Uid128u));
    memset(&encryptuid,0,sizeof(union Uid128u));
}
Actor::~Actor(){

}
uint64_t Actor::getid()
{
    return uint64_t(this);
}
uWS::WebSocket<true,true,PerSocketData>* Actor::getconnection(){
    return connection;
}

void Actor::readheader(struct PlayerHeader* header){

}


void Actor::writeheader(struct PlayerHeader* header){

}
void Actor::makeuuid(){
    union Uid128u u;
    ur->getu128rand(&u);
   // URand::getu128string(&u,uuid);
}

void Actor::decryptuidstring(const std::string& str){
    if(str.length()!=UID128LENGTH-1){
        return;
    }
    union Uid128u u = {0};
    URand::makeu128bytes(&u,str.c_str());
    ur->decryptu128(&u);
   // URand::getu128string(&u,uuid);
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
