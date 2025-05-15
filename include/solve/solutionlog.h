#pragma once

#include <string>
#include <vector>


class SolutionLog {
   public:
   static SolutionLog& getInstance();
    SolutionLog();
    void info(const std::string& s);

    SolutionLog(const SolutionLog&) = delete;
    SolutionLog& operator=(const SolutionLog&) = delete;

   private:
    std::vector<std::string> logs;
};


// #define SOLUTION_DEBUG(message) SolutionLog::getInstance().debug(message)
#define SOLUTION_INFO(message) SolutionLog::getInstance().info(message)
// #define SOLUTION_WARNING(message) SolutionLog::getInstance().warning(message)
// #define SOLUTION_ERROR(message) SolutionLog::getInstance().error(message)
// #define SOLUTION_CRITICAL(message) SolutionLog::getInstance().critical(message)