#include <math.h>
#include <driver/gpio.h>
#include "circular_buffer.h"
#include "ISR.h"
#include <esp_int_wdt.h>
#include "accel.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#define MIN_SD 100

//Algorithm period of 1200 in order to allow for another step to be taken, + some marigin
// but not to high in order to save resources
#define ALGO_PERIOD 1200

//A average runner takes 3 steps per second, doubled it to 6 (nyquist) and rounded
//to 10 to be on the safe side
#define SAMPLING_PERIOD 10

// 3 steps a second at a maximum gives a minimum of 333 ms between steps, 300 after rounding
#define MIN_INTRA_STEP_TIME 300

//from testing I got this K value to make for the best results
#define K 2.3

//step goal
#define step_goal 50

//pin for leds
#define LEDPIN 27

struct circularBuffer cBuffer;
int step_count;
int led_status = 0;
SemaphoreHandle_t xSemaphore;

//gets the values from the accelerometer
void readAccel(void *arg)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        axisValues();
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SAMPLING_PERIOD));
    }
}

//Calculating the steps, if a measurement is not big enough, do not count it as a step, using the mean + k * sd as reference
void stepCalculator(void *arg)
{
    int32_t sd;
    int32_t mean;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        uint32_t numberBufferValues = nbrOfValues(&cBuffer);
        uint32_t sum = sumOfBuffer(&cBuffer);
        if (numberBufferValues > 0)
        {
            mean = sum / numberBufferValues;
            uint32_t tempval1 = sumOfBufferAllButOne(&cBuffer, mean);
            sd = (uint32_t)sqrt((double)(tempval1 / numberBufferValues));
            if (sd < MIN_SD)
                sd = MIN_SD;
            int lastStepTS = 0;
            for (int i = 0; i < nbrOfValues(&cBuffer); i++)
            {
                if ((double)removeHead(&cBuffer) > ((K * sd) + mean))
                {
                    if ((i - lastStepTS) * (SAMPLING_PERIOD) > MIN_INTRA_STEP_TIME)
                    {
                        step_count++;
                        lastStepTS = i;
                    }
                }
            }

            printf("Steps: %d\n", step_count);
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(ALGO_PERIOD));
    }
}

//samaphore for the button.
void button_isr_handler(void *arg)
{
    xSemaphoreGiveFromISR(xSemaphore, NULL);
}

//handles the turning on and off of the led based on if the step goal is reached.
void led_handler(void *arg)
{
    while (1)
    {
        if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
        {
            if (step_count > step_goal)
            {
                led_status = !led_status;
                gpio_set_level(LEDPIN, led_status);
            }
        }
    }
}