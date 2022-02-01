#include "TraditionalRoutes.h"

int** loadTraditionalRoutes(int numRoutes, int* weekToEmptyArray, int* containersAtRouteIndex, char *filePath) {
    int** routes = allocIntMatrix(numRoutes, 40);
    for (int i = 0; i < numRoutes; i++) {
        containersAtRouteIndex[i] = 0;
    }
    FILE *RouteFile = fopen(filePath, "r");
    int numRouteEntries = 0;
    char buffer[20];
    while (fgets(buffer, 20, RouteFile) != NULL) {
        numRouteEntries++;
    }
    rewind(RouteFile);
    for (int i = 0; i < numRouteEntries; i++) {
        int containerIndex = 0;
        int route = 0;
        int routeIndex = 0;
        int weekToEmpty = 0;
        if ((fscanf(RouteFile,"%d,%d,%d,%d", &containerIndex, &route, &routeIndex, &weekToEmpty)) == 4) {
            routes[route][routeIndex] = containerIndex;
            weekToEmptyArray[route] = weekToEmpty;
            containersAtRouteIndex[route]++;
        }
    }
    fclose(RouteFile);
    return routes;
}

int *filterTraditionalRoutes(int **traditionalroutes, int *weekToEmptyArray, int *containersAtRouteIndex, int numTraditionalRoutes, int *numRoutes, int dayOfTheYear) {
    int *filteredRoutes = (int*)malloc(sizeof(int) * numTraditionalRoutes);
    *numRoutes = 0;
    int week = (dayOfTheYear / 7);
    for (int i = 0; i < numTraditionalRoutes; i++) {
        if ((week % numTraditionalRoutes) == weekToEmptyArray[i] && dayOfTheYear % 7 == 6) {
            filteredRoutes[*numRoutes] = i;
            (*numRoutes)++;
        }
    }
    return filteredRoutes;
}

void setUsersBasedOnEmptyFrequency (container* containerList, char* filePath, int numContainer) {
    FILE* file = fopen(filePath, "r");
    for (int i = 0; i < numContainer; i++) {
        int scanResult;
        fscanf(file, "%d", &scanResult);
        containerList[i].users = 1000 / scanResult;
    }
    fclose(file);
}