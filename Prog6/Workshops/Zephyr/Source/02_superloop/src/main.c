/*
 * 02_superloop - The problem an RTOS solves.
 *
 * One big loop, two jobs:
 *   - FAST: a heartbeat that must tick every 100 ms (think: reading a
 *     patient's heart rate)
 *   - SLOW: heavy work every 3 seconds that takes a full second
 *     (think: updating a display, writing a log)
 *
 * Watch the output: every time SLOW runs, FAST stops ticking for a
 * whole second. In a superloop, one slow job freezes everything else.
 * This is exactly the structure of a typical Arduino loop().
 */

#include <zephyr/kernel.h>

int main(void)
{
    int64_t last_slow_ms = 0;

    while (1) {
        printk("[fast] tick at %lld ms\n", k_uptime_get());

        if ((k_uptime_get() - last_slow_ms) >= 3000) {
            printk("[slow] heavy work starts...\n");
            k_busy_wait(1000000);  /* 1 second of blocking CPU work */
            printk("[slow] heavy work done\n");
            last_slow_ms = k_uptime_get();
        }

        k_msleep(100);
    }

    return 0;
}
