#include "station/Sampler.hpp"

namespace station {

Sampler::Sampler(EnvironmentSensor& sensor, std::chrono::milliseconds period)
    : sensor_(sensor), period_(period)
{
}

Sampler::~Sampler()
{
    stop();
}

void Sampler::onMeasurement(Callback callback)
{
    callback_ = std::move(callback);
}

void Sampler::start()
{
    if (running_) {
        return;
    }
    running_ = true;
    thread_  = std::thread(&Sampler::run, this);
}

void Sampler::stop()
{
    if (!running_) {
        return;
    }
    running_ = false;
    wakeUp_.notify_all();          // do not wait out the remaining period
    if (thread_.joinable()) {
        thread_.join();
    }
}

void Sampler::run()
{
    while (running_) {
        bmp280::Measurement m;
        if (sensor_.readForced(m) == bmp280::Error::None && callback_) {
            callback_(m);          // sampler thread, not main
        }

        // Sleep for `period_` unless stop() wakes us earlier.
        std::unique_lock<std::mutex> lock(mutex_);
        wakeUp_.wait_for(lock, period_, [this] { return !running_; });
    }
}

} // namespace station
