#include <esp_task_wdt.h>
#include "pins.h"
#include "random.h"
#include "controller.h"


void app_main()
{
    initPins();
    bool scored = false;
    while (1)
    {
        // Game is about to start behavior:
        setLEDA(1);
        setLEDB(1);
        waitMs(300);
        setLEDA(0);
        setLEDB(0);
        waitMs(300);
        setLEDA(1);
        setLEDB(1);
        waitMs(300);
        setLEDA(0);
        setLEDB(0);
        waitMs(300);
        setLEDA(1);
        setLEDB(1);
        waitMs(300);
        setLEDA(0);
        setLEDB(0);

        //Wait random time
        waitMs(getRandomsecs(3000, 6000));
        setLEDA(1);
        setLEDB(1);

        //Game loop, shows the winner by flashing the corresponding LED 
        while(!scored){
            if(isButtonAPressed()){
                showWinnerA();
                scored = true;
            }
            if (isButtonBPressed()){
                showWinnerB();
                scored = true;
            }
        }
        waitMs(1000);
        scored = false;
    }
}
