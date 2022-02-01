#if !defined(SIMGUARD)
#define SIMGUARD

    #include<stdio.h>
    #include<stdlib.h>
    #include<math.h>
    #include<time.h>
    #include <unistd.h>
    #include "../WasteManagement/WasteManagement.h"
    #include "../TraditionalRoutes/traditionalRoutes.h"
    #include "../CSVModule/matrixToCSV.h" 
    #include "../dataTypes.h"
    /* values are in % filled */
    #define MAX_FILLED_PER_DAY 17
    #define MIN_FILLED_PER_DAY 1
    #define AVERAGE_RATE_PER_USER 0.0001
    #define CUBIC_METER_IN_CONTAINER 30


    void stepContainers(container *containerList, int containerSize, double wastePerUser);
    simulationResult simulateTraditionalRoutes (double** distanceMatrix, container* containerList, int numContainers, double wastePerUser);
    simulationResult simulateNewRoutes (double** distanceMatrix, container* containerList, int numContainers,double wastePerUser);
    void emptyAndCountUpDistanceAndWaste (double **distanceMatrix, container *containerList, int **routes, int *filteredRoutes, int numRoutes, int *containersAtRouteIndex, simulationResult *simResult);
    void printResult (simulationResult result);

#endif // SIMGUARD


