#include <driver/gpio.h>
#include "pins.h"
#include "ISR.h"


//pins for led and button
#define led_Pin 27
#define button_Pin 12

//initiates the button
void initLEDandButton()
{
    gpio_set_direction(button_Pin, GPIO_MODE_INPUT);
    gpio_set_direction(led_Pin, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(button_Pin, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(button_Pin, GPIO_INTR_NEGEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(button_Pin, button_isr_handler, NULL);
}