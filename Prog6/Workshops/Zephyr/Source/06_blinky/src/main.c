/*
 * 06_blinky - The same code on REAL hardware.
 *
 * This app never names a pin number or a port address. It asks the
 * devicetree for "the LED known as led0" and the board's own .dts
 * (plus any overlay) decides what that is:
 *
 *   XIAO nRF52840: works out of the box - led0 is the red channel of
 *                  the onboard RGB LED (led1 = green, led2 = blue)
 *   Pico 2 (W):    see boards/rpi_pico2_rp2350a_m33.overlay - on the W
 *                  the onboard LED hangs off the WiFi chip, so we map
 *                  led0 to an external LED on a GPIO pin instead
 *
 * Build (pick your board):
 *   west build -p -b xiao_ble/nrf52840
 *   west build -p -b rpi_pico2/rp2350a/m33
 *
 * Flash (both boards have a UF2 bootloader, no tools needed):
 *   XIAO:    double-tap the reset button, then copy
 *            build/zephyr/zephyr.uf2 onto the XIAO drive
 *   Pico 2:  hold BOOTSEL while plugging in USB, then copy
 *            build/zephyr/zephyr.uf2 onto the RP2350 drive
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

static const struct gpio_dt_spec led =
    GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

int main(void)
{
    if (!gpio_is_ready_dt(&led)) {
        printk("LED device not ready\n");
        return 0;
    }

    gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);

    while (1) {
        gpio_pin_toggle_dt(&led);
        printk("blink at %lld ms\n", k_uptime_get());
        k_msleep(500);
    }

    return 0;
}
