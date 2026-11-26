/*
 * 01_hello - Your first Zephyr application.
 *
 * Runs on the qemu_cortex_m3 board: an emulated ARM Cortex-M3, so
 * nobody needs hardware or a flasher today.
 *
 * Build and run (from this directory):
 *   west build -b qemu_cortex_m3
 *   west build -t run          (quit QEMU with Ctrl-A, then X)
 */

#include <zephyr/kernel.h>

int main(void)
{
    printk("Hello from Zephyr on board: %s\n", CONFIG_BOARD);

    while (1) {
        printk("uptime: %lld ms\n", k_uptime_get());
        k_msleep(1000);
    }

    return 0;
}
