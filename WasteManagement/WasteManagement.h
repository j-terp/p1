#if !defined(MAINGUARD)
#define MAINGUARD

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../MatrixModule/MatrixOperations.h"
#include "../CSVModule/CSVtoMatrix.h"
#include "../ConvertGraph/AsymGraphToSym.h"
#include "../CSVModule/ReadLatLon/readlatLon.h"
#include "../CSVModule/CSVtoDoubleList/CSVToList.h"
#include "../ClusterModule/Cluster.h"
#include "../FilterModule/MatrixFilter.h"
#include "../dataTypes.h"
#include "../LinkedLists/List.h"
#include "../CSVModule/matrixToCSV.h"
#include "../TSP/tspModule.h"
#include "../Simulation/SimModule.h"

#define TEST_MODE 0
#define TRUCK_LOAD 11
#define DAYS_IN_YEAR 365
#define ORIGIN_PATH "./data/LatLon.csv"
#define LAT_LON_PATH (TEST_MODE ? "./data/testLatLon.csv" : "./data/LatLon.csv")
#define FILL_LIST_PATH (TEST_MODE ? "./data/Fyldningsrater/fillRateTest.csv" : "./data/Fyldningsrater/fyldningsgrad5.csv")

int** calculateRoutes(container* containerList, int containerListSize, int *numRoutes, double** distanceMatrix, int ifOutputFile);

#endif // MAINGUARD
