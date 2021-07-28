#include "random.h"
#include "esp_system.h"


//randoms a value between min and max using esp_random()
int getRandomFreq(int min, int max){

    float r = esp_random();

    //result here will be between 0 and 1
    r = r / UINT32_MAX;

    //result here will be within the range from min to max
    r = min + r*(max - min);

    return r;
}