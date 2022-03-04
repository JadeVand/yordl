
#include "words.h"

int main(){
   // Word w;
   // w.getnewword();
    nlohmann::json j;
    j["category"] = "yo";
    j["akali"] = nlohmann::json::object();
    j["akali"]["q"] = "b";
    std::ofstream o("key.json");
    o << j;
    return 0;
}
