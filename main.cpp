#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <limits> // std::numeric_limits

#include "debug_utils/debug_utils.h"
#include "soduku.h"
#include "solve/peerpruner.h"
#include "solve/singles/nakedsingle.h"
#include "solve/singles/hiddensingle.h"
#include "solve/intersections/lockedcandidates.h"
#include "solve/subsets/nakedsubset.h"


void pressAnyKeyToContinue() {
    std::cout << "按任意键继续..." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void clearConsole() {
    std::cout << "\033[2J\033[H";
}

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

    // LOG_INFO("The initial state of Sudoku...");
    // soduku.print(std::cout);

    PeerPruner peerPruner(&soduku);
    NakedSingleSolver nakedSingleSolver(&soduku);
    HiddenSingleSolver hiddenSingleSolver(&soduku);

    LockedCandidatesPointing lockedCandidatesPointing(&soduku);
    LockedCandidatesClaiming lockedCandidatesClaiming(&soduku);
    
    NakedSubsetSolver nakedSubsetSolver(&soduku);
    

    clearConsole();

    while(true) {
        peerPruner.prune();  
        soduku.print(std::cout);
        pressAnyKeyToContinue();
        clearConsole();

        Cell* c = nullptr;
        bool change = false;

        c = nakedSingleSolver.solve();
        if(c != nullptr) {
            continue;
        }

        c = hiddenSingleSolver.solve();
        if(c != nullptr) {
            continue;
        }

        change = lockedCandidatesPointing.solve();
        if(change) {
            continue;
        }

        change = lockedCandidatesClaiming.solve();
        if(change) {
            continue;
        }

        change = nakedSubsetSolver.solve();
        if(change) {
            continue;
        }

        break;
    }

    LOG_INFO("The final result...(really?)");
    soduku.print(std::cout);
    return 0;
}

