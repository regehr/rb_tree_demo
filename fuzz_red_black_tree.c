#include "container.h"
#include "red_black_tree.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define META_REPS 1
#define FUZZ_REPS 3000

/*  this file has functions to test a red-black tree of integers */

void IntDest(void *a) { free((int *)a); }

int IntComp(const void *a, const void *b) {
  if (*(int *)a > *(int *)b)
    return (1);
  if (*(int *)a < *(int *)b)
    return (-1);
  return (0);
}

void IntPrint(const void *a) { printf("%i", *(int *)a); }

void InfoPrint(void *a) { ; }

void InfoDest(void *a) { ; }

int FUZZ_RANGE;

int randomInt(void) { return my_rand() % FUZZ_RANGE; }

void *randomVoidP(void) {
  uintptr_t p = 0;
  int i;
  for (i = 0; i < sizeof(p); i++) {
    p <<= 8;
    p |= my_rand() % 256;
  }
  return (void *)p;
}

int idx;

int nodups;

void InorderTreeVerify(rb_red_blk_tree *tree, rb_red_blk_node *x) {
  // RBTreePrint(tree);
  printf("\n");
  if (x != tree->nil) {
    struct elt_t e;
    InorderTreeVerify(tree, x->left);
    e = containerGet(idx);
    printf("expecting %d and got %d\n",
           e.val,
           *(int *)x->key);
    Assert(e.val == *(int *)x->key, "value mismatch");
    if (nodups)
      Assert(e.info == x->info, "info mismatch");
    idx = containerNext(idx);
    InorderTreeVerify(tree, x->right);
  }
}

void RBTreeVerify(rb_red_blk_tree *tree) {
  idx = containerStart();
  InorderTreeVerify(tree, tree->root->left);
  Assert(idx == -1, "index should be -1");
}

static void fuzzit(void) {
  stk_stack *enumResult;
  int option = 0;
  int newKey, newKey2;
  int *newInt;
  rb_red_blk_node *newNode;
  rb_red_blk_tree *tree;
  int i;
  int fuzz_reps;

  fuzz_reps = 1 + my_rand() % FUZZ_REPS;

  tree = RBTreeCreate(IntComp, IntDest, InfoDest, IntPrint, InfoPrint);
  containerCreate();
  nodups = my_rand() % 2;
  if (my_rand() % 2 == 0) {
    FUZZ_RANGE = 1 + my_rand() % fuzz_reps;
  } else {
    FUZZ_RANGE = 1 + my_rand() % RAND_MAX;
  }

  for (i = 0; i < fuzz_reps; i++) {

    checkRep(tree);

  again:
    option = 1 + my_rand() % 7;
    printf("option = %d\n", option);
    switch (option) {
    case 1: {
      void *p;
      newKey = randomInt();

      if (nodups) {
        if (containerFind(newKey))
          goto again;
      }

      newInt = (int *)malloc(sizeof(int));
      *newInt = newKey;
      p = randomVoidP();
      RBTreeInsert(tree, newInt, p);
      containerInsert(newKey, p);
      printf("Inserted %d.\n", newKey);
    } break;

    case 2: {
      newKey = randomInt();
      if ((newNode = RBExactQuery(tree, &newKey))) {
        Assert(containerFind(newKey), "find");
        RBDelete(tree, newNode); /*assignment*/
        containerDelete(newKey);
        printf("Deleted %d.\n", newKey);
      } else {
        Assert(!containerFind(newKey), "should not be found");
      }
    } break;

    case 3: {
      newKey = randomInt();
      if ((newNode = RBExactQuery(tree, &newKey))) { /*assignment*/
        Assert(containerFind(newKey), "should be found");
      } else {
        Assert(!containerFind(newKey), "should not be found");
      }
      printf("Find.\n");
    } break;
    case 4: {
      int res, key2;
      newKey = randomInt();
      res = containerPred(newKey, &key2);
      if ((newNode = RBExactQuery(tree, &newKey))) { /*assignment*/
        newNode = TreePredecessor(tree, newNode);
        if (nodups) {
          if (tree->nil == newNode) {
            Assert(res == NO_PRED_OR_SUCC, "pred");
          } else {
            Assert(res == FOUND, "res mismatch");
            Assert(*(int *)newNode->key == key2, "key mismatch");
          }
        }
      } else {
        Assert(res == KEY_NOT_FOUND, "must not be found");
      }
      printf("Query pred.\n");
    } break;
    case 5: {
      int res, key2;
      newKey = randomInt();
      res = containerSucc(newKey, &key2);
      if ((newNode = RBExactQuery(tree, &newKey))) {
        newNode = TreeSuccessor(tree, newNode);
        if (nodups) {
          if (tree->nil == newNode) {
            Assert(res == NO_PRED_OR_SUCC, "res mismatch");
          } else {
            Assert(res == FOUND, "must be found");
            Assert(*(int *)newNode->key == key2, "key mismatch");
          }
        }
      } else {
        Assert(!containerFind(newKey), "should not be found");
        Assert(res == KEY_NOT_FOUND, "res mismatch");
      }
      printf("Query succ.\n");
    } break;
    case 6: {
      int i;
      newKey = randomInt();
      newKey2 = randomInt();
      i = containerStartVal(newKey, newKey2);
      enumResult = RBEnumerate(tree, &newKey, &newKey2);
      while ((newNode = StackPop(enumResult))) {
        struct elt_t e;
        Assert(i != -1, "i mismatch");
        e = containerGet(i);
        Assert(e.val == *(int *)newNode->key, "val mismatch");
        if (nodups)
          Assert(e.info == newNode->info, "dups mismatch");
        i = containerNextVal(newKey2, i);
      }
      Assert(i == -1, "another i mismatch");
      free(enumResult);
      printf("Enumerate.\n");
    } break;
    case 7: {
      RBTreeVerify(tree);
    } break;
    default:
      Assert(0, "unreachable");
    }
  }
  RBTreeVerify(tree);
  if (my_rand() % 2 == 0) {
    while (1) {
      int val;
      int res = containerRandom(&val);
      if (!res)
        break;
      containerDelete(val);
      if (!(newNode = RBExactQuery(tree, &val)))
        Assert(0, "also unreachable");
      RBDelete(tree, newNode);
    }
  }
  RBTreeDestroy(tree);
}

int main() {
  int i;

  my_srand(11);
  //my_srand(time(NULL));

  for (i = 0; i < META_REPS; i++) {
    fuzzit();
  }

  printf("Done fuzzing\n");
  return 0;
}
