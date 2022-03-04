#ifndef _actor_h
#define _actor_h
#include <App.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <string>
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <future>
#include <set>
#include <deque>
#include <queue>
#include <vector>
#include <random>
#include <sstream>
#include <list>
#include <algorithm>
#include <uuid4.h>
struct PerSocketData {
    void* data;
};

enum class Identifiers : uint32_t
{
    kNothing = 0,
    kMyId = 1,
    kGuess
};
struct PlayerHeader{
    struct Uuidv4 uuid;
    uint64_t score;
}
struct LeagueLDate{
    uint8_t day;
    uint8_t month;
    uint16_t year;
};
struct History{
    char word[16];
    struct Date d;
    uint32_t allowedattempts;
}
class Actor : public std::enable_shared_from_this<Actor>{
private:
    uWS::WebSocket<true,true,PerSocketData>* connection;
    char uuid[UUID4_LEN];
public:
    Actor(uWS::WebSocket<true,true,PerSocketData>* connection);
    uint64_t getid();
    uWS::WebSocket<true,true,PerSocketData>* getconnection();
    void setuuid(char* uuid);
};
#endif
