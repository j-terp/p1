/**
 * @file main.c
 * @author B207
 * @brief Simulates traditional routes or simulates routes created using filtering, clustering and a TSP-algorithm
 * @version 1.0
 * @date 2021-17-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* typedef enum userControl{Exit, CalcRoutes, SimulateTraditional, SimulateNew} userControl; */

#include "./Simulation/SimModule.h"
#include "./WasteManagement/WasteManagement.h"
#include "./TraditionalRoutes/TraditionalRoutes.h"
#include "./CSVModule/matrixToCSV.h"

int main (void) {
    int programRunning = 1;
    char *distMatrixFilepath = ORIGIN_PATH;
    char *latLonFilePath = LAT_LON_PATH;
    double **distanceMatrix;
    container* containerList;
    int dataRows;
    int dataColumns;
    int answer;
    int filteredListLength;
    

    srand(time(NULL));
    make_directory("Output");

    while (programRunning) {
        printf("\n"
               "Choose\n"
               "1 Calculate routes from file?\n"
               "2 Simulate traditional routes?\n"
               "3 Simulate new routes?\n"
               "(Any other button exits the program)\n");
        answer = 0;
        scanf("%d", &answer);
        fflush(stdin);
        
        if (answer == 1) {
            container* filteredContainerList;
            // Data input from CSV
            distanceMatrix = createMatrixFromFile(distMatrixFilepath, &dataRows, &dataColumns);
            containerList = createCoordStructListFromFile(latLonFilePath, dataRows);
            insertFillrateFromFile(FILL_LIST_PATH, containerList, dataRows);
            int *filteredList = (int *)malloc(sizeof(int) * dataRows);
            int numRoutes = filterFillRateList(containerList, filteredList, dataRows, &filteredListLength, TRUCK_LOAD);
            filteredContainerList = filterContainerList(containerList, filteredList, filteredListLength);
            int** routes = calculateRoutes(filteredContainerList, filteredListLength, &numRoutes, distanceMatrix, 1);
            for(int i = 0; i < numRoutes; i++){
                outputCSV(routes[i],i,TRUCK_LOAD);
            }
            freeIntMatrixPtr(routes, numRoutes);
        }
        else if (answer == 2 || answer == 3) {
            // Data input from CSV
            distanceMatrix = createMatrixFromFile(distMatrixFilepath, &dataRows, &dataColumns);
            containerList = createCoordStructListFromFile(latLonFilePath, dataRows);
            setUsersBasedOnEmptyFrequency(containerList, "data/afhentning.csv", dataRows);
            printf("Enter year: ");
            int year = 0;
            scanf("%d", &year);
            for (int i = 0; i < year; i++) {
                double wastePerUser = AVERAGE_RATE_PER_USER * (i+1);
                if (answer == 2) {
                    simulationResult simResult = simulateTraditionalRoutes(distanceMatrix, containerList, dataRows, wastePerUser);
                    printResult(simResult);
                }
                else if (answer == 3) {
                    simulationResult simResult = simulateNewRoutes(distanceMatrix, containerList, dataRows, wastePerUser);
                    printResult(simResult);
                }
            }
            free(containerList);
            freeDoubleMatrixPtr(distanceMatrix, dataRows);
        }
         else {
            programRunning = false;
        }
    }

    return EXIT_SUCCESS;
}
