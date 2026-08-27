// config.hpp
#pragma once

#include <cstdint>

/// @brief Configuration management
/// @details Follows RAII - configuration loaded on construction
class Config {
public:
    /// @brief Load configuration on construction
    Config();
    ~Config() = default;

    // Explicitly default copy/move operations (Rule of Five)
    Config(const Config&) = default;
    Config& operator=(const Config&) = default;
    Config(Config&&) = default;
    Config& operator=(Config&&) = default;

    /// @brief Get sensor sampling rate
    /// @return Sample rate in milliseconds
    int32_t getSampleRate() const;

    /// @brief Check if debug mode is enabled
    /// @return true if debug mode is enabled, false otherwise
    bool isDebugMode() const;

private:
    static constexpr int32_t DEFAULT_SAMPLE_RATE_MS = 1000;
    static constexpr bool DEFAULT_DEBUG_MODE = true;
};
