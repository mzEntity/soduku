#include "solve/solutionlog.h"
#include "debug_utils/debug_utils.h"

#include <iostream>

SolutionLog& SolutionLog::getInstance() {
    static SolutionLog instance;
    return instance;
}

SolutionLog::SolutionLog(){}

void SolutionLog::info(const std::string& s){
    this->logs.push_back(s);   
    std::cout << s << std::endl;
    // LOG_INFO(s);
}

