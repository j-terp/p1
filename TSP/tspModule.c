/**
 * @file tspModule.c
 * @brief TSP functions
 * 
 */

#include "tspModule.h"

static void primAlgorithm(double **primMatrix, int matrixSize, int currentNode, int **treeEdges, bool *visited);
static void getNextEdge(int matrixSize, bool visited[], double **primMatrix, double *lowestEdgeCost, int *nextNode, int *fromNode);
static void preorderWalk(int *route, int matrixSize, int **treeEdges);

/**
 * @brief Approximates the shortest route and cost from given matrix, returns cost of route.
 * 
 * @param matrix
 * @param matrixSize
 * @return int*
 */
int *approximateShortestRoute(double **matrix, int matrixSize) {
    double **primMatrix = allocDoubleMatrix(matrixSize, matrixSize);
    int currentNode = STARTING_NODE;
    int **treeEdges = allocIntMatrix(matrixSize - 1, 2);
    int *route = (int*)malloc(sizeof(int) * (matrixSize / 2));
    bool *visited = (bool *)malloc(sizeof(bool) * matrixSize);

    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            primMatrix[i][j] = matrix[i][j];
        }
        visited[i] = false;
        // Arbitrarily picked row is the root, where the elements are set to INF
        primMatrix[STARTING_NODE][i] = INF;
    }
    visited[STARTING_NODE] = true;
    route[0] = STARTING_NODE;
    treeEdges[0][0] = STARTING_NODE;

    primAlgorithm(primMatrix, matrixSize, currentNode, treeEdges, visited);

    /* We are handling a matrix twice the size of the original matrix because of assym to symm conversion. 
       Thus, we must remove all points visited with an index greater than half the size of the matrixSize. */
    preorderWalk(route, matrixSize, treeEdges);
       
    freeDoubleMatrixPtr(primMatrix, matrixSize);
    free(visited);
    freeIntMatrixPtr(treeEdges, 2);

    return route;
}

/**
 * @brief Constructs nodes based on new values of fromNode and nextNode and sets the currentNode value to the newly obtained value of nextNode
 * 
 *  @param primMatrix 
 * @param matrixSize 
 * @param currentNode 
 * @param treeEdges 
 * @param visited 
 */
static void primAlgorithm(double **primMatrix, int matrixSize, int currentNode, int **treeEdges, bool *visited) {
    double lowestEdgeCost; 
    int nextNode = STARTING_NODE;
    int fromNode = STARTING_NODE;

    for (int i = 0; i < matrixSize - 1; i++){
        // Start from arbitrary node. Just start from the first node.
        lowestEdgeCost = primMatrix[0][currentNode];

        getNextEdge(matrixSize, visited, primMatrix, &lowestEdgeCost, &nextNode, &fromNode);

        // Replace every element in the row with key-value.
        for (int j = 0; j < matrixSize; j++) {
            primMatrix[nextNode][j] = INF;
        }
        treeEdges[i][0] = fromNode;
        treeEdges[i][1] = nextNode;

        currentNode = nextNode;
    }
}

/**
 * @brief Gets the lowestEdgeCost edge by iterating all columns of visited nodes, marks new visited node as visited.
 * 
 * @param matrixSize 
 * @param visited 
 * @param primMatrix 
 * @param lowestEdgeCost 
 * @param nextNode 
 * @param fromNode 
 */
static void getNextEdge(int matrixSize, bool visited[], double **primMatrix, double *lowestEdgeCost, int *nextNode, int *fromNode) {
    for (int i = 0; i < matrixSize; i++) {
        if (visited[i]) {
            for (int j = 0; j < matrixSize; j++) {
                if (primMatrix[j][i] < *lowestEdgeCost) {
                    *lowestEdgeCost = primMatrix[j][i];
                    *nextNode = j;
                    *fromNode = i;
                }
            }
        }
    }
    visited[*nextNode] = true;
}

/**
 * @brief Performs a preorder walk of all edges with an index less than half of the original matrix. The minimum segment tree is then converted to a hamiltonian circuit.
 * 
 * @param route 
 * @param matrixSize 
 * @param treeEdges 
 */
static void preorderWalk(int *route, int matrixSize, int **treeEdges) {
    int j = 1;
    for (int i = 0; i < matrixSize - 1; i++){
        if (treeEdges[i][1] < matrixSize / 2){
            route[j] = treeEdges[i][1];
            j++;
        }
    }
}

/**
 * @brief Prints the hamiltonian circuit from result array
 * 
 * @param routeIndex 
 * @param matrixSize 
 * @param route 
 */
void printRoute(int routeIndex, int matrixSize, int *route) {
    printf("\nRoute %2d: ", routeIndex + 1);
    for (int i = 0; i < (matrixSize / 2); i++) {
        printf("%3i", route[i]);
        if (i != matrixSize / 2 - 1) {
            printf(" -> ");
        }
    }
}

/**
 * @brief Converts local route elements to global elements
 * 
 * @param route 
 * @param cluster 
 * @param clusterSize 
 * @return int*
 */
int* turnLocalIdsToGlobalAlloc(int *route, int *clusters, int clusterSize) {
    int *globalIdList = (int *)malloc(sizeof(int) * clusterSize);
    for (int i = 0; i < clusterSize; i++) {
        globalIdList[i] = clusters[route[i]];
    }
    return globalIdList;
}
