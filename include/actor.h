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
    kWord,
};
struct PlayerHeader{
    struct Uid128 uid;
    uint16_t score;
    uint16_t currenstreak;
    uint16_t maxstreak;
    uint16_t dayofprogress;
    char progress[128];
};

class Actor : public std::enable_shared_from_this<Actor>{
private:
    uWS::WebSocket<true,true,PerSocketData>* connection;
    URand* ur;
    uint8_t index;
    union Uid128u uid ;
public:
    Actor(uWS::WebSocket<true,true,PerSocketData>* connection,URand* ur);
    uint64_t getid();
    uWS::WebSocket<true,true,PerSocketData>* getconnection();
    void readheader(struct PlayerHeader* header);
    void writeheader(struct PlayerHeader* header);
    void makeuuid();
    void stringtouid(const std::string& uid);
    void assignuid(union Uid128u* u);
    void decryptuidstring(union Uid128u* u, const std::string& str);
    std::string getencryptuid();
    std::string getdecryptuid();

    ~Actor();
};
#endif
