#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "single_linked_list.h"

int addElementSingleLinkedList(struct singleLinkedList *list, int value)
{

  struct singleLinkedListElement *newElement = (struct singleLinkedListElement *)malloc(sizeof(struct singleLinkedListElement));
  struct singleLinkedListElement *temp;

  //in case of empty list, no "next" exists to build list from
  if (list->first == NULL)
  {
    list->first = newElement;
    newElement->data = value;
    newElement->next = NULL;
    return value;
  }
  else //if not empty
  {
    temp = list->first;

    //loop to last element
    while (temp->next != NULL)
    {
      temp = temp->next;
    }

    //set the new element as next of the last element in the list and insert the data
    temp->next = newElement;
    newElement->data = value;
    newElement->next = NULL;

    sortSLL(list);

    return value;
  }

  return INT_MIN;
}

void initSingleLinkedList(struct singleLinkedList *list)
{
  list->first = NULL;
}

int removeFirstElementSingleLinkedList(struct singleLinkedList *list)
{
  struct singleLinkedListElement *removeFirst = list->first;
  struct singleLinkedListElement *removeMalocFirst = list->first;
  int returnValue;

  //in case of empty list, nothing can be removed
  if (list->first == NULL)
  {
    return INT_MIN;
  }

  //in case of one element in the list
  if (removeFirst->next == NULL)
  {
    returnValue = list->first->data;
    free(list->first); //deallocate memory
    list->first = NULL;
    return returnValue;
  }

  //in case of more than one element in the list
  if (removeFirst->next != NULL)
  {
    list->first = list->first->next;
    free(removeMalocFirst); //deallocate memory
    return list->first->data;
  }
  return INT_MIN;
}

int removeLastElementSingleLinkedList(struct singleLinkedList *list)
{
  struct singleLinkedListElement *removeLast = list->first;
  struct singleLinkedListElement *secondLastNode = list->first;
  int returnValue;

  //in case of empty list, nothing can be removed
  if (list->first == NULL)
  {
    return INT_MIN;
  }

  //in case of one element in the list
  if (removeLast->next == NULL)
  {
    returnValue = list->first->data;
    free(list->first); //deallocate memory
    list->first = NULL;
    return returnValue;
  }
  else
  { //saving away 2nd to last element in order too free() the space taken by the last element (which is being removed)
    while (removeLast->next != NULL)
    {
      secondLastNode = removeLast;
      removeLast = removeLast->next;
    }
    secondLastNode->next = NULL;
    free(removeLast); //deallocate memory
    return list->first->data;
  }
  return INT_MIN;
}

void printSingleLinkedList(struct singleLinkedList *list)
{
  struct singleLinkedListElement *element = list->first;
  printf("\n\n");
  printf("Single Linked List:\n");
  while (element != NULL)
  {
    printf("%d \n", element->data);
    element = element->next;
  }
  printf("\n\n");
}

//simple bubble sort algorithm, ineffective for big lists but easy to implement
void sortSLL(struct singleLinkedList *list)
{
  struct singleLinkedListElement *elementA;
  struct singleLinkedListElement *elementB = NULL;
  int swapped;

  //returns if list is empty
  if (list->first == NULL)
  {
    return;
  }

  do
  {
    swapped = 0;
    elementA = list->first;
    while (elementA->next != elementB)
    {
      if (elementA->data > elementA->next->data)
      {
        swapSLL(elementA, elementA->next);
        swapped = 1;
      }
      elementA = elementA->next;
    }
    elementB = elementA;
  } while (swapped);
}

void swapSLL(struct singleLinkedListElement *valueA, struct singleLinkedListElement *valueB)
{
  int temp = valueA->data;
  valueA->data = valueB->data;
  valueB->data = temp;
}

//reset function used in testing

void resetSingleLinkedList(struct singleLinkedList *list)
{
  struct singleLinkedListElement *remove = list->first;
  struct singleLinkedListElement *ptr = list->first;

  while (remove != NULL)
  {
    ptr = ptr->next;
    free(remove); //deallocate space
    remove = NULL;
    remove = ptr;
  }
  list->first = NULL;
}