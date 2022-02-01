#if !defined(ASYMGUARD)
#define ASYMGUARD
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#define INF DBL_MAX
#define M 2
#include "../MatrixModule/MatrixOperations.h"


double** convertToSymGraph(double** matrix, int rowTotal, int columnTotal, int *symMatrixRowTotal, int *symMatrixColumnTotal);
double** createModifiedGraph(double** graph, int rowTotal, int columnTotal);

#endif // ASYMGUARD
