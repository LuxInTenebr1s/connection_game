#include <zephyr.h>

#include <sys/printk.h>

#include <device.h>
#include <drivers/gpio.h>
#include <sys/util.h>

#define GPIO_OUT_DRV_NAME DT_ALIAS_LED0_GPIOS_CONTROLLER
#define GPIO_LED0_PIN DT_ALIAS_LED0_GPIOS_PIN
#define GPIO_LED1_PIN DT_ALIAS_LED1_GPIOS_PIN

#define GPIO_IN_DRV_NAME DT_ALIAS_SW0_GPIOS_CONTROLLER
#define GPIO_SW0_PIN DT_ALIAS_SW0_GPIOS_PIN
#define GPIO_SW1_PIN DT_ALIAS_SW1_GPIOS_PIN

void gpio_callback(struct device *port, struct gpio_callback *cb, u32_t pins)
{
    int led = -1, pin = -1;
    u32_t value;

    if (pins & BIT(GPIO_SW0_PIN)) {
        pin = GPIO_SW0_PIN;
        led = GPIO_LED0_PIN;
    }
    else if (pins & BIT(GPIO_SW1_PIN)) {
        pin = GPIO_SW1_PIN;
        led = GPIO_LED1_PIN;
    }
    printk("Pin %d triggered! Toggling LED%d.\n", pin, led == GPIO_LED0_PIN ? 1 : 2);

    gpio_pin_read(port, led, &value);
    gpio_pin_write(port, led, value > 0 ? 0 : 1);
}

static struct gpio_callback gpio_cb;

void main(void)
{
    struct device *gpio_out_dev, *gpio_in_dev;
    int ret = 0;

    gpio_out_dev = device_get_binding(GPIO_OUT_DRV_NAME);
    if (!gpio_out_dev) {
        printk("Cannot find %s!\n", GPIO_OUT_DRV_NAME);
        return;
    }

    gpio_in_dev = device_get_binding(GPIO_IN_DRV_NAME);
    if (!gpio_in_dev) {
        printk("Cannot find %s!\n", GPIO_IN_DRV_NAME);
        return;
    }

    ret += gpio_pin_configure(gpio_out_dev, GPIO_LED0_PIN, (GPIO_DIR_OUT));
    ret += gpio_pin_configure(gpio_out_dev, GPIO_LED1_PIN, (GPIO_DIR_OUT));
    if (ret) {
        printk("Error configuring output pins!\n");
        return;
    }

    ret += gpio_pin_configure(gpio_in_dev, GPIO_SW0_PIN,
				 (GPIO_DIR_IN | GPIO_INT | GPIO_INT_EDGE |
                  GPIO_INT_ACTIVE_HIGH | GPIO_INT_DEBOUNCE));
    ret += gpio_pin_configure(gpio_in_dev, GPIO_SW1_PIN,
				 (GPIO_DIR_IN | GPIO_INT | GPIO_INT_EDGE |
                  GPIO_INT_ACTIVE_HIGH | GPIO_INT_DEBOUNCE));
    if (ret) {
        printk("Error configuring input pins!\n");
        return;
    }

    gpio_init_callback(&gpio_cb, gpio_callback, BIT(GPIO_SW0_PIN) |
                                                BIT(GPIO_SW1_PIN));

    ret = gpio_add_callback(gpio_in_dev, &gpio_cb);
    if (ret) {
        printk("Cannot setup callback!\n");
        return;
    }

    ret += gpio_pin_enable_callback(gpio_in_dev, GPIO_SW0_PIN);
    ret += gpio_pin_enable_callback(gpio_in_dev, GPIO_SW1_PIN);
    if (ret) {
        printk("Error enablink callback!\n");
        return;
    }

    while (1) {
        printk("I'm alive!\n");
        k_sleep(K_SECONDS(5));
    }
}
