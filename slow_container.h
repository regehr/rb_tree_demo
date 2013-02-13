enum result {
  KEY_NOT_FOUND = 777,
  NO_PRED_OR_SUCC,
  FOUND
};

void slowContainerCreate (void);
void slowContainerInsert (int);
int slowContainerFind (int);
void slowContainerDelete (int);
int slowContainerPred (int, int *);
int slowContainerSucc (int, int *);
void slowContainerPrint (void);
int slowContainerStart (void);
int slowContainerStartVal (int, int);
int slowContainerNextVal (int, int);
int slowContainerNext (int);
int slowContainerGet (int);
int slowContainerRandom (int *);
