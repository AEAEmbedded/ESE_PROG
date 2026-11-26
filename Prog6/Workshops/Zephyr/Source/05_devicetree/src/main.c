/*
 * 05_devicetree - The hardware is described in data, not in code.
 *
 * Devicetree answers "what hardware is there and where": every board
 * ships a .dts file, your app can add to it with an overlay
 * (app.overlay here). At BUILD time this becomes C macros: your code
 * asks the devicetree instead of hard-coding addresses.
 *
 * After building, look at the merged result of board + overlay:
 *   build/zephyr/zephyr.dts
 * That generated file is the truth. When a devicetree macro confuses
 * you (or an AI assistant), look there first.
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>

/* The console UART: which device is it, and at which address? */
#define CONSOLE_NODE DT_CHOSEN(zephyr_console)

/* Our own setting from app.overlay ('-' in DT becomes '_' in C) */
#define FEVER_THRESHOLD_MDEG DT_PROP(DT_PATH(zephyr_user), fever_threshold_mdeg)

int main(void)
{
    const struct device *console = DEVICE_DT_GET(CONSOLE_NODE);

    printk("board:            %s\n", CONFIG_BOARD);
    printk("console device:   %s\n", console->name);
    printk("console address:  0x%08lx (from devicetree, not hard-coded)\n",
           (unsigned long)DT_REG_ADDR(CONSOLE_NODE));
    printk("RAM size:         %d KiB\n",
           DT_REG_SIZE(DT_CHOSEN(zephyr_sram)) / 1024);
    printk("fever threshold:  %d millidegrees (from app.overlay)\n",
           FEVER_THRESHOLD_MDEG);

    return 0;
}
