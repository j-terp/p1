#include "WasteManagement.h"

int** calculateRoutes(container* containerList, int containerListSize, int *numRoutes, double** distanceMatrix, int ifOutputFile) {
    //container *filteredContainerList;
    int **clusters;
    int **routes;
    int *tempRoute;
    //Clustering 
    clusters = kmeansSameSize(containerList, containerListSize, *numRoutes, TRUCK_LOAD);
    // Prints clusters to outputfile
    

    //TSP-implementation
    routes = (int**)malloc(sizeof(int*) * (*numRoutes));
    FILE* clusterOutput = fopen("./data/clusterOutput.csv","w");
    for (int i = 0; i < *numRoutes; i++) {
        if (ifOutputFile == 1) {
            //free(filteredContainerList);
        }
        int newNumRows = 0;
        int newNumColumns = 0;
        double **clusterMatrix = filterDistanceMatrix(distanceMatrix, clusters[i], TRUCK_LOAD);
        //printf("\n");
        //printMatrix(clusterMatrix, TRUCK_LOAD, TRUCK_LOAD);
        double **clusterModifiedMatrix = createModifiedGraph(clusterMatrix, TRUCK_LOAD, TRUCK_LOAD);
        double **clusterSymMatrix = convertToSymGraph(clusterModifiedMatrix, TRUCK_LOAD, TRUCK_LOAD, &newNumRows, &newNumColumns);
        tempRoute = approximateShortestRoute(clusterSymMatrix, newNumRows);
        routes[i] = turnLocalIdsToGlobalAlloc(tempRoute, clusters[i], TRUCK_LOAD);
        free(tempRoute);
        freeDoubleMatrixPtr(clusterMatrix, TRUCK_LOAD);
        freeDoubleMatrixPtr(clusterModifiedMatrix, TRUCK_LOAD);
        freeDoubleMatrixPtr(clusterSymMatrix, newNumRows);
    }
    printNodesToFile(clusterOutput, containerList, containerListSize);
    fclose(clusterOutput);
    freeIntMatrixPtr(clusters, *numRoutes);
    return routes;
}