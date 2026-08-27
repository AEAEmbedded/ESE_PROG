// logger.hpp
#pragma once

#include <string>

/// @brief Simple logging utility
/// @details Stateless logger - safe to copy and move
class Logger {
public:
    Logger() = default;
    ~Logger() = default;

    // Explicitly default copy/move operations (Rule of Five)
    Logger(const Logger&) = default;
    Logger& operator=(const Logger&) = default;
    Logger(Logger&&) = default;
    Logger& operator=(Logger&&) = default;

    /// @brief Log informational message
    /// @param message Message to log
    void log(const std::string& message) const;

    /// @brief Log error message
    /// @param message Error message to log
    void error(const std::string& message) const;
};
