#if !defined(PRIM_GUARD)
#define PRIM_GUARD

    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    #include <stdbool.h>
    #include <string.h>
    #include <float.h>

    #include "../MatrixModule/MatrixOperations.h"
    #include "../ConvertGraph/AsymGraphToSym.h"

    #define STARTING_NODE 0

    int *approximateShortestRoute(double **matrix, int matrixSize);
    void printRoute(int routeIndex, int matrixSize, int *result);
    int* turnLocalIdsToGlobalAlloc(int* result, int* clusters, int clusterSize);
    
#endif // PRIM_GUARD

