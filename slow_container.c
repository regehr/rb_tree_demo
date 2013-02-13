#include "slow_container.h"
#include <assert.h>

#define MAX_SIZE 1000

int size;
int array[MAX_SIZE];

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

