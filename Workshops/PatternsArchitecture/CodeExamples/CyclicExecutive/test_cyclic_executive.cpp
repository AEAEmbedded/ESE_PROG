#include "CppUTest/TestHarness.h"
#include "CyclicExecutive.hpp"

using namespace cyclic_executive;

// ============================================================================
// CyclicExecutive Tests
// ============================================================================

TEST_GROUP(CyclicExecutive) {
    CyclicExecutive<8>* scheduler;
    CounterTask* fastTask;
    CounterTask* slowTask;

    void setup() {
        scheduler = new CyclicExecutive<8>();
        fastTask = new CounterTask("fast");
        slowTask = new CounterTask("slow");
    }

    void teardown() {
        delete slowTask;
        delete fastTask;
        delete scheduler;
    }

    void advanceTimeMs(uint32_t ms) {
        for (uint32_t i = 0; i < ms; i++) {
            scheduler->tick();
        }
    }
};

TEST(CyclicExecutive, CanAddTask) {
    CHECK_TRUE(scheduler->addTask(fastTask, 10));
    LONGS_EQUAL(1, scheduler->getTaskCount());
}

TEST(CyclicExecutive, TaskNotRunBeforePeriod) {
    scheduler->addTask(fastTask, 10);

    advanceTimeMs(5);
    scheduler->run();

    LONGS_EQUAL(0, fastTask->getCount());
}

TEST(CyclicExecutive, TaskRunsAtPeriod) {
    scheduler->addTask(fastTask, 10);

    advanceTimeMs(10);
    scheduler->run();

    LONGS_EQUAL(1, fastTask->getCount());
}

TEST(CyclicExecutive, TaskRunsMultipleTimes) {
    scheduler->addTask(fastTask, 10);

    advanceTimeMs(35);
    scheduler->run();  // Runs at 10, 20, 30

    LONGS_EQUAL(3, fastTask->getCount());
}

TEST(CyclicExecutive, MultipleTasks) {
    scheduler->addTask(fastTask, 10);  // Runs at 10, 20, 30, 40, 50...
    scheduler->addTask(slowTask, 25);  // Runs at 25, 50...

    advanceTimeMs(50);
    scheduler->run();

    LONGS_EQUAL(5, fastTask->getCount());
    LONGS_EQUAL(2, slowTask->getCount());
}

TEST(CyclicExecutive, RejectsWhenFull) {
    CyclicExecutive<2> smallScheduler;
    CounterTask task1("t1"), task2("t2"), task3("t3");

    CHECK_TRUE(smallScheduler.addTask(&task1, 10));
    CHECK_TRUE(smallScheduler.addTask(&task2, 20));
    CHECK_FALSE(smallScheduler.addTask(&task3, 30));  // No room!
}

// ============================================================================
// TimeSlotScheduler Tests
// ============================================================================

TEST_GROUP(TimeSlotScheduler) {
    TimeSlotScheduler<4, 4>* scheduler;  // 4 slots, 4 tasks per slot
    CounterTask* taskA;
    CounterTask* taskB;
    CounterTask* taskC;

    void setup() {
        scheduler = new TimeSlotScheduler<4, 4>(10);  // 10ms slots
        taskA = new CounterTask("A");
        taskB = new CounterTask("B");
        taskC = new CounterTask("C");
    }

    void teardown() {
        delete taskC;
        delete taskB;
        delete taskA;
        delete scheduler;
    }

    void advanceTimeMs(uint32_t ms) {
        for (uint32_t i = 0; i < ms; i++) {
            scheduler->tick();
        }
    }
};

TEST(TimeSlotScheduler, CanAddTaskToSlot) {
    CHECK_TRUE(scheduler->addTaskToSlot(0, taskA));
}

TEST(TimeSlotScheduler, StartsAtSlotZero) {
    LONGS_EQUAL(0, scheduler->getCurrentSlot());
}

TEST(TimeSlotScheduler, RunsTasksInSlot) {
    scheduler->addTaskToSlot(0, taskA);
    scheduler->addTaskToSlot(0, taskB);

    advanceTimeMs(10);
    scheduler->run();

    LONGS_EQUAL(1, taskA->getCount());
    LONGS_EQUAL(1, taskB->getCount());
}

TEST(TimeSlotScheduler, AdvancesToNextSlot) {
    scheduler->addTaskToSlot(0, taskA);
    scheduler->addTaskToSlot(1, taskB);

    advanceTimeMs(10);
    scheduler->run();  // Slot 0

    LONGS_EQUAL(1, scheduler->getCurrentSlot());
}

TEST(TimeSlotScheduler, CyclesBackToSlotZero) {
    // 4 slots of 10ms each = 40ms cycle
    advanceTimeMs(40);
    for (int i = 0; i < 4; i++) {
        scheduler->run();
        advanceTimeMs(10);
    }

    LONGS_EQUAL(0, scheduler->getCurrentSlot());
}

TEST(TimeSlotScheduler, DifferentTasksInDifferentSlots) {
    scheduler->addTaskToSlot(0, taskA);  // Runs in slot 0
    scheduler->addTaskToSlot(2, taskB);  // Runs in slot 2

    // Run through 40ms (one complete cycle)
    for (int i = 0; i < 4; i++) {
        advanceTimeMs(10);
        scheduler->run();
    }

    LONGS_EQUAL(1, taskA->getCount());
    LONGS_EQUAL(1, taskB->getCount());
}

// ============================================================================
// Workshop Discussion
// ============================================================================

/**
 * CYCLIC EXECUTIVE PATTERN:
 *
 * When to use:
 * - Simple embedded systems without RTOS
 * - Hard real-time requirements (deterministic timing)
 * - Memory-constrained systems (no RTOS overhead)
 * - Safety-critical systems (easier to analyze)
 *
 * When NOT to use:
 * - Complex systems with many independent tasks
 * - When tasks have very different periods (e.g., 1ms and 10s)
 * - When priority-based preemption is needed
 * - When tasks may block (waiting for I/O, etc.)
 *
 * Design guidelines:
 * 1. Tasks must be NON-BLOCKING (return quickly)
 * 2. Long operations must be split across multiple runs
 * 3. Analyze WCET (Worst Case Execution Time) for each task
 * 4. Sum of all task WCETs must fit within the shortest period
 *
 * Example timing budget (10ms base period):
 *   - Button debounce: 50us
 *   - Sensor read: 200us
 *   - Control loop: 500us
 *   - Display update: 1ms
 *   - Total: ~1.75ms << 10ms (OK!)
 *
 * Related patterns:
 * - Static Priority (adds preemption)
 * - Rate Monotonic Scheduling (priority = rate)
 * - Earliest Deadline First (dynamic priorities)
 */
