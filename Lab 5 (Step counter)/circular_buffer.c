#include <stdio.h>
#include <limits.h>
#include "circular_buffer.h"
#include <math.h>
#include "pins.h"
#include <stdlib.h>

void initCircularBuffer(struct circularBuffer *bufferPtr, int *data, int maxLength)
{
  bufferPtr->data = data;
  bufferPtr->head = 0;
  bufferPtr->tail = 0;
  bufferPtr->maxLength = maxLength;
  bufferPtr->empty = 1;
}

/* 
 * This function should check if the buffer pointed to by bufferPtr
 * contains one or more elements with the value specified by the 
 * 'value' argument.
 *
 * The function should return:
 *  - 'value' if the an element with the argument value was found in the queue.
 *  - INT_MIN (defined in limits.h) if value was not found.
 */
int contains(struct circularBuffer *bufferPtr, int value)
{

  if (bufferPtr->empty == 0)
  {
    //if head is lower pos than tail, check from 0 to head and then tail to maxLength - 1
    if (bufferPtr->head < bufferPtr->tail)
    {
      for (int i = 0; i <= bufferPtr->head; i++)
      {
        if (value == bufferPtr->data[i])
        {
          return value;
        }
      }
      for (int i = bufferPtr->tail; i <= bufferPtr->maxLength - 1; i++)
      {
        if (value == bufferPtr->data[i])
        {
          return value;
        }
      }
    }

    //if tail is lower than head, it means that the buffer has no gaps in the memory, in that case loop from tail to head
    if (bufferPtr->tail < bufferPtr->head)
    {
      for (int i = bufferPtr->tail; i <= bufferPtr->head; i++)
      {
        if (value == bufferPtr->data[i])
        {
          return value;
        }
      }
    }
  }

  //if no if-statements were true, value was not found
  return INT_MIN;
}

/*
 * This function should add the value specified by the 'value' 
 * argument at the tail of the buffer.
 *
 * The function should return:
 *  - 'value' if the value was successfully added to the queue.
 *  - INT_MIN (defined in limits.h) if the value was not added.
 */
int addElement(struct circularBuffer *bufferPtr, int value)
{
  //if the buffer is empty, dont move head or tail
  if (bufferPtr->empty == 1)
  {
    bufferPtr->data[bufferPtr->head] = value;
    bufferPtr->empty = 0;
    return value;
  }

  //If the buffer is full (this is indicated by that the head is 1 pos before tails), head also needs to move when the tails move to add the new element.

  //2 cases when the buffer is full:
  //case 1. head is 1 pos before tail,
  //case 2. head is at end and tail is at 0.
  if (bufferPtr->head == bufferPtr->tail - 1 || (bufferPtr->head == bufferPtr->maxLength - 1 && bufferPtr->tail == 0))
  {
    //Moves head back one space, if its outside the buffer, move head to end of the buffer
    bufferPtr->head = bufferPtr->head - 1;
    if (bufferPtr->head < 0)
    {
      bufferPtr->head = bufferPtr->maxLength - 1;
    }
  }

  //moves tail back one index, if its outside the buffer, move tail to end of buffer, insert value at the new tail
  bufferPtr->tail = bufferPtr->tail - 1;
  if (bufferPtr->tail < 0)
  {
    bufferPtr->tail = bufferPtr->maxLength - 1;
  }
  bufferPtr->data[bufferPtr->tail] = value;

  // no longer empty if successful add
  if (bufferPtr->empty == 1)
  {
    bufferPtr->empty = 0;
  }
  return value;
}

/* 
 * This function should remove all elements in the buffer whose
 * value matches the 'value' argument.
 * 
 * The function should return:
 *  The argument value: if at least one element was removed from queue.
 *  INT_MIN (defined in limits.h) if no element was removed. 
 */

//makes a new array and only transfers values that are !value, then puts them back into the buffer
int removeValue(struct circularBuffer *bufferPtr, int value)
{
  if (bufferPtr->empty == 0)
  {
    int tempArr[bufferPtr->maxLength];
    int index = 0;

    //case where only one element is in the buffer, check the value against the value of the element in the buffer
    if (bufferPtr->head == bufferPtr->tail && value == bufferPtr->data[bufferPtr->head])
    {
      bufferPtr->empty = 1;
      return value;
    }

    //in case of no gaps (tail < head), copy values to temp array
    if (bufferPtr->tail < bufferPtr->head)
    {
      for (int i = bufferPtr->tail; i <= bufferPtr->head; i++)
      {
        if (bufferPtr->data[i] != value)
        {
          tempArr[index] = bufferPtr->data[i];
          index++;
        }
      }
    }

    //in case of gaps (head < tail), copy tail part first, then head part
    if (bufferPtr->head < bufferPtr->tail)
    {
      for (int i = bufferPtr->tail; i <= bufferPtr->maxLength - 1; i++)
      {
        if (bufferPtr->data[i] != value)
        {
          tempArr[index] = bufferPtr->data[i];
          index++;
        }
      }
      for (int i = 0; i <= bufferPtr->head; i++)
      {
        if (bufferPtr->data[i] != value)
        {
          tempArr[index] = bufferPtr->data[i];
          index++;
        }
      }
    }

    //take values from new array, and put back in to buffer
    for (int i = 0; i < index; i++)
    {
      bufferPtr->data[i] = tempArr[i];
    }
    bufferPtr->head = index - 1;
    bufferPtr->tail = 0;

    return value;
  }

  //return int min if empty
  return INT_MIN;
}

/* 
 * Remove the oldest element, which is at the head of the queue. 
 * 
 * The function should return:
 *   - 'value' if the head element was successfully removed
 *   - INT_MIN (defined in limits.h) if no element was removed (i.e., the
 *     queue was empty when the function was called.       
 */
int removeHead(struct circularBuffer *bufferPtr)
{
  if (bufferPtr->empty == 0)
  {
    int tempVal = bufferPtr->data[bufferPtr->head];

    if (bufferPtr->head == bufferPtr->tail)
    {
      bufferPtr->head = bufferPtr->head - 1;
      bufferPtr->tail = bufferPtr->tail - 1;

      //update empty if head and tail were at same pos
      bufferPtr->empty = 1;
    }

    if (bufferPtr->head != bufferPtr->tail)
    {
      bufferPtr->head = bufferPtr->head - 1;
    }

    //put head and tail at the end of the buffer if they happen to be placed outside ( <1 )
    if (bufferPtr->head < 0)
    {
      bufferPtr->head = bufferPtr->maxLength - 1;
    }
    if (bufferPtr->tail < 0)
    {
      bufferPtr->tail = bufferPtr->maxLength - 1;
    }

    return tempVal;
  }
  return INT_MIN;
}

/* 
 * Print the elements in the buffer from head to tail. 
 */
void printBuffer(struct circularBuffer *bufferPtr)
{
  printf("\n");
  printf("---------Buffer--------\n");
  if (bufferPtr->empty == 1)
  {
    printf("Buffer is empty \n");
    printf("-----------------------\n");
    printf("\n\n");
    return;
  }

  printf("Head at index [%d] - value: %d \n", bufferPtr->head, bufferPtr->data[bufferPtr->head]);
  printf("Tail at index [%d] - value: %d \n", bufferPtr->tail, bufferPtr->data[bufferPtr->tail]);

  //if there is only one value in the buffer, head and tail are at the same spot BUT buffer is not empty, print heads, tails and the value
  if (bufferPtr->head == bufferPtr->tail && bufferPtr->empty == 0)
  {
    printf("Index: [%d] - value: %d \n", bufferPtr->head, bufferPtr->data[bufferPtr->head]);
  }

  //if there are gaps (head < tail), print (in backwards manner) from head to 0, then from end to tail
  if (bufferPtr->head < bufferPtr->tail)
  {
    for (int i = bufferPtr->head; i >= 0; i--)
    {
      printf("Index: [%d] - value: %d \n", i, bufferPtr->data[i]);
    }
    for (int i = bufferPtr->maxLength - 1; i >= bufferPtr->tail; i--)
    {
      printf("Index: [%d] - value: %d \n", i, bufferPtr->data[i]);
    }
  }

  //other case if tail < head, print from head (in backwards manner) from head to tail
  if (bufferPtr->head > bufferPtr->tail)
  {
    for (int i = bufferPtr->head; i >= bufferPtr->tail; i--)
    {
      printf("Index: %d - value: %d \n", i, bufferPtr->data[i]);
    }
  }
  printf("-----------------------\n");
  printf("\n\n");
}

void resetBuffer(struct circularBuffer *bufferPtr)
{
  bufferPtr->empty = 1;
  bufferPtr->head = 0;
  bufferPtr->tail = 0;
}

int nbrOfValues(struct circularBuffer *bufferPtr)
{
  int tempHeadPointer = bufferPtr->head;
  int tempIndex = 0;
  if (bufferPtr->empty)
    return 0;
  do
  {
    tempHeadPointer++;
    tempIndex++;
    if (tempHeadPointer == (bufferPtr->tail + 1))
      break;
    if (tempHeadPointer >= bufferPtr->maxLength)
      tempHeadPointer = 0;
  } while (tempHeadPointer != (bufferPtr->tail + 1));
  return tempIndex;
}

uint32_t sumOfBuffer(struct circularBuffer *bufferPtr)
{
  int tempHeadPointer = bufferPtr->head;
  uint32_t sum = 0;
  if (bufferPtr->empty)
    return 0;
  do
  {
    sum += bufferPtr->data[tempHeadPointer];
    tempHeadPointer += 1;
    if (tempHeadPointer == (bufferPtr->tail + 1))
      break;
    if (tempHeadPointer >= bufferPtr->maxLength)
      tempHeadPointer = 0;
  } while (tempHeadPointer != (bufferPtr->tail + 1));
  return sum;
}

uint32_t sumOfBufferAllButOne(struct circularBuffer *bufferPtr, int value)
{
  int tempHeadPointer = bufferPtr->head;
  uint32_t sum = 0;
  if (bufferPtr->empty)
    return 0;
  do
  {
    sum += (uint32_t)pow((double)(bufferPtr->data[tempHeadPointer] - value), 2); //removes the result of the value from the buffer
    tempHeadPointer += 1;
    if (tempHeadPointer == (bufferPtr->tail + 1))
      break;
    if (tempHeadPointer >= bufferPtr->maxLength)
      tempHeadPointer = 0;

  } while (tempHeadPointer != (bufferPtr->tail + 1));
  return sum;
}
