#include "random.h"
#include "esp_system.h"

int getRandomsecs(int min, int max){
    float r = esp_random();
    r = r / UINT32_MAX;
    r = min + r*(max-min);
    return r;
}