#include <fileio.h>
Actor::Actor(uWS::WebSocket<true,true,PerSocketData>* connection){
    this->connection = connection;
    angle = 0;
    forward = false;
    x = 100.0f;
    y = 100.0f;
    bulletcount = 0;
}
uint64_t Actor::getid()
{
    return uint64_t(this);
}
uWS::WebSocket<true,true,PerSocketData>* Actor::getconnection(){
    return connection;
}
void Actor::goforward(){
    forward = true;
}
void Actor::stopforward(){
    forward = false;
}
void Actor::updateangle(float angle){
    this->angle = angle;
}
void Actor::update(float delta){
    if(forward){
        x += 0.1* sinf(angle) * delta;
        y -= 0.1* cosf(angle) * delta;
    }
    for(std::shared_ptr<Projectile> projectile : projectiles){
        projectile->update(delta);
    }
    
}
float Actor::getx(){
    return x;
}
float Actor::gety(){
    return y;
}
float Actor::getdirection(){
    return angle;
}

void Actor::incrementbullet(){
    ++bulletcount;
}

void Actor::decrementbullet(){
    --bulletcount;
}

void Actor::createprojectile(uint64_t timestamp){
    projectiles.insert(std::make_shared<Projectile>(angle,timestamp));
}
int Actor::getprojectilecount(){
    return bulletcount;
}
