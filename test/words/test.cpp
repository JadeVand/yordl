
#include "words.h"

int main(){
   // Word w;
   // w.getnewword();
    nlohmann::json jf;
    std::ifstream ifs("history.json");
    if(ifs.good()){
        jf  = nlohmann::json::parse(ifs);
        for (auto& [key, val] : jf.items())
        {
            time_t day = val.at("date");
            std::cout << day << std::endl;
        }
        ifs.close();
    }
    return 0;
}
