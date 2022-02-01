#if !defined(CSVMATRIXGUARD)
#define CSVMATRIXGUARD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>
#include "../MatrixModule/MatrixOperations.h"

#define DELIMETER ','
#define EARTH_RADIUS 6371
#define RADIAN M_PI / 180

double** createMatrixFromFile(char filePath[], int *rowTotal, int *columnTotal);
double distanceInKmBetweenEarthCoordinates(double lat1, double lon1, double lat2, double lon2);
double fetchCellData(FILE *);

#endif // CSVMATRIXGUARD
