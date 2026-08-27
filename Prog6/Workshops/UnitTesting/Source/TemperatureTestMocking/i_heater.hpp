#ifndef I_HEATER_HPP
#define I_HEATER_HPP

namespace temperature {

/// @brief Interface for heater control
/// @details Allows dependency injection for testability
class IHeater {
public:
    virtual ~IHeater() = default;

    /// @brief Turn heater on
    virtual void turnOn() = 0;

    /// @brief Turn heater off
    virtual void turnOff() = 0;

    /// @brief Check if heater is currently on
    /// @return true if heater is on
    virtual bool isOn() const = 0;
};

}  // namespace temperature

#endif  // I_HEATER_HPP