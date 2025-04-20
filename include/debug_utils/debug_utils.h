#pragma once

#include <string_view>
#include <source_location>

#include "debug_utils/debug_config.h"

namespace debug {

    void log_impl(LogLevel level, std::string_view message,
                  const std::source_location& location = std::source_location::current());

    void assert_impl(bool condition,
                     std::string_view message,
                     std::string_view cond_str,
                     const std::source_location& location = std::source_location::current());

    void panic(std::string_view message,
               const std::source_location& location = std::source_location::current());
}

// 宏封装
#ifndef NDEBUG
#define ASSERT(cond, msg) \
    debug::assert_impl(static_cast<bool>(cond), msg, #cond, std::source_location::current())
#else
#define ASSERT(cond, msg) ((void)0)
#endif

#define LOG_INFO(msg)  debug::log_impl(debug::LogLevel::Info, msg, std::source_location::current())
#define LOG_WARN(msg)  debug::log_impl(debug::LogLevel::Warn, msg, std::source_location::current())
#define LOG_ERROR(msg) debug::log_impl(debug::LogLevel::Error, msg, std::source_location::current())
#define PANIC(msg)     debug::panic(msg, std::source_location::current())
