#if !defined(FILTERGUARD)
#define FILTERGUARD
#include <stdio.h>
#include <stdlib.h>
#include "../dataTypes.h"
#include "../MatrixModule/MatrixOperations.h"
#include "../ClusterModule/Cluster.h"
int filterFillRateList(container *containerList, int *curatedList, int listLen, int *curatedLen, int truckLoad);
container* filterContainerList(container* fullList, int* IDList, int numIDs);
double** filterDistanceMatrix(double** matrix, int* filterList, int listLen);
#endif // FILTERGUARD
