#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/regs/addressmap.h"
#include "hardware/regs/sio.h"

const uint LED_PIN = 25;

int main(void)
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // RP2040 Datasheet, SIO: SET +0x14, CLR +0x18, SIO_BASE 0xd0000000.
    volatile uint32_t *gpio_out_set = (volatile uint32_t *)(SIO_BASE + SIO_GPIO_OUT_SET_OFFSET);
    volatile uint32_t *gpio_out_clr = (volatile uint32_t *)(SIO_BASE + SIO_GPIO_OUT_CLR_OFFSET);
    const uint32_t led_mask = 1u << LED_PIN;

    while (1)
    {
        *gpio_out_set = led_mask;
        sleep_ms(250);
        *gpio_out_clr = led_mask;
        sleep_ms(1000);
    }
}
