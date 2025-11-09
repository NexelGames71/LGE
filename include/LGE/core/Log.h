#pragma once

#include <iostream>
#include <string>

namespace LGE {

enum class LogLevel {
    Trace = 0,
    Info,
    Warn,
    Error,
    Fatal
};

class Log {
public:
    static void Trace(const std::string& message);
    static void Info(const std::string& message);
    static void Warn(const std::string& message);
    static void Error(const std::string& message);
    static void Fatal(const std::string& message);

private:
    static void Print(LogLevel level, const std::string& message);
};

} // namespace LGE

