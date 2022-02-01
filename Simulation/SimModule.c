#include "SimModule.h"
#define DEPONI_LAT 56.12277726837733
#define DEPONI_LON 8.938142168041606


/* Returns true if container overlow */
void stepContainers(container *containerList, int containerSize, double wastePerUser) {
    for (int i = 0; i < containerSize; i++) {
        containerList[i].filledRate += (containerList[i].users * wastePerUser)/* * (randBetween(95,120)/100) */;
        if(containerList[i].filledRate > 1){
            containerList[i].filledRate = 1;
        }
    }
}

simulationResult simulateTraditionalRoutes (double** distanceMatrix, container* containerList, int numContainers, double wastePerUser) {
    simulationResult result;
    result.totalCost = 0;
    result.totalGarbageCollected = 0;
    result.totalContainerOverflow = 0;
    //Loops after week 16, to calculate if it is to be emptied do: if (week % 16) + 1 == weekToEmpty[i] then empty route.
    int numTraditionalRoutes = 16;
    int *weekToEmptyArray = (int*)malloc(sizeof(int) * numTraditionalRoutes);
    int *containersAtRouteIndex = (int*)malloc(sizeof(int) * numTraditionalRoutes);
    int **routes = loadTraditionalRoutes(numTraditionalRoutes, weekToEmptyArray, containersAtRouteIndex, "./data/TraditionalRoutes.csv");

    for(int i = 0; i < DAYS_IN_YEAR; i++) {
        stepContainers(containerList, numContainers, wastePerUser);
        int *filteredRoutes;
        int numFilteredRoutes = 0;
        filteredRoutes = filterTraditionalRoutes(routes, weekToEmptyArray, containersAtRouteIndex, numTraditionalRoutes, &numFilteredRoutes, i);
        emptyAndCountUpDistanceAndWaste(distanceMatrix, containerList, routes, filteredRoutes, numFilteredRoutes, containersAtRouteIndex, &result);
        free(filteredRoutes);
    }
    freeIntMatrixPtr(routes, numTraditionalRoutes);
    free(weekToEmptyArray);
    free(containersAtRouteIndex);
    return result;
}

simulationResult simulateNewRoutes (double** distanceMatrix, container* containerList, int numContainers,double wastePerUser) {
    simulationResult result;
    result.totalCost = 0;
    result.totalGarbageCollected = 0;
    result.totalContainerOverflow = 0;
    int *filteredList = (int *)malloc(sizeof(int) * numContainers);
    int filteredListLength;
    for(int i = 0; i < DAYS_IN_YEAR; i++) {
        stepContainers(containerList, numContainers, wastePerUser);                
        container* filteredContainerList;
        int numRoutes = filterFillRateList(containerList, filteredList, numContainers, &filteredListLength, TRUCK_LOAD);
        if (numRoutes > 0) {
            filteredContainerList = filterContainerList(containerList, filteredList, filteredListLength);
            int** routes = calculateRoutes(filteredContainerList, filteredListLength, &numRoutes, distanceMatrix, 0);
            int* filteredRoutes = (int*)malloc(sizeof(int) * numRoutes);
            int *containersAtRouteIndex = (int*)malloc(sizeof(int) * numRoutes);

            for (int j = 0; j < numRoutes; j++) {
                filteredRoutes[j] = j;
                containersAtRouteIndex[j] = TRUCK_LOAD;
            }

            emptyAndCountUpDistanceAndWaste(distanceMatrix, containerList, routes, filteredRoutes, numRoutes, containersAtRouteIndex, &result);
            freeIntMatrixPtr(routes, numRoutes);
            free(filteredRoutes);
            free(containersAtRouteIndex);
        }
    }
    free(filteredList);
    return result;
}

void emptyAndCountUpDistanceAndWaste (double **distanceMatrix, container *containerList, int **routes, int *filteredRoutes, int numRoutes, int *containersAtRouteIndex, simulationResult *simResult) {
    for (int i = 0; i < numRoutes; i++) {
        int routeIndex = filteredRoutes[i];
        for (int j = 0; j < containersAtRouteIndex[routeIndex]; j++) {
            //Hvis sidste punkt kør til start.
            int nextId = ((j+1 == containersAtRouteIndex[routeIndex]) ? -1 : j+1);
            if (nextId != -1) {
                simResult->totalCost += distanceMatrix[routes[routeIndex][j]]   [routes[routeIndex][nextId]];
                double filledRate = containerList[routes[routeIndex][j]].filledRate;
                simResult->totalGarbageCollected += filledRate;
                if (filledRate == 1) {
                    (simResult->totalContainerOverflow)++;
                }
                containerList[routes[routeIndex][j]].filledRate = 0;
            } else {
                //Find distance fra punkt til deponi...
                container *first = &(containerList[routes[routeIndex][0]]);
                container *last = &(containerList[routes[routeIndex][containersAtRouteIndex[routeIndex] - 1]]);
                simResult->totalCost += distanceInKmBetweenEarthCoordinates(first->lat, first->lon, DEPONI_LAT, DEPONI_LON);
                simResult->totalCost += distanceInKmBetweenEarthCoordinates(last->lat, last->lon, DEPONI_LAT, DEPONI_LON);
            }
        }
    }
}

void printResult (simulationResult result) {
    printf("The simulation drove: %.4lf km, and collected %.4lf times the volume of a container worth"
        " of trash in a year. That is %lf waste/km. And %d containers overflowed.\n",
        result.totalCost, result.totalGarbageCollected, result.totalGarbageCollected/result.totalCost, result.totalContainerOverflow);
}
