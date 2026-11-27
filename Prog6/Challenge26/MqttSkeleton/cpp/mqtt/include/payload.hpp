/**
 * @file payload.hpp
 * @brief Minimal JSON payload building and reading for the skeleton.
 *
 * The challenge payloads are flat JSON objects, so a tiny builder is enough
 * to get going without a dependency. For the real project use a JSON library
 * (for example nlohmann/json via FetchContent), especially for the scenario
 * files: `extractNumber` and `extractString` do a plain text search and do
 * not handle nesting or escaping.
 */

#ifndef SIM_PAYLOAD_HPP
#define SIM_PAYLOAD_HPP

#include <cstdint>
#include <optional>
#include <string>

namespace sim {

/// Unix epoch seconds, the timestamp format used in all payloads.
int64_t unixNow();

/// Builds `{"key":value,...}`. Keys are written in insertion order.
class Payload {
public:
    Payload& add(const std::string& key, double value);
    Payload& add(const std::string& key, int64_t value);
    Payload& add(const std::string& key, int value) { return add(key, static_cast<int64_t>(value)); }
    Payload& add(const std::string& key, bool value);
    Payload& add(const std::string& key, const std::string& value);
    Payload& add(const std::string& key, const char* value) { return add(key, std::string(value)); }

    /// The JSON text.
    std::string str() const;

private:
    void separator(const std::string& key);
    std::string body_;
};

/// Value of a top-level numeric field, or nullopt when absent or not a number.
std::optional<double> extractNumber(const std::string& json, const std::string& key);

/// Value of a top-level string field (without quotes), or nullopt.
std::optional<std::string> extractString(const std::string& json, const std::string& key);

} // namespace sim

#endif // SIM_PAYLOAD_HPP
