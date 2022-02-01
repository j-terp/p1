#if !defined(CSVTOLISTGUARD)
#define CSVTOLISTGUARD
#include "../CSVtoMatrix.h"
#include <stdlib.h>
#include "../../dataTypes.h"
double* createFillRateListFromFile(char* filePath, int numContainers);
void insertFillrateFromFile(char* filePath, container* containerList, int numContainers);
#endif // CSVTOLISTGUARD