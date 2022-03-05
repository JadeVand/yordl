
#include "words.h"
#include <chrono>
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
    union LeagueLTime{
        uint32_t t;
        struct Parts{
            uint8_t day;
            uint8_t month;
            uint16_t year;
        } p;
    };
    std::time_t t = std::time(nullptr);
      std::tm *const pTInfo = std::localtime(&t);
    std::cout << "Current year: " << 1900 + pTInfo->tm_year << '\n';
    union LeagueLTime llt = {0};
    llt.p.day = (uint8_t)pTInfo->tm_mday;
    llt.p.month = (uint8_t)pTInfo->tm_mon;
    llt.p.year = (uint16_t)(pTInfo->tm_year + 1900);
    //std::cout << llt.t;
    //std::cout <<( llt.t>>8&0xFF);
    union LeagueLTime lt;
    lt.t = 132514308;
    std::cout <<( lt.t>>16);
    return 0;
}
