#if !defined(CLUSTERLEGACYGUARD)
#define CLUSTERLEGACYGUARD

#include <stdlib.h>
#include <float.h>
#include "../dataTypes.h"

int** kmeans(container*, int, int);

void findMaxMin(container*, int, double*, double*, double*, double*);
double calcDistanceCoord(double, double, double, double);

centroid *initCentroidRandom(int, double, double, double, double);
void assignNodesToCentroids(centroid*, int, container*, int);
void assignNodesNaiveXmax(centroid*, int, container*, int);
void standardAssignSingleNodeToCentroid(centroid*, int, container*, int, double, double*);
void naiveXmaxAssignSingleNodeToCentroid(centroid*, int, container*, int, double, double*);
#endif // CLUSTERGUARD