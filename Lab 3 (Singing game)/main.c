#include <stdio.h>
#include "driver/dac.h"
#include <esp_task_wdt.h>
#include "driver/adc.h"
#include "soc/syscon_reg.h"
#include "soundgen.h"
#include "sampler.h"
#include "leds.h"
#include "flash.h"
#include "random.h"

float playerFreq;
int radomFreq;
int actualFreq;

//Eliminates negative values and turns them positive
float absVal(float a)
{
    if (a < 0)
    {
        return -a;
    }
    else
    {
        return a;
    }
}

void app_main()
{
    initPins();
    while (1)
    {
        startFlash(300, 2);
        //gets a random number between 200 and 1000;
        radomFreq = getRandomFreq(300, 1000);

        //plays a sound with a frequency that is the same as the random number
        actualFreq = startSound(radomFreq);

        vTaskDelay(pdMS_TO_TICKS(1000));
        //stops playing the sound
        stopSound();
        //samples the input to the microphone
        startSampling(4000);

        vTaskDelay(pdMS_TO_TICKS(2000));
        stopSampling();
        
        //averages the player's frequency based on the input over time
        playerFreq = getFrequency();

        printf("Players sound: %f, Actual sound: %d\n", playerFreq, actualFreq);
        // if you are within the range of 50 Hz from the played sound you win
        if (absVal(playerFreq - (float)actualFreq) < 50)
        {
            printf("Player win \n");
            winFlash();
        }
        else
        {
            printf("Player lose! \n");
            lostFlash();
        }
    }
}