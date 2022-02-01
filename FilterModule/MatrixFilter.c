/**
 * @file MatrixFilter.c
 * @brief 
 * 
 */

#include "MatrixFilter.h"

/**
 * @brief Filters Fill Rate list to a multiple of truckload, with one full iterations at least. Returns length of list compared to truckload
 * 
 * @param fillRateList 
 * @param curatedList 
 * @param listLen 
 * @param curatedLen 
 * @param truckLoad 
 * @return int 
 */
int filterFillRateList(container *containerList, int *curatedList, int listLen, int *curatedLen, int truckLoad) {
    int done = 0;
    double lowerBound = 0.8, upperBound = 1;
    *curatedLen = 0;
    do {
        for (int i = 0; i < listLen; i++) {
            if (containerList[i].filledRate > lowerBound && containerList[i].filledRate <= upperBound) {
                if (done && *curatedLen % truckLoad == 0) {
                    break;
                }
                curatedList[*curatedLen] = containerList[i].id;
                (*curatedLen)++;
            }
        }
        upperBound = lowerBound;
        lowerBound -= 0.05;
        done = 1;
        // prevents infinite loop on data with less than 10 data points
        if (lowerBound < 0){
            break;
        }
    } while (*curatedLen % truckLoad != 0);
    *curatedLen = (*curatedLen / truckLoad) * truckLoad;    //Tilføjet efter aflevering
    return *curatedLen / truckLoad;
}

/**
 * @brief Filters Container list using a list of IDs
 * 
 * @param fullList 
 * @param IDList 
 * @param numIDs 
 * @return container* 
 */
container* filterContainerList(container* fullList, int* IDList, int numIDs) {
    container* mappedList = (container*)malloc(sizeof(container)*numIDs);
    memoryCheck(mappedList);
    for (int i = 0; i < numIDs; i++) {
       mappedList[i] = fullList[IDList[i]]; 
    }
    return mappedList;
}

/**
 * @brief FIlters Distance Matrix using a list of IDs
 * 
 * @param matrix 
 * @param filterList 
 * @param listLen 
 * @return double** 
 */
double** filterDistanceMatrix(double** matrix, int* filterList, int listLen) {
    double** filteredMatrix = allocDoubleMatrix(listLen, listLen);
    for (int i = 0; i < listLen; i++) {
        int id1 = filterList[i];
        for (int j = 0; j < listLen; j++) {
            int id2 = filterList[j];
            filteredMatrix[i][j] = matrix[id1][id2];
        }
    }
    return filteredMatrix;
}
