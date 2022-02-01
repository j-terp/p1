#if !defined(MATRIXOPERATIONGUARD)
#define MATRIXOPERATIONGUARD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "../ConvertGraph/AsymGraphToSym.h"

double** allocDoubleMatrix(int rowTotal, int columnTotal);
int **allocIntMatrix(int rowTotal, int columnTotal);
void printMatrix(double **matrix, int rowTotal, int columnTotal);
void freeDoubleMatrixPtr(double **matrix, int rowTotal);
void freeIntMatrixPtr(int **matrix, int rowTotal);
void transposeQuadraticMatrix(double** matrix, double** transposedMatrix, int rowTotal);

#endif // MATRIXOPERATIONGUARD
