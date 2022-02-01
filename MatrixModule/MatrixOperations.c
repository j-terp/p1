/**
 * @file MatrixOperations.c
 * @brief Matrix operation functions
 * 
 */

#include "MatrixOperations.h"

/**
 * @brief allocates room for a Double matrix of size rows x columns
 * 
 * @param rowTotal 
 * @param columnTotal 
 * @return double** 
 */
double** allocDoubleMatrix(int rowTotal, int columnTotal) {
    double **matrix = (double **)malloc(sizeof(double *) * rowTotal);
    for (int i = 0; i < rowTotal; i++) {
        matrix[i] = (double *)malloc(sizeof(double) * columnTotal);
    }
    return matrix;
}

/**
 * @brief Allocates Int Matrix
 * 
 * @param rowTotal 
 * @param columnTotal 
 * @return int** 
 */
int **allocIntMatrix(int rowTotal, int columnTotal) {
    int **matrix = (int **)malloc(sizeof(int *) * rowTotal);
    for (int i = 0; i < rowTotal; i++) {
        matrix[i] = (int *)malloc(sizeof(int) * columnTotal);
    }
    return matrix;
}

/**
 * @brief Prints Matrix
 * 
 * @param matrix 
 * @param rowTotal 
 * @param columnTotal 
 */
void printMatrix(double **matrix, int rowTotal, int columnTotal) {
    for (int i = 0; i < rowTotal; i++) {
        for (int j = 0; j < columnTotal; j++) {
            if(matrix[i][j] == INF) {
                printf("  INF ");
            }
            else {
                printf("%5.4lf ", matrix[i][j]);
            }
        }
        printf("\n");
    }
}

/**
 * @brief Free Double Matrix
 * 
 * @param matrix 
 * @param rowTotal 
 */
void freeDoubleMatrixPtr(double **matrix, int rowTotal) {
    for (int i = 0; i < rowTotal; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

/**
 * @brief Free Int Matrix
 * 
 * @param matrix 
 * @param rowTotal 
 */
void freeIntMatrixPtr(int **matrix, int rowTotal) {
    for (int i = 0; i < rowTotal; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

/**
 * @brief Transposes Quadtratic Matrix
 * 
 * @param matrix 
 * @param transposedMatrix 
 * @param rowTotal 
 */
void transposeQuadraticMatrix(double** matrix, double** transposedMatrix, int rowTotal) {
    int column;
    for (int i = 0; i < rowTotal; i++) {
        column = 0;
        // this makes sure we dont double swap
        while (column < i) {
            // this will swap the position of x,y coordinates
            transposedMatrix[i][column] = matrix[column][i];
            transposedMatrix[column][i] = matrix[i][column];
            column++;
        }
        if (i == column) {
            transposedMatrix[i][i] = matrix[i][i];
        }
    }  
}
