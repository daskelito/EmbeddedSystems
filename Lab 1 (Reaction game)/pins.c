#include "pins.h"
#include "driver/gpio.h"

/* initialises the 4 pins */
#define PIN1 12
#define PIN2 14
#define BT1 27
#define BT2 26

void initPins()
{
    gpio_pad_select_gpio(PIN1);
    gpio_pad_select_gpio(PIN2);
    gpio_pad_select_gpio(BT1);
    gpio_pad_select_gpio(BT2);
    gpio_set_direction(PIN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN2, GPIO_MODE_OUTPUT);
    gpio_set_direction(BT1, GPIO_MODE_INPUT);
    gpio_set_direction(BT2, GPIO_MODE_INPUT);
    gpio_pullup_en(BT1);
    gpio_pullup_en(BT2);
}

/* switches LED A on if level!=0 or off if level==0*/
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

/* switches LED B on if level!=0 or off if level==0*/
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

/* tells if button A is currently being pressed */
uint8_t isButtonAPressed()
{
    return !gpio_get_level(BT1);
}
/* tells if button A is currently being pressed */
uint8_t isButtonBPressed()
{
    return !gpio_get_level(BT2);
}
