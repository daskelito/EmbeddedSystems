#include <stdio.h>
#include <stdlib.h>
#include "esp32/rom/ets_sys.h"
#include <esp_task_wdt.h>
#include <driver/gpio.h>
#include "double_linked_list.h"

//Pins used for leds and button
#define LED_PIN_LEVEL_UP 12
#define LED_PIN_LEVEL_MIDDLE 14
#define LED_PIN_LEVEL_DOWN 27
#define BUTTON_PIN 26

//time between button pushes that is acceptable
#define PUSH_TIME_US 500000 // 500 ms

//Max value of number of passangers
#define NUMBER_OF_PASSANGERS 50

//function pointer
void (*lvl)();

//stores the level we are going to. Origin and destination
int getLevel;

//stores what level you are on. Initiated to 0 as you start on level 0
int currentLvl = 0;

//boolean for checking if you are on the same floor. Travel time == 0
int sameFloor = 0;

//boolean for when there are no passangees left for traveling.
int noPassangersLeft = 0;

//used for calculating time spent traveling and also loading/unloading passangers.
int timerStart;
int timerStop;
int totalTime;

//used for setting how many passangers there are. Used in test cases
int numberOfPassangers;

// Used to represent a travel need of a passenger.
struct travel_need
{
    int origin;
    int destination;
};

// Used to not allow button pushes that are too close to each other in time
static volatile uint64_t lastPush = -PUSH_TIME_US;

// This data structure holds information about every passangers travel needs, origin and destination
static volatile struct travel_need travel_needs[NUMBER_OF_PASSANGERS];

struct doubleLinkedList pressingButton;
struct doubleLinkedList elevatorQueue;

void testZero();
void testOne();
void testTwo();
void testThree();
void testFour();
void testFive();

//states used
void lvl0();
void lvl1();
void lvl2();
void idle();
void moving();

/* 
* Configure pin LED_PIN_LEVEL_UP as output
* Configure pin LED_PIN_LEVEL_MIDDLE as output
* Configure pin LED_PIN_LEVEL_DOWN as output
*/
void initPins()
{
    gpio_pad_select_gpio(LED_PIN_LEVEL_DOWN);
    gpio_set_direction(LED_PIN_LEVEL_DOWN, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(LED_PIN_LEVEL_MIDDLE);
    gpio_set_direction(LED_PIN_LEVEL_MIDDLE, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(LED_PIN_LEVEL_UP);
    gpio_set_direction(LED_PIN_LEVEL_UP, GPIO_MODE_OUTPUT);
}

//Stores the time, used at the start
void startTime()
{
    timerStart = esp_timer_get_time() / 1000000;
}

//Stores the time, used at the end
void endTime()
{
    timerStop = esp_timer_get_time() / 1000000;
}

//Prints the load/unload time
void calcTimeUnload()
{
    totalTime = timerStop - timerStart;
    printf("Time elapsed load/unload: %d sec\n", totalTime);
}

//Prints the travel time
void calcTimeTravel()
{
    totalTime = timerStop - timerStart;
    printf("Time elapsed travel: %d sec\n", totalTime);
}

//Absolute value function
int absval(int a)
{
    if (a < 0)
        return -a;
    else
        return a;
}

//Function for lower lvl. Turn on lower lvl LED.
//If arriving to lower lvl, wait 2 sec unless already there
void lvl0()
{
    startTime();
    printf("lower lvl\n");
    gpio_set_level(LED_PIN_LEVEL_DOWN, 1);
    if (!sameFloor)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
    endTime();
    calcTimeUnload();
}

//Function for middle lvl. Turn on middle lvl LED.
//If arriving to middle lvl, wait 2 sec unless already there
void lvl1()
{
    startTime();
    printf("middle lvl\n");
    gpio_set_level(LED_PIN_LEVEL_MIDDLE, 1);
    if (!sameFloor)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
    endTime();
    calcTimeUnload();
}

//Function for upper lvl. Turn on middle lvl LED.
//If arriving to upper lvl, wait 2 sec unless already there
void lvl2()
{
    startTime();
    printf("upper lvl\n");
    gpio_set_level(LED_PIN_LEVEL_UP, 1);
    if (!sameFloor)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
    endTime();
    calcTimeUnload();
}

//Function for idle state
void idle()
{
    while (1)
    {
        //If the passanger list is empty the boolean noPassangersLeft will be true and the program will loop:
        while (noPassangersLeft)
        {
            //Checks if there is any new passanger in the doubleLinkedList
            if (pressingButton.first != NULL)
            {
                noPassangersLeft = 0;
                currentLvl = 0;
            }
            vTaskDelay(pdMS_TO_TICKS(250));
        }

        //Checks if there is any passangers thats pressed the button and are waitning for the elevator
        if (elevatorQueue.first != NULL)
        {
            lvl = moving;
            lvl();

            if (getLevel == 0)
            {
                lvl = lvl0;
            }
            if (getLevel == 1)
            {
                lvl = lvl1;
            }
            if (getLevel == 2)
            {
                lvl = lvl2;
            }
            //If the queue is empty it returns INT_MIN and the elevator goes to lower lvl
            if (getLevel == INT_MIN)
            {
                lvl = lvl0;
            }

            lvl();
            sameFloor = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

//Handles the movement of the elevator calculating the traveling time
void moving()
{
    startTime();
    //All leds turned of while the elevator is moving
    gpio_set_level(LED_PIN_LEVEL_DOWN, 0);
    gpio_set_level(LED_PIN_LEVEL_MIDDLE, 0);
    gpio_set_level(LED_PIN_LEVEL_UP, 0);

    //Getting the next destination in the queue
    getLevel = removeFirstElementDoubleLinkedList(&elevatorQueue);

    //calculating how many levels the elevator is travelling
    int timeMultiplier = getLevel - currentLvl;
    timeMultiplier = absval(timeMultiplier);

    //If there is no travel needs noPassangers becomes true
    if (getLevel == INT_MIN)
    {
        timeMultiplier = currentLvl;
        noPassangersLeft = 1;
    }

    //When the next destination is the same as the last one.
    if (timeMultiplier == 0)
        sameFloor = 1;

    //Takes 5 sec between every lvl, multiplice by lvls traveld
    vTaskDelay(pdMS_TO_TICKS(timeMultiplier * 2000));
    endTime();
    calcTimeTravel();
    currentLvl = getLevel;
}

// This function is called when button is pushed in order to get around bouncing
static void handle_push(void *arg)
{
    // Disable interrupts
    gpio_intr_disable(BUTTON_PIN);

    // Get the current time
    uint64_t now = esp_timer_get_time();

    //If enough time passed, we should consider this event as a genuine push
    if ((now - lastPush) > PUSH_TIME_US)
    {
        //Moves both the origin and destination from passengerlist to the queue
        //Every time the button is pushed moves a travellers needs from the traveler list to the elevatorqueue list
        for (int i = 0; i < 2; i++)
        {
            int value = removeFirstElementDoubleLinkedList(&pressingButton);
            addElementDoubleLinkedList(&elevatorQueue, value);
        }
        lastPush = now;
    }

    // Re-enable interrupts
    gpio_intr_enable(BUTTON_PIN);
}

void app_main()
{
    initPins();
    initDoubleLinkedList(&pressingButton);
    initDoubleLinkedList(&elevatorQueue);

    /*
    * Run one test at the time
    */

    testZero();
    //testOne();
    //testTwo();
    //testThree();
    //testFour();
    //testFive();

    //Add passangers travel_needs to a list
    for (int i = 0; i < numberOfPassangers; i++)
    {
        addElementDoubleLinkedList(&pressingButton, travel_needs[i].origin);
        addElementDoubleLinkedList(&pressingButton, travel_needs[i].destination);
    }

    //Configure pin BUTTON_PIN as input, pull up and with interrupts on the negative edge
    gpio_config_t config;
    config.pin_bit_mask = (u_int64_t)1 << BUTTON_PIN;
    config.mode = GPIO_MODE_INPUT;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.pull_up_en = GPIO_PULLUP_ENABLE;
    config.intr_type = GPIO_INTR_NEGEDGE;
    esp_err_t res = gpio_config(&config);

    // Activate the interrupts for the GPIOs
    res = gpio_install_isr_service(0);
    if (res != ESP_OK)
    {
        printf("Cannot activate interrupts: %s\n", esp_err_to_name(res));
    }
    // Add a handler to the ISR for pin BUTTON_PIN
    res = gpio_isr_handler_add(BUTTON_PIN, handle_push, NULL);
    if (res != ESP_OK)
    {
        printf("Cannot activate interrupts: %s\n", esp_err_to_name(res));
    }

    //Turns on the led, our elevator starts its journey on the first floor (floor 0)
    gpio_set_level(LED_PIN_LEVEL_DOWN, 1);

    //Starts in the idle state
    lvl = idle;
    lvl();
}

//tests 50 different passangers with origin and destination

void testZero(){
    numberOfPassangers = 3;
    travel_needs[0].origin = 1;
    travel_needs[0].destination = 2;
    travel_needs[1].origin = 1;
    travel_needs[1].destination = 2;
    travel_needs[2].origin = 1;
    travel_needs[2].destination = 2;

}

void testOne()
{
    numberOfPassangers = 50;

    //Initialize travel needs (50 randomly generated travel needs)
    travel_needs[0].origin = 2;
    travel_needs[0].destination = 1;
    travel_needs[1].origin = 1;
    travel_needs[1].destination = 2;
    travel_needs[2].origin = 1;
    travel_needs[2].destination = 2;
    travel_needs[3].origin = 0;
    travel_needs[3].destination = 2;
    travel_needs[4].origin = 2;
    travel_needs[4].destination = 1;
    travel_needs[5].origin = 0;
    travel_needs[5].destination = 2;
    travel_needs[6].origin = 1;
    travel_needs[6].destination = 2;
    travel_needs[7].origin = 1;
    travel_needs[7].destination = 0;
    travel_needs[8].origin = 0;
    travel_needs[8].destination = 1;
    travel_needs[9].origin = 1;
    travel_needs[9].destination = 0;
    travel_needs[10].origin = 1;
    travel_needs[10].destination = 2;
    travel_needs[11].origin = 0;
    travel_needs[11].destination = 1;
    travel_needs[12].origin = 0;
    travel_needs[12].destination = 2;
    travel_needs[13].origin = 0;
    travel_needs[13].destination = 1;
    travel_needs[14].origin = 0;
    travel_needs[14].destination = 2;
    travel_needs[15].origin = 0;
    travel_needs[15].destination = 1;
    travel_needs[16].origin = 2;
    travel_needs[16].destination = 1;
    travel_needs[17].origin = 2;
    travel_needs[17].destination = 1;
    travel_needs[18].origin = 1;
    travel_needs[18].destination = 0;
    travel_needs[19].origin = 2;
    travel_needs[19].destination = 1;
    travel_needs[20].origin = 1;
    travel_needs[20].destination = 0;
    travel_needs[21].origin = 0;
    travel_needs[21].destination = 1;
    travel_needs[22].origin = 1;
    travel_needs[22].destination = 2;
    travel_needs[23].origin = 0;
    travel_needs[23].destination = 2;
    travel_needs[24].origin = 2;
    travel_needs[24].destination = 1;
    travel_needs[25].origin = 1;
    travel_needs[25].destination = 0;
    travel_needs[26].origin = 1;
    travel_needs[26].destination = 2;
    travel_needs[27].origin = 0;
    travel_needs[27].destination = 2;
    travel_needs[28].origin = 1;
    travel_needs[28].destination = 0;
    travel_needs[29].origin = 1;
    travel_needs[29].destination = 2;
    travel_needs[30].origin = 0;
    travel_needs[30].destination = 1;
    travel_needs[31].origin = 1;
    travel_needs[31].destination = 2;
    travel_needs[32].origin = 0;
    travel_needs[32].destination = 2;
    travel_needs[33].origin = 0;
    travel_needs[33].destination = 2;
    travel_needs[34].origin = 1;
    travel_needs[34].destination = 2;
    travel_needs[35].origin = 2;
    travel_needs[35].destination = 1;
    travel_needs[36].origin = 0;
    travel_needs[36].destination = 2;
    travel_needs[37].origin = 1;
    travel_needs[37].destination = 0;
    travel_needs[38].origin = 0;
    travel_needs[38].destination = 2;
    travel_needs[39].origin = 2;
    travel_needs[39].destination = 1;
    travel_needs[40].origin = 0;
    travel_needs[40].destination = 1;
    travel_needs[41].origin = 0;
    travel_needs[41].destination = 1;
    travel_needs[42].origin = 0;
    travel_needs[42].destination = 1;
    travel_needs[43].origin = 1;
    travel_needs[43].destination = 0;
    travel_needs[44].origin = 0;
    travel_needs[44].destination = 2;
    travel_needs[45].origin = 2;
    travel_needs[45].destination = 1;
    travel_needs[46].origin = 2;
    travel_needs[46].destination = 1;
    travel_needs[47].origin = 2;
    travel_needs[47].destination = 1;
    travel_needs[48].origin = 0;
    travel_needs[48].destination = 2;
    travel_needs[49].origin = 1;
    travel_needs[49].destination = 0;
    
}

//tests with no travel_needs/no passangers
void testTwo()
{
    numberOfPassangers = 0;
}

//tests where the index in travel_needs are higher than the value set on numberOfPassangers.
void testThree()
{
    numberOfPassangers = 3;
    travel_needs[18].origin = 1;
    travel_needs[18].destination = 0;
    travel_needs[19].origin = 2;
    travel_needs[19].destination = 1;
    travel_needs[20].origin = 1;
    travel_needs[20].destination = 0;
}

//tests with negative values for origin and destination.
void testFour()
{
    numberOfPassangers = 3;
    travel_needs[0].origin = 1;
    travel_needs[0].destination = 0;
    travel_needs[1].origin = -2;
    travel_needs[1].destination = 1;
    travel_needs[2].origin = -1;
    travel_needs[2].destination = 0;
}

//shorter test than test one.
void testFive()
{
    numberOfPassangers = 3;
    travel_needs[0].origin = 1;
    travel_needs[0].destination = 0;
    travel_needs[1].origin = 2;
    travel_needs[1].destination = 1;
    travel_needs[2].origin = 1;
    travel_needs[2].destination = 0;
}