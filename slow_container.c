#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "slow_container.h"

#define MAX_SIZE 1000

static int size;
static int array[MAX_SIZE];

void slowContainerCreate (void)
{
  size = 0;
}

void slowContainerInsert (int val)
{
  assert (size < MAX_SIZE);
  array[size] = val;
  size++;
}

int slowContainerFind (int val)
{
  int i;
  for (i=0; i<size; i++) {
    if (array[i] == val) return 1;
  }
  return 0;
}

int slowContainerRandom (int *res)
{
  if (size==0) return 0;
  *res = array[rand()%size];
  return 1;
}

void slowContainerDelete (int val)
{
  int i, j;
  for (i=0; i<size; i++) {
    if (array[i] == val) goto remove;
  }
  assert (0);
 remove:
  for (j=i; j<size; j++) {
    array[j] = array[j+1];
  }
  size--;
}

static int compar (const void *a, const void *b)
{
  int ai = * ((int *) a);
  int bi = * ((int *) b);
  return ai > bi;
}

void slowContainerSort (void)
{
  qsort (array, size, sizeof (int), compar);
}

int slowContainerPred (int val, int *ret)
{
  int i;
  slowContainerSort();
  for (i=0; i<size; i++) {
    if (array[i] == val) goto found;
  }
  return KEY_NOT_FOUND;
 found:
  if (i==0) return NO_PRED_OR_SUCC;
  *ret = array[i-1];
  return FOUND;
}

int slowContainerSucc (int val, int *ret)
{
  int i;
  slowContainerSort();
  for (i=0; i<size; i++) {
    if (array[i] == val) goto found;
  }
  return KEY_NOT_FOUND;
 found:
  if (i==(size-1)) return NO_PRED_OR_SUCC;
  *ret = array[i+1];
  return FOUND;
}

int slowContainerStartVal (int val, int val2)
{
  int i;
  slowContainerSort();
  for (i=0; i<size; i++) {
    if (array[i]>=val && array[i]<=val2) {
      printf ("%d was found at %d\n", array[i], i);
      return i;
    }
  }
  printf ("%d wasn't found\n", val);
  return -1;
}

int slowContainerNextVal (int val, int i)
{
  i++;
  if (i==size) return -1;
  if (array[i] <= val) return i;
  return -1;
}

int slowContainerNext (int i)
{
  i++;
  if (i==size) return -1;
  return i;
}

int slowContainerStart (void)
{
  slowContainerSort();
  if (size==0) return -1;
  return 0;
}

int slowContainerGet (int i)
{
  assert (i>=0);
  assert (i<size);
  return array[i];
}

void slowContainerPrint (void)
{
  int i;
  slowContainerSort();
  for (i=0; i<size; i++) {
    printf ("%d ", array[i]);
  }
  printf ("\n");
}
