#include <stdio.h>
#include <stdlib.h>
#include <esp32/rom/ets_sys.h>
#include "circular_buffer.h"
#include <limits.h>

/*
 * Change the value of BUFFER_SIZE if you want to change the size of the buffer. 
 */
#define BUFFER_SIZE 4

struct circularBuffer buffer;
void BBtest1();
void BBtest2();
void BBtest3();
void BBtest4();
void BBtest5();
void BBtest6();
void BBtest7();
void BBtest8();
void BBtest9();
void WBtestInitCircularBuffer();
void WBtestAddElement1();
void WBtestAddElement2();
void WBtestAddElement3();
void WBtestAddElement4();
void WBtestRemoveValue1();
void WBtestRemoveValue2();
void WBtestRemoveValue3();
void WBtestRemoveHead1();
void WBtestRemoveHead2();
void WBtestRemoveHead3();
void WBtestRemoveHead4();

void app_main()
{

    // Some code to help you get started

    int *buffer_data = (int *)malloc(BUFFER_SIZE * sizeof(int));
    initCircularBuffer(&buffer, buffer_data, BUFFER_SIZE);
    
    BBtest1();
    BBtest2();
    BBtest3();
    BBtest4();
    BBtest5();
    BBtest6();
    BBtest7();
    BBtest8();
    BBtest9();
    WBtestInitCircularBuffer();
    WBtestAddElement1();
    WBtestAddElement2();
    WBtestAddElement3();
    WBtestAddElement4();
    WBtestRemoveValue1();
    WBtestRemoveValue2();
    WBtestRemoveValue3();
    WBtestRemoveHead1();
    WBtestRemoveHead2();
    WBtestRemoveHead3();
    WBtestRemoveHead4();

    free(buffer_data);
}

void WBtestInitCircularBuffer()
{
    printf("\nWhite box test: InitCircularBuffer: \n");
    printf("\n");
    printf("Buffer size: %d\n", BUFFER_SIZE);
    printf("Buffer maxLength: %d\n", buffer.maxLength);
    printf("Buffer head: %d\n", buffer.head);
    printf("Buffer tail: %d\n", buffer.tail);
}
void WBtestAddElement1()
{
    printf("\nWhite box test: Add element (1) \n");
    printf("\n");
    resetBuffer(&buffer);
    addElement(&buffer, 1);
    printBuffer(&buffer);
}
void WBtestAddElement2()
{
    printf("\nWhite box test: Add element (2) \n");
    printf("\n");
    resetBuffer(&buffer);
    buffer.head = 1;
    buffer.tail = 1;
    addElement(&buffer, 1);
    printBuffer(&buffer);
}
void WBtestAddElement3()
{
    printf("\nWhite box test: Add element (3) \n");
    printf("\n");
    resetBuffer(&buffer);
    buffer.data[buffer.head] = 3;
    buffer.empty = 0;
    addElement(&buffer, 10);
    printBuffer(&buffer);

    resetBuffer(&buffer);
    buffer.head = 3;
    buffer.tail = 3;
    buffer.data[buffer.head] = 7;
    buffer.empty = 0;
    addElement(&buffer, 10);
    printBuffer(&buffer);
}
void WBtestAddElement4()
{
    printf("\nWhite box test: Add element (4) \n");
    printf("\n");
    resetBuffer(&buffer);
    buffer.data[0] = 11;
    buffer.data[1] = 11;
    buffer.data[2] = 11;
    buffer.empty = 0;
    buffer.tail = 0;
    buffer.head = 2;
    addElement(&buffer, 20);
    printBuffer(&buffer);
}

void WBtestRemoveValue1()
{
    printf("\nWhite box test: Remove value (1) \n");
    printf("\n");
    resetBuffer(&buffer);

    buffer.data[0] = 5;
    buffer.empty = 0;
    printBuffer(&buffer);
    printf("Removing value: 5\n");
    removeValue(&buffer, 5);
    printBuffer(&buffer);

    resetBuffer(&buffer);
    buffer.data[3] = 5;
    buffer.head = 3;
    buffer.tail = 3;
    buffer.empty = 0;
    printBuffer(&buffer);
    printf("Removing value: 5\n");
    removeValue(&buffer, 5);
    printBuffer(&buffer);
}
void WBtestRemoveValue2()
{
    printf("\nWhite box test: Remove value (2) \n");
    printf("\n");
    resetBuffer(&buffer);

    addElement(&buffer, 1);
    addElement(&buffer, 2);
    addElement(&buffer, 3);
    addElement(&buffer, 4);

    printBuffer(&buffer);
    removeValue(&buffer, 1);
    printBuffer(&buffer);

    resetBuffer(&buffer);

    buffer.data[0] = 4;
    buffer.data[1] = 6;
    buffer.data[2] = 7;
    buffer.data[3] = 11;
    buffer.head = 3;
    buffer.tail = 0;
    buffer.empty = 0;
    printBuffer(&buffer);
    removeValue(&buffer, 4);
    printBuffer(&buffer);
}
void WBtestRemoveValue3()
{
    printf("\nWhite box test: Remove value (3) \n");
    printf("\n");
    resetBuffer(&buffer);
    int val = removeValue(&buffer, 55);
    if (val == INT_MIN)
    {
        printf("Nothing removed since buffer is empty. (Return value from function = %d)\n", INT_MIN);
    }
}

void WBtestRemoveHead1()
{
    printf("\nWhite box test: Remove head (1) \n");
    printf("\n");
    resetBuffer(&buffer);

    addElement(&buffer, 30);
    printBuffer(&buffer);
    removeHead(&buffer);
    printBuffer(&buffer);

    resetBuffer(&buffer);
    buffer.data[3] = 45;
    buffer.head = 3;
    buffer.tail = 3;
    buffer.empty = 0;
    printBuffer(&buffer);
    removeHead(&buffer);
    printBuffer(&buffer);
}
void WBtestRemoveHead2()
{
    printf("\nWhite box test: Remove head (2) \n");
    printf("\n");

    resetBuffer(&buffer);
    buffer.data[3] = 45;
    buffer.data[2] = 55;
    buffer.head = 3;
    buffer.tail = 2;
    buffer.empty = 0;
    printBuffer(&buffer);
    removeHead(&buffer);
    printBuffer(&buffer);

    resetBuffer(&buffer);
    buffer.data[0] = 70;
    buffer.data[3] = 90;
    buffer.head = 0;
    buffer.tail = 3;
    buffer.empty = 0;
    printBuffer(&buffer);
    removeHead(&buffer);
    printBuffer(&buffer);
}
void WBtestRemoveHead3()
{
    printf("\nWhite box test: Remove head (3) \n");
    printf("\n");
    resetBuffer(&buffer);

    buffer.data[0] = 60;
    buffer.data[2] = 55;
    buffer.data[3] = 45;
    buffer.head = 0;
    buffer.tail = 2;
    buffer.empty = 0;
    printBuffer(&buffer);
    removeHead(&buffer);
    printBuffer(&buffer);

    resetBuffer(&buffer);
    buffer.data[0] = 70;
    buffer.data[1] = 100;
    buffer.data[3] = 90;
    buffer.head = 1;
    buffer.tail = 3;
    buffer.empty = 0;
    printBuffer(&buffer);
    removeHead(&buffer);
    printBuffer(&buffer);
}
void WBtestRemoveHead4()
{
    printf("\nWhite box test: Remove head (4) \n");
    printf("\n");
    resetBuffer(&buffer);
    int val = removeHead(&buffer);
    if (val == INT_MIN)
    {
        printf("Nothing removed since buffer is empty. (Return value from function = %d)\n", INT_MIN);
    }
    printBuffer(&buffer);
}

void BBtest1()
{
    resetBuffer(&buffer);
    printf("\nBlack box test 1: \n");
    printf("\n");

    printf("Adding value: 3 \n");
    int val1 = addElement(&buffer, 3);
    printBuffer(&buffer);

    printf("Removing head \n");
    int val2 = removeHead(&buffer);
    printBuffer(&buffer);

    if (val1 == 3 && val2 == 3)
    {
        printf("Test passed! \n");
    }
    else
    {
        printf("Test failed: values are not the same! \n");
    }
}
void BBtest2()
{
    resetBuffer(&buffer);
    printf("\nBlack box test 2: \n");
    printf("\n");

    printf("Adding value: 1 \n");
    addElement(&buffer, 1);
    printBuffer(&buffer);

    printf("Adding value: 2 \n");
    addElement(&buffer, 2);
    printBuffer(&buffer);

    printf("Removing value: 1 \n");
    removeValue(&buffer, 1);
    printBuffer(&buffer);

    printf("Removing value: 2 \n");
    removeValue(&buffer, 2);
    printBuffer(&buffer);
}
void BBtest3()
{
    resetBuffer(&buffer);
    printf("\nBlack box test 3: \n");
    printf("\n");

    printf("Adding values: 1, 2, 3, 4 \n");
    addElement(&buffer, 1);
    addElement(&buffer, 2);
    addElement(&buffer, 3);
    addElement(&buffer, 4);
    printBuffer(&buffer);

    printf("Removing value: 1 \n");
    removeValue(&buffer, 1);
    printBuffer(&buffer);

    printf("Removing value: 2 \n");
    removeValue(&buffer, 2);
    printBuffer(&buffer);

    printf("Removing value: 3 \n");
    removeValue(&buffer, 3);
    printBuffer(&buffer);

    printf("Removing value: 4 \n");
    removeValue(&buffer, 4);
    printBuffer(&buffer);
}
void BBtest4()
{
    resetBuffer(&buffer);
    printf("\nBlack box test 4: \n");
    printf("\n");
    printf("Adding values: 1, 2, 3, 4, 5 \n");
    addElement(&buffer, 1);
    addElement(&buffer, 2);
    addElement(&buffer, 3);
    addElement(&buffer, 4);
    addElement(&buffer, 5);
    printBuffer(&buffer);
}
void BBtest5()
{
    resetBuffer(&buffer);
    printf("\nBlack box test 5: \n");
    printf("\n");

    for (int i = 1; i < BUFFER_SIZE + 2; i++)
    {
        printf("Adding value: %d \n", i);
        int val1 = addElement(&buffer, i);
        printBuffer(&buffer);
        printf("Removing head \n");
        int val2 = removeHead(&buffer);
        printf("Val1 is %d\n", val1);
        printf("Val2 is %d\n", val2);
        printf("i is %d \n", i);
        if (val1 != i || val2 != i)
        {
            printf("Test failed! Removed value is not the same as the added value.");
        }
        printBuffer(&buffer);
    }
}
void BBtest6()
{
    resetBuffer(&buffer);
    printf("\nBlack box test 6: \n");
    printf("\n");

    int val = contains(&buffer, 6);
    if (val == INT_MIN)
    {
        printf("Test passed!\n");
    }
    else
    {
        printf("Test failed!\n");
    }
}
void BBtest7()
{
    resetBuffer(&buffer);
    printf("\nBlack box test 7: \n");
    printf("\n");
    addElement(&buffer, 1);
    printBuffer(&buffer);
}
void BBtest8()
{
    resetBuffer(&buffer);
    printf("\nBlack box test 8: \n");
    printf("\n");

    addElement(&buffer, 1);
    addElement(&buffer, 2);
    printBuffer(&buffer);
}
void BBtest9()
{
    resetBuffer(&buffer);
    printf("\nBlack box test 3: \n");
    printf("\n");

    printf("Adding values: 11, 244, 31, 7 \n");
    addElement(&buffer, 11);
    addElement(&buffer, 244);
    addElement(&buffer, 31);
    addElement(&buffer, 7);
    printBuffer(&buffer);
}