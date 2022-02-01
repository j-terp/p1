/**
 * @file AsymGraphToSym.c
 * @brief Asymmetrical graph functions
 * 
 */

#include "AsymGraphToSym.h"

/**
 * @brief Takes an asymmetrical graph and returns it as symmetric
 * 
 * @param matrix 
 * @param rowTotal 
 * @param columnTotal 
 * @param symMatrixRowTotal 
 * @param symMatrixColumnTotal 
 * @return double** 
 */
double** convertToSymGraph(double** matrix, int rowTotal, int columnTotal, int *symMatrixRowTotal, int *symMatrixColumnTotal) {
    *symMatrixRowTotal = 2 * rowTotal;
    *symMatrixColumnTotal = 2 * columnTotal;
    double** transposedMatrix = allocDoubleMatrix(rowTotal, columnTotal);
    transposeQuadraticMatrix(matrix, transposedMatrix, rowTotal);
    double** symMatrix = allocDoubleMatrix(*symMatrixRowTotal, *symMatrixColumnTotal);
    // in this new matrix, the middle i will be the number of rows, and the middle j will be the original number of columns
    for (int i = 0; i < *symMatrixRowTotal; i++) {
        for (int j = 0; j < *symMatrixColumnTotal; j++) {
            if (i < rowTotal) {
                /*if we are above the middle, we need to translate this index to an index smaller than the middle value
                this is done by subtracting columns from j so for example j 5 will be 5-4=> 1 if there are 4 columns. 
                after this we can look up the value in either the transposed matrix or the original matrix*/
                symMatrix[i][j] = (j < columnTotal) ? (INF) : (transposedMatrix[i][j - columnTotal]);
            }
            else { // This means we are above the middle index
            symMatrix[i][j] = (j < columnTotal) ? (matrix[i - rowTotal][j]) : (INF);
            }
        }
    }
    freeDoubleMatrixPtr(transposedMatrix, rowTotal);
    return symMatrix;
}

/**
 * @brief Create a Modified Graph object, where the diagonal points are -M
 * 
 * @param graph 
 * @param rowTotal 
 * @param columnTotal 
 * @return double** 
 */
double** createModifiedGraph(double** graph, int rowTotal, int columnTotal) {
    double** modifiedGraph = allocDoubleMatrix(rowTotal, columnTotal);
    for (int i = 0; i < rowTotal; i++){
        for (int j = 0; j < columnTotal; j++){
            modifiedGraph[i][j] = (i == j) ? (-1 * M) : (graph[i][j]); 
        }
    }
    return modifiedGraph;
}
