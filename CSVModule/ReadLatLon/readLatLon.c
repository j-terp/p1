/**
 * @file readLatLon.c
 * @brief Coord Struct functions
 * 
 */

#include "readlatLon.h"
#include "../../Simulation/SimModule.h"

/**
 * @brief Create a Coord Struct list From File object
 * 
 * @param filePath 
 * @param numContainers 
 * @return container* 
 */
container* createCoordStructListFromFile(char* filePath, int numContainers){
    container* containerList = (container*) malloc(numContainers*sizeof(container));
    FILE* openFile = fopen(filePath, "r");
    for (int i = 0; i < numContainers; i++){
        containerList[i].lat = fetchCellData(openFile);
        containerList[i].lon = fetchCellData(openFile);
        containerList[i].id = i;
        containerList[i].group = 0; // Initial group assignment
        containerList[i].filledRate = 0.7;
        containerList[i].users = 0;
    }
    fclose(openFile);
    return containerList;
}

/**
 * @brief Prints Location and ID from an array
 * 
 * @param containerList 
 * @param numContainers 
 */
void printLocAndID(container* containerList, int numContainers){
    for (int i = 0; i < numContainers;i++){
        printf("container %i: lat: %lf lon : %lf \n", containerList[i].id, containerList[i].lat, containerList[i].lon);
    }
}