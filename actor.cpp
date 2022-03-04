#include <actor.h>
Actor::Actor(uWS::WebSocket<true,true,PerSocketData>* connection){
    this->connection = connection;
    memset(uuid,0,UUID4_LEN);
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
