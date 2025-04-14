#pragma once

namespace debug {
    enum class LogLevel {
        Info,
        Warn,
        Error
    };

    // 当前日志等级（编译时可修改）
    constexpr LogLevel CURRENT_LOG_LEVEL = LogLevel::Info;
    constexpr bool USE_COLOR = true;
}
