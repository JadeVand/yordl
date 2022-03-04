#include <actor.h>

time_t servertime = 0;
time_t time_since_epoch()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t( now );
}
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
        case Identifiers::kMyId:{
            std::string selfid;
            
            nlohmann::json uuidpacket = nlohmann::json::parse(buffer);
            try
            {
                selfid = uuidpacket.at("myuuid");
            }
            catch (nlohmann::json::exception &e)
            {
                return;
            }
            if(!selfid.compare("null")){
                char uuidforplayer[UUID4_LEN] = {0};
                uuid4_generate(uuidforplayer);
                actor->setuuid(uuidforplayer);
                nlohmann::json senduuidpacket;
                senduuidpacket["pid"] = Identifiers::kMyId;
                senduuidpacket["selfid"] = uuidforplayer;
                actor->getconnection()->send(senduuidpacket.dump(),uWS::OpCode::TEXT,true);
            }else{
                char selfuuid[UUID4_LEN] = {0};
                memcpy(selfuuid, selfid.c_str(), UUID4_LEN);
                actor->setuuid(selfuuid);
            }
        }
            break;
        case Identifiers::kGuess:{
            std::string letter ;
            try
            {
                letter = packet.at("char");
            }
            catch (nlohmann::json::exception&)
            {
                return;
            }
            
        }
            break;
        default:
            //    Remove(Triggered);
            break;
    }
    
}

void ServerInstance::onconnect(uWS::WebSocket<true,true,PerSocketData>* ws)
{
    time_t now = time_since_epoch();
    time_t diff = now - servertime;
    diff/=86400;//this is our day, we check if this file currently exists, if not we make a new file with our new word
    
    
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
    
    FILE* timestamp = fopen("leagueltimestamp/timestamp","rb");
    if(timestamp){
        
        fread(&servertime,sizeof(servertime),1,timestamp);
        fclose(timestamp);
    }else{
        timestamp = fopen("leagueltimestamp/timestamp","wb");
        if(timestamp){
            servertime = time_since_epoch();
            fwrite(&servertime,sizeof(servertime),1,timestamp);
            fclose(timestamp);
        }
    }
    uuid4_init();
    char s[37] = {0};
    uuid4_generate(s);
    printf("%s\n",s);
    ServerInstance Instance(uWS::Loop::get());
    /* Keep in mind that uWS::SSLApp({options}) is the same as uWS::App() when compiled without SSL support.
     * You may swap to using uWS:App() if you don't need SSL */
    uWS::SSLApp({
        /* There are example certificates in uWebSockets.js repo */
        .key_file_name = "key.pem",
        .cert_file_name = "cert.pem",
    }).ws<PerSocketData>("/*", {
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
