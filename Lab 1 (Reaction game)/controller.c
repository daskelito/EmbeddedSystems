#include "pins.h"
#include "controller.h"
#include <esp_task_wdt.h>

void waitMs(unsigned int milllis)
{
    TickType_t delay = milllis / portTICK_PERIOD_MS;
    vTaskDelay(delay);
}

void showWinnerA()
{
     setLEDB(0);

    setLEDA(1);
    waitMs(100);
    setLEDA(0);
    waitMs(100);

    setLEDA(1);
    waitMs(100);
    setLEDA(0);
    waitMs(100);

    setLEDA(1);
    waitMs(100);
    setLEDA(0);
    waitMs(100);

    setLEDA(1);
    waitMs(100);
    setLEDA(0);
    waitMs(100);
    
    setLEDA(1);
    waitMs(100);
    setLEDA(0);
}

void showWinnerB()
{
    setLEDA(0);

    setLEDB(1);
    waitMs(100);
    setLEDB(0);
    waitMs(100);

    setLEDB(1);
    waitMs(100);
    setLEDB(0);
    waitMs(100);

    setLEDB(1);
    waitMs(100);
    setLEDB(0);
    waitMs(100);

    setLEDB(1);
    waitMs(100);
    setLEDB(0);
    waitMs(100);

    setLEDB(1);
    waitMs(100);
    setLEDB(0);
    
}