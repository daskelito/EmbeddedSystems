#include <driver/i2c.h>
#include <stdio.h>
#include <math.h>
#include <esp_pm.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <driver/gpio.h>
#include <esp_int_wdt.h>
#include "circular_buffer.h"
#include "ISR.h"
#include "accel.h"
#include "pins.h"


//buffer size for storage of values from accelerometer
#define BUFFER_SIZE 1000

//priority of the different tasks
#define SAMPLING_PRIORITY 1
#define ALGO_PRIORITY 2
#define LED_PRIORITY 3

struct circularBuffer cBuffer;
SemaphoreHandle_t xSemaphore = NULL;

void sleepMode();

void app_main()
{
    int *buffer_data = (int *)malloc(BUFFER_SIZE * sizeof(uint32_t));
    initCircularBuffer(&cBuffer, buffer_data, BUFFER_SIZE);
    
    sleepMode();

    initI2C(33, 32);
    initLEDandButton();

    xSemaphore = xSemaphoreCreateBinary();

    xTaskCreate(readAccel, "change_in_acceleration", 2048, NULL, SAMPLING_PRIORITY, NULL);
    xTaskCreate(stepCalculator, "check_steps", 2048, NULL, ALGO_PRIORITY, NULL);
    xTaskCreate(led_handler, "step_goal_reached", 2048, NULL, LED_PRIORITY, NULL);
}


//configuration for power saving/sleep mode
void sleepMode()
{
    esp_pm_config_esp32_t conf_sleep_light_mode = {
        .light_sleep_enable = true,
        .max_freq_mhz = 80,
        .min_freq_mhz = 13};
    esp_err_t error = esp_pm_configure(&conf_sleep_light_mode);
    ESP_ERROR_CHECK(error);
}