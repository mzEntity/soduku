#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#include "debug_utils/debug_utils.h"
#include "soduku.h"
#include "solve/peerpruner.h"



std::string getCurrentTimeString() {
    using namespace std;

    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    
    ostringstream oss;
    oss << put_time(localtime(&now_time), "%Y-%m-%d %H:%M:%S");
    
    return oss.str();
}

int main() {
    using namespace std;
    LOG_INFO("Hello world!");
    LOG_INFO("It's now " + getCurrentTimeString());

    Soduku soduku;
    vector<vector<int>> quest = {
        {-1, 2, 7, -1, 4, -1, 1, -1, -1},
        {-1, -1, -1, -1, 1, -1, -1, -1, -1},
        {5, -1, 9, -1, -1, -1, -1, -1, 4},

        {-1, -1, 6, 5, -1, -1, -1, -1, -1},
        {-1, 9, 1, -1, -1, -1, 2, 6, -1},
        {-1, -1, -1, -1, -1, 2, 4, -1, -1},

        {7, -1, -1, -1, -1, -1, 3, -1, 8},
        {-1, -1, -1, -1, 7, -1, -1, -1, -1},
        {-1, -1, 8, -1, 9, -1, 6, 7, -1}
    };
    soduku.init(quest);
    soduku.print(std::cout);

    PeerPruner p(&soduku);
    p.prune();
    soduku.print(std::cout);
    return 0;
}

