#pragma once

// Periodically reads an EnvironmentSensor on its own thread and hands every
// measurement to a callback. Owns the thread (composition): stop() joins it,
// the destructor calls stop().

#include "station/EnvironmentSensor.hpp"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

namespace station {

class Sampler {
public:
    using Callback = std::function<void(const bme280::Measurement&)>;

    Sampler(EnvironmentSensor& sensor, std::chrono::milliseconds period);
    ~Sampler();

    Sampler(const Sampler&)            = delete;
    Sampler& operator=(const Sampler&) = delete;

    /// Register the callback *before* start(). It runs on the sampler thread.
    void onMeasurement(Callback callback);

    void start();
    void stop();   ///< returns as soon as the thread has joined, at most one sensor read later
    bool isRunning() const { return running_; }

private:
    void run();

    EnvironmentSensor&        sensor_;
    std::chrono::milliseconds period_;
    Callback                  callback_;
    std::thread               thread_;
    std::atomic<bool>         running_{false};
    std::mutex                mutex_;
    std::condition_variable   wakeUp_;
};

} // namespace station
