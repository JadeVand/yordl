#include "fileio.h"




class ServerInstance
{
private:
    std::set<std::shared_ptr<Actor>> players;
    uWS::Loop* loop;
public:
    ServerInstance(uWS::Loop* loop);

    void onremove(uWS::WebSocket<true,true,PerSocketData>* ws,int code, std::string_view message);

    void onmessage(uWS::WebSocket<true,true,PerSocketData>* ws, std::string_view, uWS::OpCode opCode);
    void onconnect(uWS::WebSocket<true,true,PerSocketData>* ws);

    
    void packethandler(std::shared_ptr<Actor> actor, uint32_t packettype, const std::string& buffer,nlohmann::json& packet);
    void sendpacket(std::shared_ptr<Actor> actor, const nlohmann::json& packet);
private:

};

ServerInstance::ServerInstance(uWS::Loop* loop){
    this->loop = loop;
}
void ServerInstance::sendpacket(std::shared_ptr<Actor> actor, const nlohmann::json& packet){
    actor->getconnection()->send(packet.dump(),uWS::OpCode::TEXT,true);
}
void ServerInstance::packethandler(std::shared_ptr<Actor> actor, uint32_t packettype, const std::string& buffer,nlohmann::json& packet)
{
    Identifiers pid = (Identifiers)packettype;
    switch (pid)
    {

        case Identifiers::kNothing:
            break;
        case Identifiers::kForward:{
            actor->goforward();

        }
            break;
        case Identifiers::kUpdateAngle:{
            float angle = 0;
            try
            {
                angle = packet.at("angle");
            }
            catch (nlohmann::json::exception&)
            {
                return;
            }

            actor->updateangle(angle);

        }
            break;
        case Identifiers::kStopForward:{
            actor->stopforward();

        }
            break;
        case Identifiers::kProjectile:{
            actor->incrementbullet();
        }
            break;
    default:
        //    Remove(Triggered);
        break;
    }

}

void ServerInstance::onconnect(uWS::WebSocket<true,true,PerSocketData>* ws)
{
    std::shared_ptr<Actor> actor = std::make_shared<Actor>(ws);
    if(actor){
        ws->getUserData()->data = actor.get();
        players.insert(actor);
    }
}

void ServerInstance::onremove(uWS::WebSocket<true,true,PerSocketData>* ws,int code, std::string_view message)
{
    std::shared_ptr<Actor> actor = ((Actor*)ws->getUserData()->data)->shared_from_this();
    players.erase(actor);
    //send remove packet to all players
}

void ServerInstance::onmessage(uWS::WebSocket<true,true,PerSocketData>* ws, std::string_view message, uWS::OpCode opCode)
{
    
    std::string payload(message);

    nlohmann::json packet;
    try
    {
        packet = nlohmann::json::parse(payload);
    }
    catch (const nlohmann::json::parse_error&)
    {
        return;
    }

    uint32_t packettype = 0;
    try
    {
        packettype = packet.at("pid");
    }
    catch (nlohmann::json::exception&)
    {
        return;
    }

    std::shared_ptr<Actor> actor = ((Actor*)ws->getUserData()->data)->shared_from_this();

    packethandler(actor, packettype, payload,packet);
     
}


int main()
{


    ServerInstance Instance(uWS::Loop::get());
    /* Keep in mind that uWS::SSLApp({options}) is the same as uWS::App() when compiled without SSL support.
     * You may swap to using uWS:App() if you don't need SSL */
    uWS::App().ws<PerSocketData>("/*", {
        /* Settings */
        .compression = uWS::SHARED_COMPRESSOR,
        .maxPayloadLength = 16 * 1024 * 1024,
        .idleTimeout = 16,
        .maxBackpressure = 1 * 1024 * 1024,
        .closeOnBackpressureLimit = false,
        .resetIdleTimeoutOnSend = false,
        .sendPingsAutomatically = true,
        /* Handlers */
        .upgrade = nullptr,
        .open = [&Instance](auto *ws/*ws*/) {
            /* Open event here, you may access ws->getUserData() which points to a PerSocketData struct */
            Instance.onconnect((uWS::WebSocket<true,true,PerSocketData>*)ws);
        },
        .message = [&Instance](auto *ws, std::string_view message, uWS::OpCode opCode) {
            Instance.onmessage((uWS::WebSocket<true,true,PerSocketData>*)ws, message, opCode);
            //ws->send(message, opCode, true);
        },
        .drain = [](auto */*ws*/) {
            /* Check ws->getBufferedAmount() here */
        },
        .ping = [](auto */*ws*/, std::string_view) {
            /* Not implemented yet */
        },
        .pong = [](auto */*ws*/, std::string_view) {
            /* Not implemented yet */
        },
        .close = [&Instance](auto *ws/*ws*/, int code/*code*/, std::string_view message/*message*/) {
            /* You may access ws->getUserData() here */
            Instance.onremove((uWS::WebSocket<true,true,PerSocketData>*)ws,code,message);

        }
    }).listen(8080, [](auto *listen_socket) {
        if (listen_socket) {
            std::cout << "Listening on port " << 8080 << std::endl;
        }
    }).run();

}
