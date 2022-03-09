#include <actor.h>
Actor::Actor(uWS::WebSocket<true,true,PerSocketData>* connection,URand* ur){
    this->connection = connection;
    this->ur = ur;
    memset(uuid,0,UID128LENGTH);
    f = nullptr;
    memset(&header,0,sizeof(struct PlayerHeader));
}
uint64_t Actor::getid()
{
    return uint64_t(this);
}
uWS::WebSocket<true,true,PerSocketData>* Actor::getconnection(){
    return connection;
}
void Actor::setuuid(char* uuid){
    memcpy(this->uuid,uuid,UID128LENGTH);
}
void Actor::openfile(){
    char filename[64] = {0};
    strcat(filename,"yordles/");
    strcat(filename,uuid);
    f = fopen(filename,"r+");
    
}
void Actor::createfile(){
    char filename[64] = {0};
    strcat(filename,"yordles/");
    strcat(filename,uuid);
    f = fopen(filename,"wb");
}
bool Actor::fileexists(){
    if(f){
        return true;
    }
    return false;
}
void Actor::readheader(){
    if(f){
        setfileoffsetfrombeg(0);
        fread(&header,sizeof(struct PlayerHeader),1,f);
    }
}
struct PlayerHeader* Actor::getheader(){
    return &header;
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
void Actor::writeheader(){
    if(f){
        setfileoffsetfrombeg(0);
        fwrite(&header,sizeof(struct PlayerHeader),1,f);
    }
}
void Actor::makeuuid(){
    union Uid128u u;
    ur->getu128rand(&u);
    memcpy(&header.uuid,&u.u,sizeof(struct Uid128));
    ur->getu128string(&u,uuid);
}
char* Actor::getuuid(){
    return uuid;
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
