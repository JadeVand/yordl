
#include "words.h"

int main(){
   // Word w;
   // w.getnewword();
    nlohmann::json j;
    std::ifstream ifs("history.json");
    if(!ifs.good()){
        nlohmann::json j ;
        j["hello"] = nlohmann::json::object();
        j["hello"]["date"] = 0xdeadbeef;
        j["hello"]["str"] = "b";
        std::ofstream ofs("history.json");
        ofs << j;
        ofs.close();
    }else{
        j = nlohmann::json::parse(ifs);
        j["dart"] = nlohmann::json::object();
        j["dart"]["date"] = 0xcafef00d;
        ifs.close();
        std::ofstream ofs("history.json");
        ofs << j;
        ofs.close();
    }
    return 0;
}
