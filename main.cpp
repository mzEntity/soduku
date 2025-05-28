#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <limits>
#include <fstream>

#include "debug_utils/debug_utils.h"
#include "sudoku.h"
#include "solve/peerpruner.h"
#include "solve/singles/nakedsingle.h"
#include "solve/singles/hiddensingle.h"
#include "solve/intersections/lockedcandidates.h"
#include "solve/subsets/nakedsubset.h"
#include "solve/subsets/hiddensubset.h"

#include "solve/link.h"

#include "solve/single_digit_patterns/turbot_fish.h"

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

std::vector<std::vector<int>> loadSudoku(const std::string& file_path) {
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
    Sudoku sudoku;
    vector<vector<int>> quest = loadSudoku(file_path);
    
    sudoku.init(quest);

    LOG_INFO("The initial state of Sudoku...");
    sudoku.print(std::cout);
    pressAnyKeyToContinue();
    clearConsole();

    PeerPruner peerPruner(&sudoku);
    NakedSingleSolver nakedSingleSolver(&sudoku);
    HiddenSingleSolver hiddenSingleSolver(&sudoku);

    LockedCandidatesPointing lockedCandidatesPointing(&sudoku);
    LockedCandidatesClaiming lockedCandidatesClaiming(&sudoku);
    
    NakedSubsetSolver nakedSubsetSolver(&sudoku);
    HiddenSubsetSolver hiddenSubsetSolver(&sudoku);

    LinkManager linkManager(&sudoku);

    TurbotFishSolver turbotFishSolver(&sudoku, &linkManager);

    clearConsole();

    // 如果有候选被消除，则从头开始计算
    while(true) {
        peerPruner.prune();  
        sudoku.print(std::cout);
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

        linkManager.build();
        change = turbotFishSolver.solve();
        if(change) {
            continue;
        }

        break;
    }

    LOG_INFO("The final result...(really?)");
    sudoku.print(std::cout);

    
    // linkManager.print_all_links();

    return 0;
}

