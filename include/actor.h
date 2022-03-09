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
    struct Uid128 uuid;
    uint64_t score;
    char progress[128];
};

class Actor : public std::enable_shared_from_this<Actor>{
private:
    uWS::WebSocket<true,true,PerSocketData>* connection;
    char uuid[UID128LENGTH];

    struct PlayerHeader header;
    FILE* f;
    URand* ur;
    uint8_t index;
public:
    Actor(uWS::WebSocket<true,true,PerSocketData>* connection,URand* ur);
    uint64_t getid();
    uWS::WebSocket<true,true,PerSocketData>* getconnection();
    void openfile();
    void readheader();
    struct PlayerHeader* getheader();
    void setfileoffsetfrombeg(int32_t offset);
    void setfileoffsetfromend(int32_t offset);
    void setfileoffsetfromcur(int32_t offset);
    void createfile();
    bool fileexists();
    void writeheader();
    void makeuuid();
    void setuuid(char* uuid);
    char* getuuid();
    ~Actor();
};
#endif
