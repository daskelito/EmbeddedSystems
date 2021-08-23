#include <stdio.h>
#include <stdlib.h>

#include "single_linked_list.h"
#include "double_linked_list.h"

struct singleLinkedList singleLinkedList;
struct doubleLinkedList doubleLinkedList;

//Tests for single linked list
void SLLBBtest1();
void SLLBBtest2();
void SLLBBtest3();
void SLLBBtest4();
void SLLWBtest1();
void SLLWBtest2();
void SLLWBtest3();
void SLLWBtest4();

//Tests for double linked list
void DLLBBtest1();
void DLLBBtest2();
void DLLBBtest3();
void DLLBBtest4();
void DLLWBtest1();
void DLLWBtest2();
void DLLWBtest3();
void DLLWBtest4();

void app_main()
{
    initSingleLinkedList(&singleLinkedList);
    SLLBBtest1();
    SLLBBtest2();
    SLLBBtest3();
    SLLBBtest4();
    SLLWBtest1();
    SLLWBtest2();
    SLLWBtest3();
    SLLWBtest4();

    initDoubleLinkedList(&doubleLinkedList);
    DLLBBtest1();
    DLLBBtest2();
    DLLBBtest3();
    DLLBBtest4();
    DLLWBtest1();
    DLLWBtest2();
    DLLWBtest3();
    DLLWBtest4();
}

//Adding elements in non-ascending/descending order to test add and sort function
void SLLBBtest1()
{
    printf("Single Linked List: Black box test 1\n");

    addElementSingleLinkedList(&singleLinkedList, 42);
    addElementSingleLinkedList(&singleLinkedList, 110);
    addElementSingleLinkedList(&singleLinkedList, 0);

    printSingleLinkedList(&singleLinkedList);
    resetSingleLinkedList(&singleLinkedList);
}

//testing of removeFirstElementSingleLinkedList
void SLLBBtest2()
{
    printf("Single Linked List: Black box test 2\n");

    addElementSingleLinkedList(&singleLinkedList, 55);
    addElementSingleLinkedList(&singleLinkedList, 11);
    printSingleLinkedList(&singleLinkedList);
    removeFirstElementSingleLinkedList(&singleLinkedList);

    printSingleLinkedList(&singleLinkedList);
    resetSingleLinkedList(&singleLinkedList);
}

//testing of removeLastElementSingleLinkedList
void SLLBBtest3()
{
    printf("Single Linked List: Black box test 3\n");

    addElementSingleLinkedList(&singleLinkedList, 42);
    addElementSingleLinkedList(&singleLinkedList, 951);
    printSingleLinkedList(&singleLinkedList);
    removeLastElementSingleLinkedList(&singleLinkedList);

    printSingleLinkedList(&singleLinkedList);
    resetSingleLinkedList(&singleLinkedList);
}

//testing of removal of element in empty list using both removeFirst and removeLast
void SLLBBtest4()
{
    printf("Single Linked List: Black box test 4\n");

    removeFirstElementSingleLinkedList(&singleLinkedList);
    removeLastElementSingleLinkedList(&singleLinkedList);

    printSingleLinkedList(&singleLinkedList);
    resetSingleLinkedList(&singleLinkedList);
}

//testing addElement and checking the location and the value at its position
void SLLWBtest1()
{
    printf("Single Linked List: White box test 1\n");

    addElementSingleLinkedList(&singleLinkedList, 16);
    printf("The location of the element: %p \n", singleLinkedList.first);
    printf("The value of the element: %d \n", singleLinkedList.first->data);

    printSingleLinkedList(&singleLinkedList);
    resetSingleLinkedList(&singleLinkedList);
}

//testing addElement twice to check the location and value of the "next"-reference
void SLLWBtest2()
{
    printf("Single Linked List: White box test 2\n");

    addElementSingleLinkedList(&singleLinkedList, 25);
    addElementSingleLinkedList(&singleLinkedList, 6);
    printf("The location of the element: %p \n", singleLinkedList.first);
    printf("The location of the next element: %p \n", singleLinkedList.first->next);
    printf("The value of the next element: %d \n", singleLinkedList.first->next->data);

    printSingleLinkedList(&singleLinkedList);
    resetSingleLinkedList(&singleLinkedList);
}

//testing removeFirst and checking the location and value of head before and after removal
void SLLWBtest3()
{
    printf("Single Linked List: White box test 3\n");

    addElementSingleLinkedList(&singleLinkedList, 1785);
    addElementSingleLinkedList(&singleLinkedList, 551);
    printf("The location of the element: %p \n", singleLinkedList.first);
    printf("The location of the next element: %p \n", singleLinkedList.first->next);
    printf("The value of the next element: %d \n", singleLinkedList.first->data);

    printf("*removing first element*\n");
    removeFirstElementSingleLinkedList(&singleLinkedList);

    printf("The location of the new first element: %p \n", singleLinkedList.first);
    printf("The value of the new first element: %d \n", singleLinkedList.first->data);

    printSingleLinkedList(&singleLinkedList);
    resetSingleLinkedList(&singleLinkedList);
}

//testing removeLast and checking the location/value before and printing list after
void SLLWBtest4()
{
    printf("Single Linked List: White box test 4\n");

    addElementSingleLinkedList(&singleLinkedList, 59);
    addElementSingleLinkedList(&singleLinkedList, 441);
    printf("The location of the element: %p \n", singleLinkedList.first);
    printf("The location of the next element: %p \n", singleLinkedList.first->next);
    printf("The value of the next element: %d \n", singleLinkedList.first->data);

    printf("*removing last element*\n");
    removeLastElementSingleLinkedList(&singleLinkedList);

    printSingleLinkedList(&singleLinkedList);
    resetSingleLinkedList(&singleLinkedList);
}

//Adding elements in non-ascending/descending order to test add and sort function
void DLLBBtest1()
{
    printf("\nDouble Linked List: Black box test 1\n");

    addElementDoubleLinkedList(&doubleLinkedList, 42);
    addElementDoubleLinkedList(&doubleLinkedList, 110);
    addElementDoubleLinkedList(&doubleLinkedList, 0);

    printDoubleLinkedList(&doubleLinkedList);
    resetDoubleLinkedList(&doubleLinkedList);
}

//testing of removeFirstElementDoubleLinkedList
void DLLBBtest2()
{
    printf("\nDouble Linked List: Black box test 2\n");

    addElementDoubleLinkedList(&doubleLinkedList, 452);
    addElementDoubleLinkedList(&doubleLinkedList, 1110);
    printDoubleLinkedList(&doubleLinkedList);

    removeFirstElementDoubleLinkedList(&doubleLinkedList);

    printDoubleLinkedList(&doubleLinkedList);
    resetDoubleLinkedList(&doubleLinkedList);
}

//testing of removeLastElementDoubleLinkedList
void DLLBBtest3()
{
    printf("\nDouble Linked List: Black box test 3\n");

    addElementDoubleLinkedList(&doubleLinkedList, 452);
    addElementDoubleLinkedList(&doubleLinkedList, 1110);
    printDoubleLinkedList(&doubleLinkedList);

    removeLastElementDoubleLinkedList(&doubleLinkedList);

    printDoubleLinkedList(&doubleLinkedList);
    resetDoubleLinkedList(&doubleLinkedList);
}

//testing of removal of element in empty list using both removeFirst and removeLast
void DLLBBtest4()
{
    printf("\nDouble Linked List: Black box test 4\n");

    removeFirstElementDoubleLinkedList(&doubleLinkedList);
    removeLastElementDoubleLinkedList(&doubleLinkedList);

    printDoubleLinkedList(&doubleLinkedList);
    resetDoubleLinkedList(&doubleLinkedList);
}

//testing addElement and checking the location and the value at its position
void DLLWBtest1()
{
    printf("\nDouble Linked List: White box test 1\n");

    addElementDoubleLinkedList(&doubleLinkedList, 16);
    printf("The location of the first element: %p \n", doubleLinkedList.first);
    printf("The value of the element: %d \n", doubleLinkedList.first->data);
    printf("The location of the last: %p \n", doubleLinkedList.last);

    printDoubleLinkedList(&doubleLinkedList);
    resetDoubleLinkedList(&doubleLinkedList);
}

//testing addElement twice to check the location and value of the "next"-reference (and "last")
void DLLWBtest2()
{
    printf("\nDouble Linked List: White box test 2\n");

    addElementDoubleLinkedList(&doubleLinkedList, 16);
    addElementDoubleLinkedList(&doubleLinkedList, 77);
    printf("The location of the first element: %p \n", doubleLinkedList.first);
    printf("The location of the next element: %p \n", doubleLinkedList.first->next);
    printf("The location of the last: %p \n", doubleLinkedList.last);
    printf("The value of the next element: %d \n", doubleLinkedList.first->next->data);

    printDoubleLinkedList(&doubleLinkedList);
    resetDoubleLinkedList(&doubleLinkedList);
}

//testing removeFirst and checking the location and value of first before and after removal
void DLLWBtest3()
{
    printf("\nDouble Linked List: White box test 3\n");

    addElementDoubleLinkedList(&doubleLinkedList, 1661);
    addElementDoubleLinkedList(&doubleLinkedList, 7557);
    printf("The location of the first element: %p \n", doubleLinkedList.first);
    printf("The location of the next element: %p \n", doubleLinkedList.first->next);
    printf("The value of the first element: %d \n", doubleLinkedList.first->data);
    printf("The location of the last: %p \n", doubleLinkedList.last);
    printf("The value of the last element: %d \n", doubleLinkedList.last->data);

    printf("*removing first element*\n");
    removeFirstElementDoubleLinkedList(&doubleLinkedList);

    printf("The new location of first element: %p \n", doubleLinkedList.first);
    printf("The new value of first element: %d \n", doubleLinkedList.first->data);
}

//testing removeLast and checking the location/value of last before and after removal
void DLLWBtest4()
{
    printf("\nDouble Linked List: White box test 4\n");

    addElementDoubleLinkedList(&doubleLinkedList, 1661);
    addElementDoubleLinkedList(&doubleLinkedList, 7557);
    printf("The location of the first element: %p \n", doubleLinkedList.first);
    printf("The location of the next element: %p \n", doubleLinkedList.first->next);
    printf("The value of the first element: %d \n", doubleLinkedList.first->data);
    printf("The location of the last: %p \n", doubleLinkedList.last);
    printf("The value of the last element: %d \n", doubleLinkedList.last->data);

    printf("*removing last element*\n");
    removeLastElementDoubleLinkedList(&doubleLinkedList);

    printf("The new location of last element: %p \n", doubleLinkedList.last);
    printf("The new value of last element: %d \n", doubleLinkedList.last->data);
}
