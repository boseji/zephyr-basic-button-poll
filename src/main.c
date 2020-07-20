/*
 * Copyright (c) 2020 Abhijit Bose <https://boseji.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

void main(void)
{
    struct device *gpio_led, *gpio_sw;

    /* Check if the 'led0' defintion exists in the Board */
    #define LED0_NODE DT_ALIAS(led0)    
    #if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
    #error "Board Does not have 'led0' defined "
    #endif
    /* Find the LED GPIO Port Name*/
    #define LED0_GPIO   DT_GPIO_LABEL(LED0_NODE, gpios)
    /* Find the LED Pin */
    #define LED0_PIN    DT_GPIO_PIN(LED0_NODE, gpios)
    /* Get GPIO Flags for LED Pin if Available*/
    #if DT_PHA_HAS_CELL(LED0_NODE, gpios, flags)
    #define LED0_FLAGS  DT_GPIO_FLAGS(LED0_NODE, gpios)
    #else
    #define LED0_FLAGS  0
    #endif

    /* Check if the 'sw0' defintion exists in the Board */
    #define SW0_NODE DT_ALIAS(sw0)    
    #if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
    #error "Board Does not have 'sw0' defined "
    #endif
    /* Find the Button GPIO*/
    #define SW0_GPIO    DT_GPIO_LABEL(SW0_NODE, gpios)
    /* Find the Button Pin */
    #define SW0_PIN     DT_GPIO_PIN(SW0_NODE, gpios)
    #if DT_PHA_HAS_CELL(SW0_NODE, gpios, flags)
    #define SW0_FLAGS   DT_GPIO_FLAGS(SW0_NODE, gpios)
    #else
    #define SW0_FLAGS   0
    #endif

    /* Get instance of GPIO for LED */
    gpio_led = device_get_binding(LED0_GPIO);
    if (gpio_led == NULL) {
        return;
    }

    /* Get instance of GPIO for Button */
    gpio_sw = device_get_binding(SW0_GPIO);
    if (gpio_sw == NULL) {
        return;
    }

    /* Enabled LED Pins for Output */
    if (gpio_pin_configure(gpio_led, LED0_PIN, GPIO_OUTPUT|LED0_FLAGS) < 0) {
        return;
    }

    /* Enable SW0 Pin for Input */
    if (gpio_pin_configure(gpio_sw, SW0_PIN, GPIO_INPUT|SW0_FLAGS) < 0) {
        return;
    }

    /* Start the Infinite Loop */
    while(1) {
        /* Check the Button State */
        if (gpio_pin_get(gpio_sw, SW0_PIN)) {
            gpio_pin_set(gpio_led, LED0_PIN, 0);
            /* We Need to Flip this due to the DTS of STM32F3DISCOVERY board 
                ```
                user_button: button {
                    label = "User";
                    gpios = <&gpioa 0 GPIO_ACTIVE_LOW>;
                };
                ```
                Where the button input set to Active Low !
                But in reality its Active High as per schematics.
            */
        } else {
            gpio_pin_set(gpio_led, LED0_PIN, 1);
        }
        /* Wait for 50mS to Debounce */
        k_msleep(50);
    }
}
