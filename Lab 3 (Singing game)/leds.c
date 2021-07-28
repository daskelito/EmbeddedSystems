#include "leds.h"
#include "driver/gpio.h"
#include "driver/pcnt.h"

#define PIN1 12
#define PIN2 13

//Initiates the pins to map them to 12 and 13
void initPins()
{
    gpio_pad_select_gpio(PIN1);
    gpio_pad_select_gpio(PIN2);
    gpio_set_direction(PIN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN2, GPIO_MODE_OUTPUT);
}

//Sets the LED to on or off
void setLEDA(uint8_t level)
{
    if (level)
    {
        gpio_set_level(PIN1, 1);
    }
    else
    {
        gpio_set_level(PIN1, 0);
    }
}

//Sets the LED to on or off
void setLEDB(uint8_t level)
{
    if (level)
    {
        gpio_set_level(PIN2, 1);
    }
    else
    {
        gpio_set_level(PIN2, 0);
    }
}