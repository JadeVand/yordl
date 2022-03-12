#include <actor.h>
Actor::Actor(uWS::WebSocket<true,true,PerSocketData>* connection,URand* ur){
    this->connection = connection;
    this->ur = ur;
    memset(&uid,0,sizeof(union Uid128u));
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
    assignuid(&u);
}
void Actor::stringtouid(const std::string& str){
    union Uid128u u = {0};
    decryptuidstring(&u,str);
    assignuid(&u);
}
void Actor::assignuid(union Uid128u* u){
    memcpy(&uid,u,sizeof(union Uid128u));
}
std::string Actor::getencryptuid(){
    std::string res;
    char c[UID128LENGTH] = {0};
    union Uid128u encryptuid = {0};//encrypt the uid then get the string
    memcpy(&encryptuid,&uid,sizeof(union Uid128u));
    ur->encryptu128(&encryptuid);
    URand::getu128string(&encryptuid,c);
    res.assign(c);
    return res;
}
std::string Actor::getdecryptuid(){
    std::string res;
    char c[UID128LENGTH] = {0};
    URand::getu128string(&uid,c);
    res.assign(c);
    return res;
}
void Actor::decryptuidstring(union Uid128u* u, const std::string& str){
    if(str.length()!=UID128LENGTH-1){
        return;
    }
    URand::makeu128bytes(u,str.c_str());
    ur->decryptu128(u);
   // URand::getu128string(&u,uuid);
}
