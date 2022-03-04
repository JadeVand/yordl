#include <fileio.h>
Actor::Actor(uWS::WebSocket<true,true,PerSocketData>* connection){
    this->connection = connection;

}
uint64_t Actor::getid()
{
    return uint64_t(this);
}
uWS::WebSocket<true,true,PerSocketData>* Actor::getconnection(){
    return connection;
}
