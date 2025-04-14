#include "debug_utils/debug_utils.h"
#include <iostream>
#include <cstdlib>

namespace debug {

    void log_impl(LogLevel level, std::string_view message,
                  const std::source_location& location) {
        if (level < CURRENT_LOG_LEVEL) return;

        const char* level_str = nullptr;
        const char* color_code = nullptr;
        switch (level) {
            case LogLevel::Info:  level_str = "INFO "; color_code = "\033[32m"; break;
            case LogLevel::Warn:  level_str = "WARN "; color_code = "\033[33m"; break;
            case LogLevel::Error: level_str = "ERROR"; color_code = "\033[31m"; break;
        }
        if (USE_COLOR) {
            std::cerr << color_code << "[" << level_str << "] " << "\033[0m"
                  << location.file_name() << ":" << location.line() << " "
                  << location.function_name() << "(): "
                  << message << std::endl;
        } else {
            std::cerr << "[" << level_str << "] "
                  << location.file_name() << ":" << location.line() << " "
                  << location.function_name() << "(): "
                  << message << std::endl;
        }
        
    }

    void assert_impl(bool condition, std::string_view message,
                     std::string_view cond_str, const std::source_location& location) {
        if (!condition) {
            log_impl(LogLevel::Error,
                     "Assertion failed: " + std::string(cond_str) + " | " + std::string(message),
                     location);
            std::abort();
        }
    }

    void panic(std::string_view message, const std::source_location& location) {
        log_impl(LogLevel::Error, "PANIC: " + std::string(message), location);
        std::abort();
    }

}
