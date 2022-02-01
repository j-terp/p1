#if !defined(CLUSTERGUARD)
#define CLUSTERGUARD

#include <stdlib.h>
#include<stdio.h>
#include <float.h>
#include<math.h>
#include "../dataTypes.h"
#include "../LinkedLists/List.h"

#define DEBUG_OUT_FILE "./Clustermodule/rounds/debug.csv"

int** kmeansSameSize(container* containerList, int numContainers, int numCluster, int clusterSize);

centroid* kppCentroidInit(int numCentroids, container* nodeList, int numNodes);
void printToFile (centroid* centroidList, int numCluster, container *containerList, int numContainers);
int findBestNodeToInitCentroid (centroid *centroidList, int* hasBeenChosen, int numNodes, container* nodeList, int numAssigned);
void kppCentroidAssign(centroid* centroidList, int centroid, container* nodeList, int node, int* chosenList, int* count);
void centroidElementInit(centroid* centroidList, int numCentroids);
void moveCentroids(centroid*, int, container*, int);

clusterElement* elementInit(container* nodeList, int numNodes);
void sortElementList(clusterElement* elementList, int numElements, centroid* centroidList, int numCentroids);
int compareDistanceElements(const void* ptr1, const void* ptr2);

void assignSortedNodes(clusterElement* elementList, int numElements, centroid* centroidList, int numCentroids, int clusterSize);
void nodeOptimiseSwitch(centroid* centroidList, int numCentroids, clusterElement* elementList, int numElements, int* globalMoved, /* DEBUG ONLY */container *containerList);
double calcDistBetweenNodeAndCent(clusterElement element, centroid centElem);
double calcDistanceCoord(double x1, double x2, double y1, double y2);
double calcBestAltDeltaDist(clusterElement element, centroid* centroids, int numCentroids);
int evalSwapTransferList(List* transferList, clusterElement* elementList,int elemIndex,centroid* centroidList);
int compareClusterElements(const void* ptr1, const void* ptr2);
int compareClusterElementsReverse(const void* ptr1, const void* ptr2);
void centroidPrint(centroid* centroidList, int numCentroids);
void memoryCheck(void* ptr);

void printNodesToFile(FILE *printFile, container* containers, int listSize);
void printCentsToFile(FILE *printFile, centroid* centroids, int listSize);

#endif // CLUSTERGUARD