#include "flash.h"
#include "leds.h"
#include <esp_task_wdt.h>

// Waits millis miliseconds
void waitMs(unsigned int millis)
{

    TickType_t delay = millis / portTICK_PERIOD_MS;
    vTaskDelay(delay);
}

/* The lights flashes in a pattern when the game starts/restarts */
void startFlash(int ms, int laps)
{
    for (int i = 0; i <= laps; i++)
    {
        setLEDA(1);
        setLEDB(1);
        waitMs(ms);
        setLEDA(0);
        setLEDB(0);
        waitMs(ms);
        setLEDA(1);
        setLEDB(1);
        waitMs(ms);
        setLEDA(0);
        setLEDB(0);
        waitMs(ms);
        setLEDA(1);
        setLEDB(1);
        setLEDA(0);
        setLEDB(0);
    }
}

//Flash routine for restarting

void restart(unsigned int restartTime)
{

    setLEDA(0);
    setLEDB(0);

    waitMs(restartTime);
}

// Flash routine for losing
void lostFlash()
{
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

// Flash routine for winning
void winFlash()
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