
#include "words.h"
#include <chrono>
#include <memory>
#include <vector>
#include <iostream>
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

int main()
{
    nlohmann::json j ;
    j["a"] = nlohmann::json::object();
    j["a"]["1"] = "a";
    j["b"] = nlohmann::json::object();
    j["b"]["1"] = "b";
    j["c"] = nlohmann::json::object();
    j["c"]["1"] = "c";
    j["d"] = nlohmann::json::object();
    j["d"]["1"] = "d";
    
    j.items().begin();
    uint16_t r = 0;
    FILE* f = fopen("/dev/urandom","rb");
    if(f){
        fread(&r,sizeof(uint8_t),1,f);
        fclose(f);
    }
    
    
    std::vector<std::string> vec;
    for(auto&[key,val] : j.items()){
        vec.push_back(key);
    }
    std::cout << vec.size() << std::endl;
    vec.erase(std::remove(vec.begin(), vec.end(), "d"), vec.end());
    std::cout << vec.size() << std::endl;
    assert(1==2);
    return 0;
}
