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
    Word wordle;
    URand ur;
    std::set<std::shared_ptr<Actor>> players;
public:
    ServerInstance();
    
    void onremove(uWS::WebSocket<true,true,PerSocketData>* ws,int code, std::string_view message);
    
    void onmessage(uWS::WebSocket<true,true,PerSocketData>* ws, std::string_view, uWS::OpCode opCode);
    void onconnect(uWS::WebSocket<true,true,PerSocketData>* ws);
    
    
    void packethandler(std::shared_ptr<Actor> actor, uint32_t packettype, const std::string& buffer,nlohmann::json& packet);
    void sendpacket(std::shared_ptr<Actor> actor, const nlohmann::json& packet);
private:
    
};

ServerInstance::ServerInstance():wordle(&ur){

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
                selfid = uuidpacket.at("uidprivate");
            }
            catch (nlohmann::json::exception &e)
            {
                return;
            }
            if(!selfid.compare("null")){
                actor->makeuuid();
                nlohmann::json senduuidpacket;
                senduuidpacket["pid"] = Identifiers::kMyId;
                senduuidpacket["uidpublic"] = actor->getdecryptuid();
                senduuidpacket["uidprivate"] = actor->getencryptuid();

                actor->getconnection()->send(senduuidpacket.dump(),uWS::OpCode::TEXT,true);
            }else{
                actor->stringtouid(selfid);
                PlayerHeader header = {0};
                actor->readheader(&header);
                time_t day = wordle.getday(servertime,time_since_epoch());
                if(day!=header.dayofprogress){
                    memset(header.progress,0,sizeof(header.progress));
                    actor->writeheader(&header);
                }else{
                    
                    nlohmann::json progress;
                    progress["pid"] = Identifiers::kProgress;
                    progress["progress"] = nlohmann::json::array();
                    
                    uint8_t rowcount =wordle.getrowcount();
                    typedef char Word2D[rowcount][wordle.getwordlength()] ;
                    Word2D* matrix = (Word2D*)header.progress;
                    for(uint8_t row = 0; row < rowcount;++row){
                        if(*matrix[row][0]==0){
                            actor->setindex(row);
                            break;
                        }else{
                            std::string word;
                            word.assign(*matrix[row]);
                            progress["progress"].push_back(word);
                            
                        }
                    }
                    actor->getconnection()->send(progress.dump(),uWS::OpCode::TEXT,true);
                    /*sending progress but how do I tell the user which letters are correct and which arent? Should I just store it in the users local cache since it's not critical info? and if manipulated cant do any harm
                     */
                }
            }
        }
            break;
        case Identifiers::kGuess:{
            std::string row ;
            try
            {
                row = packet.at("guess");
            }
            catch (nlohmann::json::exception&)
            {
                return;
            }
            
            nlohmann::json out ;
            out["pid"] = Identifiers::kGuess;
            uint32_t result = 0;
            std::string currentword = wordle.getword();
            WordValidation valid = Word::checkword(&result,row,currentword);
            if(valid==WordValidation::kWordBadLength){
                out["valid"] = false;
            }else if(valid == WordValidation::kWordNonExist){
                out["valid"] = false;
            }else if(valid == WordValidation::kWordOk){
                out["valid"] = true;
                out["result"] = result;
            }
            actor->getconnection()->send(out.dump(),uWS::OpCode::TEXT,true);
            
            //update the actor's stats
        }
            break;
        case Identifiers::kStatsForUuid:{
            std::string uid;
            try{
                uid = packet.at("uidpublic");
            }catch(nlohmann::json::exception& e){
                return;
            }
            std::string path ="yordles/";
            path.append(uid);
            FILE* f = fopen(path.c_str(),"rb");
            if(!f){
                return;
            }
            struct PlayerHeader header = {0};
            fread(&header,sizeof(struct PlayerHeader),1,f);
            fclose(f);
            nlohmann::json stats;
            stats["score"] = header.score;
            stats["current"] = header.currenstreak;
            stats["max"] = header.maxstreak;
            actor->getconnection()->send(stats.dump().c_str(),uWS::OpCode::TEXT,true);
            
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
    
    if(wordle.neednewword(servertime,now)){
        wordle.getnewword(servertime,now);
    }
    std::shared_ptr<Actor> actor = std::make_shared<Actor>(ws,&ur);
    if(actor){
        
        ws->getUserData()->data = actor.get();
        players.insert(actor);
        
        std::string word = wordle.getword();
        size_t length = word.length();
        uint8_t rows = Word::getrowsforlength(length);
        nlohmann::json packet ;
        packet["pid"] = Identifiers::kWord;
        packet["length"] = length;
        packet["rows"] = rows;
        actor->getconnection()->send(packet.dump(),uWS::OpCode::TEXT,true);
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
    
    //uuid4_init();
    FILE* timestamp = fopen("yordl-timestamp","rb");
    if(timestamp){
        
        fread(&servertime,sizeof(servertime),1,timestamp);
        fclose(timestamp);
    }else{
        timestamp = fopen("yordl-timestamp","wb");
        if(timestamp){
            servertime = time_since_epoch();
            fwrite(&servertime,sizeof(servertime),1,timestamp);
            fclose(timestamp);
        }
    }
    FILE* currentstate = fopen("yordl-currentstate","rb");
    if(!currentstate){
        currentstate = fopen("yordl-currentstate","wb");
        uint64_t attempt = 0;
        uint64_t success = 0;
        fwrite(&attempt,sizeof(uint64_t),0,currentstate);
        fwrite(&success,sizeof(uint64_t),0,currentstate);
        fclose(currentstate);
    }else{
        fclose(currentstate);
    }
    ServerInstance Instance;
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
