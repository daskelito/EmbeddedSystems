#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "double_linked_list.h"

int addElementDoubleLinkedList(struct doubleLinkedList *list, int value)
{
  struct doubleLinkedListElement *newElement = (struct doubleLinkedListElement *)malloc(sizeof(struct doubleLinkedListElement));
  struct doubleLinkedListElement *temp;
  struct doubleLinkedListElement *tempPrev;

  //in case of empty list, no "next" exists to build list from
  if (list->first == NULL)
  {
    list->first = newElement;
    list->last = newElement;
    newElement->data = value;
    newElement->next = NULL;
    newElement->previous = NULL;
    return value;
  }
  else //if not empty
  {
    temp = list->first;
    tempPrev = list->first;

    //loop to last element
    while (temp->next != NULL)
    {
      temp = temp->next;
      tempPrev = temp->previous;
    }

    //set the new element as next of the last element and also update previous/last
    temp->next = newElement;
    list->last = newElement;
    newElement->next = NULL;
    newElement->previous = tempPrev;
    newElement->data = value;
    sortDLL(list);
    return value;
  }

  return INT_MIN;
}

void initDoubleLinkedList(struct doubleLinkedList *list)
{
  list->first = NULL;
  list->last = NULL;
}

int removeFirstElementDoubleLinkedList(struct doubleLinkedList *list)
{
  struct doubleLinkedListElement *removeFirst = list->first;
  struct doubleLinkedListElement *removeMalocFirst = list->first;
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
    list->last = NULL;
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

int removeLastElementDoubleLinkedList(struct doubleLinkedList *list)
{
  struct doubleLinkedListElement *removeLast = list->first;
  struct doubleLinkedListElement *secondLastNode = list->first;
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
    list->last = NULL;
    return returnValue;
  }
  else
  { //saving away 2nd to last element in order too free() the space taken by the last element (which is being removed)
    while (removeLast->next != NULL)
    {
      secondLastNode = removeLast;
      removeLast = removeLast->next;
    }
    list->last = secondLastNode; //update "last"-reference
    secondLastNode->next = NULL;
    free(removeLast); //deallocate memory
    return list->last->data;
  }

  return INT_MIN;
}

void printDoubleLinkedList(struct doubleLinkedList *list)
{
  struct doubleLinkedListElement *element = list->first;
  printf("\n\n");
  printf("Double Linked List:\n");
  while (element != NULL)
  {
    printf("%d \n", element->data);
    element = element->next;
  }
  printf("\n\n");
}

//simple bubble sort algorithm, ineffective for big lists but easy to implement
void sortDLL(struct doubleLinkedList *list)
{
  struct doubleLinkedListElement *elementA;
  struct doubleLinkedListElement *elementB = NULL;
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
        swapDLL(elementA, elementA->next);
        swapped = 1;
      }
      elementA = elementA->next;
    }
    elementB = elementA;
  } while (swapped);
}

void swapDLL(struct doubleLinkedListElement *valueA, struct doubleLinkedListElement *valueB)
{
  int temp = valueA->data;
  valueA->data = valueB->data;
  valueB->data = temp;
}

void resetDoubleLinkedList(struct doubleLinkedList *list)
{
  struct doubleLinkedListElement *remove = list->first;
  struct doubleLinkedListElement *ptr = list->first;

  while (remove != NULL)
  {
    ptr = ptr->next;
    free(remove);
    remove = NULL;
    remove = ptr;
  }
  list->first = NULL;
  list->last = NULL;
}