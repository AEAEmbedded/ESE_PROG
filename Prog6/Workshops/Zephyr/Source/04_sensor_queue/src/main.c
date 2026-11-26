/*
 * 04_sensor_queue - Threads that talk to each other.
 *
 * Producer/consumer with a message queue:
 *   - sensor thread: simulates a body thermometer, one reading per
 *     500 ms (the patient slowly develops a fever...)
 *   - monitor thread: receives readings from the queue and raises an
 *     alarm above the fever threshold
 *
 * The threads never share variables: all data travels through the
 * queue. No race conditions, no locking headaches. This is the same
 * pattern your final-challenge engine can use between its parts.
 */

#include <zephyr/kernel.h>

#define FEVER_THRESHOLD_MDEG  38000  /* 38.000 degrees C, in millidegrees */

struct sensor_reading {
    int64_t timestamp_ms;
    int32_t temperature_mdeg;
};

K_MSGQ_DEFINE(sensor_queue, sizeof(struct sensor_reading), 8, 4);

static void sensor_thread(void *a, void *b, void *c)
{
    int32_t temperature_mdeg = 36500;  /* start at 36.5 C */

    while (1) {
        struct sensor_reading reading = {
            .timestamp_ms = k_uptime_get(),
            .temperature_mdeg = temperature_mdeg,
        };

        k_msgq_put(&sensor_queue, &reading, K_FOREVER);

        temperature_mdeg += 100;  /* fever rises 0.1 C per reading */
        k_msleep(500);
    }
}

static void monitor_thread(void *a, void *b, void *c)
{
    struct sensor_reading reading;

    while (1) {
        k_msgq_get(&sensor_queue, &reading, K_FOREVER);

        printk("[monitor] t=%6lld ms  temp=%d.%03d C%s\n",
               reading.timestamp_ms,
               reading.temperature_mdeg / 1000,
               reading.temperature_mdeg % 1000,
               (reading.temperature_mdeg > FEVER_THRESHOLD_MDEG)
                   ? "  *** FEVER ALARM ***" : "");
    }
}

K_THREAD_DEFINE(sensor_id, 1024, sensor_thread, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(monitor_id, 1024, monitor_thread, NULL, NULL, NULL, 6, 0, 0);
