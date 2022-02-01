#if !defined(TRADITIONALGUARD)
#define TRADITIONALGUARD

    #include <stdio.h>
    #include <stdlib.h>
    #include "../dataTypes.h"
    #include "../MatrixModule/MatrixOperations.h"

    #define EMPTY_FREQUENCY "data/afhentning.csv"
    #define ROUTE_ENTRIES "./data/TraditionalRoutes.csv"

    int** loadTraditionalRoutes(int numRoutes, int* weekToEmptyArray, int* containersAtRouteIndex, char *filePath);
    int* filterTraditionalRoutes(int **traditionalroutes, int *weekToEmptyArray, int *containersAtRouteIndex, int numTraditionalRoutes, int *numRoutes, int dayOfTheYear);
    void setUsersBasedOnEmptyFrequency (container* containerList, char* filePath, int numContainer);

#endif // TRADITIONALGUARD
