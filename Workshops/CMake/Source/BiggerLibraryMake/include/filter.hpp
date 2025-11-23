// filter.hpp
#pragma once

/// @brief Signal filtering utility
/// @details Stateless filter - safe to copy and move
class Filter {
public:
    Filter() = default;
    ~Filter() = default;

    // Explicitly default copy/move operations (Rule of Five)
    Filter(const Filter&) = default;
    Filter& operator=(const Filter&) = default;
    Filter(Filter&&) = default;
    Filter& operator=(Filter&&) = default;

    /// @brief Apply filter to input value
    /// @param value Input value to filter
    /// @return Filtered output value
    float apply(float value) const;

private:
    static constexpr float FILTER_COEFFICIENT = 0.9F;
};
