#include <App.h>
#include <nlohmann/json.hpp>
#include <uv.h>
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

struct PerSocketData {
    void* data;
};

enum class Identifiers : uint32_t
{
    kNothing = 0,
    kMyId = 1,
    kJoin = 2,
    kLeave = 3,
    kUpdate = 4,
    kForward = 5,
    kUpdateAngle = 6,
    kStopForward = 7,
    kProjectile = 8,
};
class Projectile{
public:
    float x;
    float y;
    uint64_t spawntime;
    float speed;
    float angle;
    Projectile(float angle,uint64_t spawntime){
        speed = 0.5;
        this->angle = angle;
        this->spawntime = spawntime;
    }
    void update(float delta){
        x += speed* sinf(angle) * delta;
        y -= speed* cosf(angle) * delta;
    }
};
class Actor : public std::enable_shared_from_this<Actor>{
private:
    uWS::WebSocket<true,true,PerSocketData>* connection;
    float angle;
    bool forward;
    float x;
    float y;
    int bulletcount;
    std::set<std::shared_ptr<Projectile>> projectiles;
public:
    Actor(uWS::WebSocket<true,true,PerSocketData>* connection);
    uint64_t getid();
    uWS::WebSocket<true,true,PerSocketData>* getconnection();
    void goforward();
    void stopforward();
    void updateangle(float angle);
    void update(float delta);
    float getx();
    float gety();
    float getdirection();
    void incrementbullet();
    void decrementbullet();
    int getprojectilecount();
    void createprojectile(uint64_t timestamp);
    
};
