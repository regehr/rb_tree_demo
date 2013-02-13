enum result {
  KEY_NOT_FOUND = 777,
  NO_PRED_OR_SUCC,
  FOUND
};

void containerCreate (void);
void containerInsert (int);
int containerFind (int);
void containerDelete (int);
int containerPred (int, int *);
int containerSucc (int, int *);
void containerPrint (void);
int containerStart (void);
int containerStartVal (int, int);
int containerNextVal (int, int);
int containerNext (int);
int containerGet (int);
int containerRandom (int *);
