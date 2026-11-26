/*
 * 03_threads - The same two jobs as 02_superloop, now as RTOS threads.
 *
 * Each job gets its own thread with its own priority (lower number =
 * higher priority in Zephyr). The scheduler preempts the slow thread
 * whenever the fast one needs to run.
 *
 * Watch the output: FAST keeps ticking every 100 ms, right through the
 * heavy work of SLOW. That is what an RTOS buys you.
 *
 * Experiment (the point of this exercise):
 *   1. Swap the two priorities (make FAST 7 and SLOW 5) and run again.
 *      Now FAST starves while SLOW is busy. Priorities matter!
 *   2. Lower the FAST interval to 10 ms. Still rock solid?
 */

#include <zephyr/kernel.h>

#define STACK_SIZE   1024
#define PRIO_FAST    5   /* higher priority (lower number) */
#define PRIO_SLOW    7   /* lower priority */

static void fast_thread(void *a, void *b, void *c)
{
    while (1) {
        printk("[fast] tick at %lld ms\n", k_uptime_get());
        k_msleep(100);
    }
}

static void slow_thread(void *a, void *b, void *c)
{
    while (1) {
        k_msleep(3000);
        printk("[slow] heavy work starts...\n");
        k_busy_wait(1000000);  /* 1 second of blocking CPU work */
        printk("[slow] heavy work done\n");
    }
}

K_THREAD_DEFINE(fast_id, STACK_SIZE, fast_thread, NULL, NULL, NULL,
                PRIO_FAST, 0, 0);
K_THREAD_DEFINE(slow_id, STACK_SIZE, slow_thread, NULL, NULL, NULL,
                PRIO_SLOW, 0, 0);

/* No main() needed: K_THREAD_DEFINE starts both threads at boot. */
