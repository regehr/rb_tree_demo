#include"red_black_tree.h"
#include<stdio.h>
#include<ctype.h>
#include <assert.h>
#include <time.h>
#include "slow_container.h"

#define FUZZ_REPS 1000
#define FUZZ_RANGE 100


/*  this file has functions to test a red-black tree of integers */

void IntDest(void* a) {
  free((int*)a);
}



int IntComp(const void* a,const void* b) {
  if( *(int*)a > *(int*)b) return(1);
  if( *(int*)a < *(int*)b) return(-1);
  return(0);
}

void IntPrint(const void* a) {
  printf("%i",*(int*)a);
}

void InfoPrint(void* a) {
  ;
}

void InfoDest(void *a){
  ;
}

int randomInt (void)
{
  return rand()%FUZZ_RANGE;
}

int idx;

void InorderTreeVerify(rb_red_blk_tree* tree, rb_red_blk_node* x) {
  if (x != tree->nil) {
    int res;
    InorderTreeVerify(tree,x->left);
    res = slowContainerGet (idx);
    printf ("verifying %d %d\n", res, *(int *)x->key);
    assert (res==*(int *)x->key);
    idx = slowContainerNext (idx);
    InorderTreeVerify(tree,x->right);
  }
}

void RBTreeVerify(rb_red_blk_tree* tree) {
  idx = slowContainerStart();
  InorderTreeVerify(tree,tree->root->left);
  assert (idx == -1);
}

int main() {
  stk_stack* enumResult;
  int option=0;
  int newKey,newKey2;
  int* newInt;
  rb_red_blk_node* newNode;
  rb_red_blk_tree* tree;
  int i;

  srand (time(NULL));

  tree=RBTreeCreate(IntComp,IntDest,InfoDest,IntPrint,InfoPrint);
  slowContainerCreate ();

  for (i=0; i<FUZZ_REPS; i++) {

    checkRep (tree);

  again:
    option = 1 + rand()%7;
    switch(option)
      {
      case 1:
	{
	  newKey = randomInt();

	  /* fixme? */
	  if (slowContainerFind (newKey)) goto again;

	  newInt=(int*) malloc(sizeof(int));
	  *newInt=newKey;
	  RBTreeInsert(tree,newInt,0);
	  slowContainerInsert(newKey);
	}
	break;
	
      case 2:
	{
	  newKey = randomInt();
	  if ( ( newNode=RBExactQuery(tree,&newKey ) ) ) {
	    assert (slowContainerFind (newKey));
	    RBDelete(tree,newNode);/*assignment*/
	    slowContainerDelete (newKey);
	  } else {
	    assert (!slowContainerFind (newKey));
	    printf("key not found in tree, no action taken\n");
	  }
	}
	break;

      case 3:
	{
	  newKey = randomInt();
	  if ( ( newNode = RBExactQuery(tree,&newKey) ) ) {/*assignment*/
	    assert (slowContainerFind (newKey));
	    printf("data found in tree at location %p\n", (void *)newNode);
	  } else {
	    assert (!slowContainerFind (newKey));
	    printf("data not in tree\n");
	  }
	}
	break;
      case 4:
	{
	  int res, key2;
	  newKey = randomInt();
	  res = slowContainerPred(newKey, &key2);
	  if ( ( newNode = RBExactQuery(tree,&newKey) ) ) {/*assignment*/
	    newNode=TreePredecessor(tree,newNode);
	    if(tree->nil == newNode) {
	      printf("there is no predecessor for that node (it is a minimum)\n");
	      assert (res==NO_PRED_OR_SUCC);
	    } else {
	      printf("predecessor has key %i, expected %d\n",*(int*)newNode->key, key2);
	      assert (res==FOUND);
	      assert (*(int *)newNode->key == key2);
	    }
	  } else {
	    printf("data not in tree\n");
	    assert (res==KEY_NOT_FOUND);
	  } 
	}
	break;
      case 5:
	{
	  int res, key2;
	  newKey = randomInt();
	  res = slowContainerSucc(newKey, &key2);
	  if ( (newNode = RBExactQuery(tree,&newKey) ) ) {
	    newNode=TreeSuccessor(tree,newNode);
	    if(tree->nil == newNode) {
	      printf("there is no successor for that node (it is a maximum)\n");
	      assert (res==NO_PRED_OR_SUCC);
	    } else {
	      printf("successor has key %i\n",*(int*)newNode->key);
	      assert (res==FOUND);
	      assert (*(int *)newNode->key == key2);	      
	    }
	  } else {
	    assert (!slowContainerFind (newKey));
	    printf("data not in tree\n");
	    assert (res==KEY_NOT_FOUND);
	  }
	}
	break;
      case 6:
	{
	  int i;
	  printf ("tree contents:");
	  RBTreePrint(tree);
	  newKey = randomInt();
	  newKey2 = randomInt();
	  printf ("iterating from %d to %d\n", newKey, newKey2);
	  i = slowContainerStartVal (newKey,newKey2);
	  enumResult=RBEnumerate(tree,&newKey,&newKey2);	  
	  while ( (newNode = StackPop(enumResult)) ) {
	    int k;
	    printf ("rbtree: ");
	    tree->PrintKey(newNode->key);	    
	    printf ("\n");
	    assert (i != -1);
	    k = slowContainerGet(i);
	    printf (" %d\n", k);
	    assert (k == *(int *)newNode->key);
	    i = slowContainerNextVal (newKey2, i);
	  }
	  assert (i==-1);
	  free(enumResult);
	}
	break;
      case 7:
	{
	  RBTreeVerify(tree);
	}
	break;
      default:
	assert (0);
      }
  }
  RBTreeVerify(tree);
  if (rand()%2 == 0) {
    while (1) {
      int val;
      int res = slowContainerRandom (&val);
      if (!res) break;
      slowContainerDelete (val);
      if (!(newNode=RBExactQuery(tree,&val))) assert (0);
      RBDelete(tree,newNode);
    }    
  }
  RBTreeDestroy(tree);
  printf ("Done fuzzing\n");
  return 0;
}
