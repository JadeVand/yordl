#ifndef _actor_h
#define _actor_h
#include <words.h>
struct PerSocketData {
    void* data;
};

enum class Identifiers : uint32_t
{
    kNothing = 0,
    kMyId,
    kGuess,
    kStatsForUuid,
};
struct PlayerHeader{
    struct Uid128 uid;
    uint32_t score;
    uint16_t currenstreak;
    uint16_t maxstreak;
    char progress[128];
};

class Actor : public std::enable_shared_from_this<Actor>{
private:
    uWS::WebSocket<true,true,PerSocketData>* connection;
    URand* ur;
    uint8_t index;
    union Uid128u decryptuid ;
    union Uid128u encryptuid ;
public:
    Actor(uWS::WebSocket<true,true,PerSocketData>* connection,URand* ur);
    uint64_t getid();
    uWS::WebSocket<true,true,PerSocketData>* getconnection();
    void readheader(struct PlayerHeader* header);
    void writeheader(struct PlayerHeader* header);
    void makeuuid();
    void decryptuidstring(const std::string& str);
    ~Actor();
};
#endif
