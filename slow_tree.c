#include "slow_tree.h"
#include <assert.h>

#define MAX_SIZE 1000

int size;
int array[MAX_SIZE];

void slowTreeCreate (void)
{
  size = 0;
}

void slowTreeInsert (int val)
{
  assert (size < MAX_SIZE);
  array[size] = val;
  size++;
}

