enum result {
  KEY_NOT_FOUND = 777,
  NO_PRED_OR_SUCC,
  FOUND
};

struct elt_t {
  int val;
  void *info;
};

void containerCreate (void);
void containerInsert (int, void *);
int containerFind (int);
void containerDelete (int);
int containerPred (int, int *);
int containerSucc (int, int *);
void containerPrint (void);
int containerStart (void);
int containerStartVal (int, int);
int containerNextVal (int, int);
int containerNext (int);
struct elt_t containerGet (int);
int containerRandom (int *);
