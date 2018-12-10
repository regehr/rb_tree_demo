#include "container.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 10000

static int size;
static struct elt_t array[MAX_SIZE];

void containerCreate(void) { size = 0; }

void containerInsert(int val, void *info) {
  assert(size < MAX_SIZE);
  array[size].val = val;
  array[size].info = info;
  size++;
}

int containerFind(int val) {
  int i;
  for (i = 0; i < size; i++) {
    if (array[i].val == val)
      return 1;
  }
  return 0;
}

int containerRandom(int *res) {
  if (size == 0)
    return 0;
  *res = array[rand() % size].val;
  return 1;
}

void containerDelete(int val) {
  int i, j;
  for (i = 0; i < size; i++) {
    if (array[i].val == val)
      goto remove;
  }
  assert(0);
remove:
  for (j = i; j < size; j++) {
    array[j] = array[j + 1];
  }
  size--;
}

static int compar(const void *a, const void *b) {
  int ai = ((struct elt_t *)a)->val;
  int bi = ((struct elt_t *)b)->val;
  return (ai > bi) - (ai < bi);
}

void containerSort(void) { qsort(array, size, sizeof(struct elt_t), compar); }

int containerPred(int val, int *ret) {
  int i;
  containerSort();
  for (i = 0; i < size; i++) {
    if (array[i].val == val)
      goto found;
  }
  return KEY_NOT_FOUND;
found:
  if (i == 0)
    return NO_PRED_OR_SUCC;
  *ret = array[i - 1].val;
  return FOUND;
}

int containerSucc(int val, int *ret) {
  int i;
  containerSort();
  for (i = 0; i < size; i++) {
    if (array[i].val == val)
      goto found;
  }
  return KEY_NOT_FOUND;
found:
  if (i == (size - 1))
    return NO_PRED_OR_SUCC;
  *ret = array[i + 1].val;
  return FOUND;
}

int containerStartVal(int val, int val2) {
  int i;
  containerSort();
  for (i = 0; i < size; i++) {
    if (array[i].val >= val && array[i].val <= val2) {
      return i;
    }
  }
  return -1;
}

int containerNextVal(int val, int i) {
  i++;
  if (i == size)
    return -1;
  if (array[i].val <= val)
    return i;
  return -1;
}

int containerNext(int i) {
  i++;
  if (i == size)
    return -1;
  return i;
}

int containerStart(void) {
  containerSort();
  if (size == 0)
    return -1;
  return 0;
}

struct elt_t containerGet(int i) {
  assert(i >= 0);
  assert(i < size);
  return array[i];
}

void containerPrint(void) {
  int i;
  containerSort();
  for (i = 0; i < size; i++) {
    printf("%d ", array[i].val);
  }
  printf("\n");
}
