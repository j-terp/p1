/**
 * @file CSVToList.c
 * @brief list from CSV conversion functions
 * 
 */

#include "CSVToList.h"

/**
 * @brief Create a Fill Rate list From File object
 * 
 * @param filePath 
 * @param numContainers 
 * @return double* 
 */
double* createFillRateListFromFile(char* filePath, int numContainers) {
    double* fillRateList = (double*)malloc(sizeof(double)*numContainers);
    FILE* csvFile = fopen(filePath, "r");
    for (int i = 0; i < numContainers; i++) {
        fillRateList[i] = fetchCellData(csvFile);
    }
    fclose(csvFile);
    return fillRateList;
}
/**
 * @brief inserts fill rates into container array
 * 
 * @param filePath 
 * @param containerList 
 * @param numContainers 
 */
void insertFillrateFromFile(char* filePath, container* containerList, int numContainers) {
    FILE* csvFile = fopen(filePath, "r");
    for (int i = 0; i < numContainers; i++) {
        containerList[i].filledRate = fetchCellData(csvFile);
    }
    fclose(csvFile);
}