#ifndef CYCLIC_EXECUTIVE_HPP
#define CYCLIC_EXECUTIVE_HPP

#include <cstdint>
#include <array>

/**
 * =============================================================================
 * CYCLIC EXECUTIVE PATTERN (Round-Robin Scheduler)
 * =============================================================================
 *
 * Problem:
 *   Need to run multiple tasks without a full RTOS.
 *   Tasks have different timing requirements (1ms, 10ms, 100ms, etc.)
 *
 * Solution:
 *   A simple time-triggered scheduler that runs tasks in a fixed sequence.
 *   Each task is called at its configured interval.
 *
 * Advantages:
 *   - No RTOS overhead (memory, context switching)
 *   - Deterministic timing
 *   - Easy to analyze (WCET - Worst Case Execution Time)
 *   - No race conditions (single-threaded)
 *
 * Disadvantages:
 *   - Tasks must be non-blocking (cooperative)
 *   - Long tasks delay other tasks
 *   - No priority preemption
 *
 * Douglass (Ch.4): Cyclic Executive Pattern
 *
 * =============================================================================
 */

namespace cyclic_executive {

// ============================================================================
// Task Interface
// ============================================================================

/**
 * @brief Interface for a schedulable task
 */
class ITask {
public:
    virtual ~ITask() = default;
    virtual void run() = 0;
    virtual const char* getName() const = 0;
};

// ============================================================================
// Simple Cyclic Executive
// ============================================================================

/**
 * @brief Task entry with timing information
 */
struct TaskEntry {
    ITask* task;
    uint32_t periodMs;      // How often to run (ms)
    uint32_t lastRunMs;     // When it last ran
    uint32_t runCount;      // Statistics
};

/**
 * @brief Simple cyclic executive scheduler
 *
 * Call tick() every 1ms from a timer interrupt.
 * Call run() from the main loop.
 */
template<size_t MAX_TASKS = 8>
class CyclicExecutive {
public:
    CyclicExecutive() : numTasks_(0), currentTimeMs_(0) {}

    /**
     * @brief Register a task with a period
     * @param task The task to run
     * @param periodMs How often to run (in milliseconds)
     */
    bool addTask(ITask* task, uint32_t periodMs) {
        if (numTasks_ >= MAX_TASKS) return false;

        tasks_[numTasks_++] = {
            task,
            periodMs,
            0,  // lastRunMs
            0   // runCount
        };
        return true;
    }

    /**
     * @brief Call this every 1ms (from SysTick or timer ISR)
     */
    void tick() {
        currentTimeMs_++;
    }

    /**
     * @brief Call this from main loop - runs due tasks
     */
    void run() {
        for (size_t i = 0; i < numTasks_; ++i) {
            TaskEntry& entry = tasks_[i];

            uint32_t elapsed = currentTimeMs_ - entry.lastRunMs;
            if (elapsed >= entry.periodMs) {
                entry.task->run();
                entry.lastRunMs = currentTimeMs_;
                entry.runCount++;
            }
        }
    }

    // For testing and monitoring
    uint32_t getCurrentTimeMs() const { return currentTimeMs_; }
    size_t getTaskCount() const { return numTasks_; }

    uint32_t getTaskRunCount(size_t index) const {
        if (index < numTasks_) {
            return tasks_[index].runCount;
        }
        return 0;
    }

    // For testing - manually set time
    void setTimeMs(uint32_t ms) { currentTimeMs_ = ms; }

private:
    std::array<TaskEntry, MAX_TASKS> tasks_;
    size_t numTasks_;
    volatile uint32_t currentTimeMs_;  // volatile: modified by ISR
};

// ============================================================================
// Time Slot Scheduler (more deterministic variant)
// ============================================================================

/**
 * @brief Fixed time-slot scheduler
 *
 * Divides time into fixed slots (e.g., 10ms each).
 * Each slot has a predefined sequence of tasks.
 * Guarantees timing regardless of when tasks complete.
 *
 * Example with 10ms slots, 100ms major cycle:
 *   Slot 0: TaskA, TaskB
 *   Slot 1: TaskA
 *   Slot 2: TaskA, TaskC
 *   ... etc
 */
template<size_t SLOTS_PER_CYCLE = 10, size_t MAX_TASKS_PER_SLOT = 4>
class TimeSlotScheduler {
public:
    TimeSlotScheduler(uint32_t slotDurationMs = 10)
        : slotDurationMs_(slotDurationMs)
        , currentSlot_(0)
        , currentTimeMs_(0)
        , lastSlotTimeMs_(0)
    {
        // Initialize all slots as empty
        for (auto& slot : slots_) {
            slot.taskCount = 0;
        }
    }

    /**
     * @brief Add a task to a specific slot
     */
    bool addTaskToSlot(size_t slotIndex, ITask* task) {
        if (slotIndex >= SLOTS_PER_CYCLE) return false;

        Slot& slot = slots_[slotIndex];
        if (slot.taskCount >= MAX_TASKS_PER_SLOT) return false;

        slot.tasks[slot.taskCount++] = task;
        return true;
    }

    /**
     * @brief Call this every 1ms from timer
     */
    void tick() {
        currentTimeMs_++;
    }

    /**
     * @brief Call this from main loop
     */
    void run() {
        uint32_t elapsed = currentTimeMs_ - lastSlotTimeMs_;

        if (elapsed >= slotDurationMs_) {
            // Run all tasks in current slot
            Slot& slot = slots_[currentSlot_];
            for (size_t i = 0; i < slot.taskCount; ++i) {
                slot.tasks[i]->run();
            }

            // Move to next slot
            currentSlot_ = (currentSlot_ + 1) % SLOTS_PER_CYCLE;
            lastSlotTimeMs_ = currentTimeMs_;
        }
    }

    size_t getCurrentSlot() const { return currentSlot_; }

private:
    struct Slot {
        std::array<ITask*, MAX_TASKS_PER_SLOT> tasks;
        size_t taskCount;
    };

    std::array<Slot, SLOTS_PER_CYCLE> slots_;
    uint32_t slotDurationMs_;
    size_t currentSlot_;
    volatile uint32_t currentTimeMs_;
    uint32_t lastSlotTimeMs_;
};

// ============================================================================
// Example Tasks
// ============================================================================

/**
 * @brief Simple counter task for testing
 */
class CounterTask : public ITask {
public:
    CounterTask(const char* name) : name_(name), count_(0) {}

    void run() override { count_++; }
    const char* getName() const override { return name_; }

    int getCount() const { return count_; }
    void reset() { count_ = 0; }

private:
    const char* name_;
    int count_;
};

/**
 * @brief Task that tracks its execution time
 */
class TimingTask : public ITask {
public:
    TimingTask(const char* name) : name_(name), lastRunTime_(0), runCount_(0) {}

    void run() override {
        runCount_++;
        // In real code: lastRunTime_ = HAL_GetTick();
    }

    const char* getName() const override { return name_; }

    uint32_t getLastRunTime() const { return lastRunTime_; }
    int getRunCount() const { return runCount_; }

    // For testing
    void setLastRunTime(uint32_t time) { lastRunTime_ = time; }

private:
    const char* name_;
    uint32_t lastRunTime_;
    int runCount_;
};

}  // namespace cyclic_executive

#endif  // CYCLIC_EXECUTIVE_HPP
