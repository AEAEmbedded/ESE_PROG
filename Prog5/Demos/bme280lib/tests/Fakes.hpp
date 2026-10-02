#pragma once

#include "Bus.hpp"
#include "Clock.hpp"

#include <array>
#include <cstring>
#include <vector>

namespace bme280::test {

/// A 256-byte register map with a write log. Substitutable for any Bus.
class MockBus final : public Bus {
public:
    struct Write { uint8_t reg; uint8_t value; };

    MockBus() { regs_[0xD0] = 0x60; }   // a healthy BME280 answers with its chip id

    bool read(uint8_t reg, uint8_t* data, size_t len) override
    {
        if (failReads_) return false;
        std::memcpy(data, &regs_[reg], len);
        ++reads_;
        return true;
    }

    bool write(uint8_t reg, uint8_t value) override
    {
        if (failWrites_) return false;
        regs_[reg] = value;
        writes_.push_back({reg, value});
        return true;
    }

    uint8_t& reg(uint8_t addr) { return regs_[addr]; }
    void load(uint8_t start, const std::vector<uint8_t>& bytes)
    {
        for (size_t i = 0; i < bytes.size(); ++i) regs_[static_cast<uint8_t>(start + i)] = bytes[i];
    }
    const std::vector<Write>& writes() const { return writes_; }
    unsigned reads() const { return reads_; }
    void failReads(bool f = true)  { failReads_ = f; }
    void failWrites(bool f = true) { failWrites_ = f; }

private:
    std::array<uint8_t, 256> regs_{};
    std::vector<Write> writes_;
    unsigned reads_ = 0;
    bool failReads_ = false, failWrites_ = false;
};

/// Counts instead of sleeping. Tests run in microseconds, not milliseconds.
class FakeClock final : public Clock {
public:
    void delayUs(uint32_t us) override { total_ += us; ++calls_; }
    uint32_t totalUs() const { return total_; }
    unsigned calls() const { return calls_; }

private:
    uint32_t total_ = 0;
    unsigned calls_ = 0;
};

} // namespace bme280::test
