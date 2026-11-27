#include "payload.hpp"

#include <chrono>
#include <cstdio>
#include <cstdlib>

namespace sim {

int64_t unixNow() {
    using namespace std::chrono;
    return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
}

void Payload::separator(const std::string& key) {
    body_ += body_.empty() ? "{" : ",";
    body_ += '"' + key + "\":";
}

Payload& Payload::add(const std::string& key, double value) {
    separator(key);
    char buffer[32];
    std::snprintf(buffer, sizeof buffer, "%.6g", value);  // 37.2 -> "37.2"
    body_ += buffer;
    return *this;
}

Payload& Payload::add(const std::string& key, int64_t value) {
    separator(key);
    body_ += std::to_string(value);
    return *this;
}

Payload& Payload::add(const std::string& key, bool value) {
    separator(key);
    body_ += value ? "true" : "false";
    return *this;
}

Payload& Payload::add(const std::string& key, const std::string& value) {
    separator(key);
    body_ += '"';
    for (const char c : value) {  // escape the two characters that matter here
        if (c == '"' || c == '\\') {
            body_ += '\\';
        }
        body_ += c;
    }
    body_ += '"';
    return *this;
}

std::string Payload::str() const {
    return body_.empty() ? "{}" : body_ + "}";
}

namespace {

/// Position just after `"key":` and any spaces, or npos.
std::size_t valueStart(const std::string& json, const std::string& key) {
    const std::string needle = '"' + key + '"';
    auto pos = json.find(needle);
    if (pos == std::string::npos) {
        return pos;
    }
    pos = json.find(':', pos + needle.size());
    if (pos == std::string::npos) {
        return pos;
    }
    ++pos;
    while (pos < json.size() && json[pos] == ' ') {
        ++pos;
    }
    return pos;
}

} // namespace

std::optional<double> extractNumber(const std::string& json, const std::string& key) {
    const auto pos = valueStart(json, key);
    if (pos == std::string::npos || pos >= json.size()) {
        return std::nullopt;
    }
    const char* begin = json.c_str() + pos;
    char* end = nullptr;
    const double value = std::strtod(begin, &end);
    if (end == begin) {
        return std::nullopt;  // not a number (string, null, object, ...)
    }
    return value;
}

std::optional<std::string> extractString(const std::string& json, const std::string& key) {
    const auto pos = valueStart(json, key);
    if (pos == std::string::npos || pos >= json.size() || json[pos] != '"') {
        return std::nullopt;
    }
    const auto close = json.find('"', pos + 1);
    if (close == std::string::npos) {
        return std::nullopt;
    }
    return json.substr(pos + 1, close - pos - 1);
}

} // namespace sim
