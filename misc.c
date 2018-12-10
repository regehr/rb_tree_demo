#include "misc.h"

static unsigned state;

void my_srand(int s) {
  state = s;
}

int my_rand (void) {
  /* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
  unsigned x;
  do {
    x = state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    state = x;
  } while (x < 0 || x > RAND_MAX);
  return x;
}

/***********************************************************************/
/*  FUNCTION:  void Assert(int assertion, char* error)  */
/**/
/*  INPUTS: assertion should be a predicated that the programmer */
/*  assumes to be true.  If this assumption is not true the message */
/*  error is printed and the program exits. */
/**/
/*  OUTPUT: None. */
/**/
/*  Modifies input:  none */
/**/
/*  Note:  If DEBUG_ASSERT is not defined then assertions should not */
/*         be in use as they will slow down the code.  Therefore the */
/*         compiler will complain if an assertion is used when */
/*         DEBUG_ASSERT is undefined. */
/***********************************************************************/

void Assert(int assertion, char *error) {
  if (!assertion) {
    printf("Assertion Failed: %s\n", error);
    exit(-1);
  }
}

/***********************************************************************/
/*  FUNCTION:  SafeMalloc */
/**/
/*    INPUTS:  size is the size to malloc */
/**/
/*    OUTPUT:  returns pointer to allocated memory if succesful */
/**/
/*    EFFECT:  mallocs new memory.  If malloc fails, prints error message */
/*             and terminates program. */
/**/
/*    Modifies Input: none */
/**/
/***********************************************************************/

void *SafeMalloc(size_t size) {
  void *result;

  if ((result = malloc(size))) { /* assignment intentional */
    return (result);
  } else {
    printf("memory overflow: malloc failed in SafeMalloc.");
    printf("  Exiting Program.\n");
    exit(-1);
    return (0);
  }
}
/*  NullFunction does nothing it is included so that it can be passed */
/*  as a function to RBTreeCreate when no other suitable function has */
/*  been defined */

void NullFunction(void *junk) { ; }
