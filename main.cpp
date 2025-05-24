#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <limits>
#include <fstream>

#include "debug_utils/debug_utils.h"
#include "soduku.h"
#include "solve/peerpruner.h"
#include "solve/singles/nakedsingle.h"
#include "solve/singles/hiddensingle.h"
#include "solve/intersections/lockedcandidates.h"
#include "solve/subsets/nakedsubset.h"
#include "solve/subsets/hiddensubset.h"
#include "solve/link.h"


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

std::vector<std::vector<int>> loadSoduku(const std::string& file_path) {
    using namespace std;
    ifstream inputFile(file_path);
    vector<vector<int>> result;
    if (!inputFile.is_open()) {
        cerr << "无法打开文件" << endl;
        return result;
    }
    
    string line;
    while (getline(inputFile, line)) {
        size_t index = 0;
        size_t last_index = 0;
        vector<int> cur_line_nums;
        while((index = line.find_first_of(',', last_index)) != string::npos){
            cur_line_nums.push_back(stoi(line.substr(last_index, index-last_index)));
            last_index = index+1;
        }
        cur_line_nums.push_back(stoi(line.substr(last_index)));
        result.push_back(cur_line_nums);
    }
    
    inputFile.close();
    return result;
}


int main() {
    using namespace std;
    LOG_INFO("Hello world!");
    LOG_INFO("It's now " + getCurrentTimeString());

    string file_path = "assets/test3.txt";
    Soduku soduku;
    vector<vector<int>> quest = loadSoduku(file_path);
    
    soduku.init(quest);

    LOG_INFO("The initial state of Sudoku...");
    soduku.print(std::cout);
    pressAnyKeyToContinue();
    clearConsole();

    PeerPruner peerPruner(&soduku);
    NakedSingleSolver nakedSingleSolver(&soduku);
    HiddenSingleSolver hiddenSingleSolver(&soduku);

    LockedCandidatesPointing lockedCandidatesPointing(&soduku);
    LockedCandidatesClaiming lockedCandidatesClaiming(&soduku);
    
    NakedSubsetSolver nakedSubsetSolver(&soduku);
    HiddenSubsetSolver hiddenSubsetSolver(&soduku);

    LinkManager linkManager(&soduku);

    clearConsole();

    // 如果有候选被消除，则从头开始计算
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

        change = hiddenSubsetSolver.solve();
        if(change) {
            continue;
        }

        break;
    }

    LOG_INFO("The final result...(really?)");
    soduku.print(std::cout);

    linkManager.build();
    linkManager.print_all_links();

    return 0;
}

