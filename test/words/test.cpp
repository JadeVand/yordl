
#include "words.h"
#include <chrono>
#include <memory>
#include <vector>
class T{
public:
    T(){
    }
    
    ~T(){
        std::ofstream ofs("x");
        ofs << "hello";
        ofs.close();
    }
};
using namespace std;
class StringContainer{
public:
std:;string s;
};
int main()
{
    nlohmann::json j ;
    j["x"] = nlohmann::json::object();
    j["x"]["y"] = "x";
    j["y"] = nlohmann::json::object();
    j["y"]["x"] = "y";
    std::vector<std::shared_ptr<StringContainer> vsc ;

    for(auto&[key,val] : j){
        std::shared_ptr<StringContainer> sc = std::make_shared<StringContainer>();
        sc->s.assign(k);
        vsc.push_back(sc);
    }
    return 0;
}
