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

    option = 1 + rand()%7;
    switch(option)
      {
      case 1:
	{
	  newKey = randomInt();
	  newInt=(int*) malloc(sizeof(int));
	  *newInt=newKey;
	  RBTreeInsert(tree,newInt,0);
	  slowContainerInsert(newKey);
	}
	break;
	
      case 2:
	{
	  newKey = randomInt();
	  if ( ( newNode=RBExactQuery(tree,&newKey ) ) ) RBDelete(tree,newNode);/*assignment*/
	  else printf("key not found in tree, no action taken\n");
	}
	break;

      case 3:
	{
	  newKey = randomInt();
	  if ( ( newNode = RBExactQuery(tree,&newKey) ) ) {/*assignment*/
	    printf("data found in tree at location %p\n", (void *)newNode);
	  } else {
	    printf("data not in tree\n");
	  }
	}
	break;
      case 4:
	{
	  newKey = randomInt();
	  if ( ( newNode = RBExactQuery(tree,&newKey) ) ) {/*assignment*/
	    newNode=TreePredecessor(tree,newNode);
	    if(tree->nil == newNode) {
	      printf("there is no predecessor for that node (it is a minimum)\n");
	    } else {
	      printf("predecessor has key %i\n",*(int*)newNode->key);
	    }
	  } else {
	    printf("data not in tree\n");
	  }
	}
	break;
      case 5:
	{
	  newKey = randomInt();
	  if ( (newNode = RBExactQuery(tree,&newKey) ) ) {
	    newNode=TreeSuccessor(tree,newNode);
	    if(tree->nil == newNode) {
	      printf("there is no successor for that node (it is a maximum)\n");
	    } else {
	      printf("successor has key %i\n",*(int*)newNode->key);
	    }
	  } else {
	    printf("data not in tree\n");
	  }
	}
	break;
      case 6:
	{
	  newKey = randomInt();
	  newKey2 = randomInt();
	  enumResult=RBEnumerate(tree,&newKey,&newKey2);	  
	  while ( (newNode = StackPop(enumResult)) ) {
	    tree->PrintKey(newNode->key);
	    printf("\n");
	  }
	  free(enumResult);
	}
	break;
      case 7:
	{
	  RBTreePrint(tree);
	}
	break;
      default:
	assert (0);
      }
  }
  RBTreeDestroy(tree);
  return 0;
}
