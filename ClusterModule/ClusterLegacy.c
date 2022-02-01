/**
 * @file ClusterLegacy.c
 * @brief Includes the functions for executing K-means clustering
 * 
 */

#include "ClusterLegacy.h"

/**
 * @brief Does k meansclusters based on a list of struct ContainerLoc
 * 
 * @param containerList 
 * @param containerListSize 
 * @param numClusters 
 * @return int** 
 */
int** kmeans(container* containerList, int containerListSize, int numClusters) {
    double latMin,
           latMax,
           lonMin,
           lonMax,
           centroidMoveMax = 10;
    findMaxMin(containerList, containerListSize, &latMin, &latMax, &lonMin, &lonMax);
    centroid *centroidList = initCentroidRandom(numClusters, latMin, latMax, lonMin, lonMax); // function doesn't exist
    for (int clusterIndex = 0; clusterIndex < numClusters; clusterIndex++) {
        printf("centroid %i, %i, %lf, %lf \n",clusterIndex,clusterIndex,centroidList[clusterIndex].lat, centroidList[clusterIndex].lon);
    }
    centroidElementInit(centroidList,numClusters);
    assignNodesToCentroids(centroidList,numClusters,containerList,containerListSize);
    for (int i = 0; i < 10; i++) {
        for (int clusterIndex = 0; clusterIndex < numClusters; clusterIndex++) {
            printf("centroid %i, %i, %lf, %lf \n",clusterIndex,clusterIndex,centroidList[clusterIndex].lat, centroidList[clusterIndex].lon);
        }
        centroidElementInit(centroidList,numClusters);
        moveCentroids(centroidList, numClusters, containerList, containerListSize);
        assignNodesToCentroids(centroidList,numClusters,containerList,containerListSize);
    }
    
    /* while (centroidMoveMax > MAX_DELTA) {
        centroidMoveMax = moveCentroids(centroidList, clusterAmount, containerList, containerListSize);
    } */
    free(centroidList);
    int **array = 0;
    return array;
}

/**
 * @brief Find boundries for centroids, based upon highest and smallest values on lat and lon
 * 
 * @param containerList 
 * @param numContainers 
 * @param latMin 
 * @param latMax 
 * @param lonMin 
 * @param lonMax 
 */
void findMaxMin(container* containerList, int numContainers, double* latMin, double* latMax, double* lonMin, double* lonMax) {
    *latMin = containerList[0].lat;
    *latMax = containerList[0].lat;
    *lonMin = containerList[0].lon;
    *lonMax = containerList[0].lon;
    for (int i = 1; i < numContainers; i++) {
        if (containerList[i].lat < *latMin ) {
            *latMin = containerList[i].lat;
        }
        else if (containerList[i].lat > *latMax) {
            *latMax = containerList[i].lat;
        }
        if (containerList[i].lon < *lonMin) {
            *lonMin = containerList[i].lon;
        }
        else if (containerList[i].lon > *lonMax) {
            *lonMax = containerList[i].lon;
        }
    }
}

/**
 * @brief Assign nodes with standard K means distribution where distance is the standard function
 * 
 * @param centroidList 
 * @param numCentroids 
 * @param nodeList 
 * @param numNodes 
 */
void assignNodesToCentroids(centroid* centroidList, int numCentroids, container* nodeList, int numNodes) {
    double distSquared,
           minDistSquared;
    for (int i = 0; i < numNodes; i++){
        minDistSquared = DBL_MAX;
        for (int j = 0; j < numCentroids; j++) {
            // we use the formula for distance between two points, however we keep the distance squared, to avoid doing squaredroot operations
            distSquared = calcDistanceCoord(nodeList[i].lat, centroidList[j].lat, nodeList[i].lon, centroidList[j].lon);
            standardAssignSingleNodeToCentroid(centroidList, j, nodeList, i, distSquared, &minDistSquared);
        }
        centroidList[nodeList[i].group].elements++;
        printf("%i,%i,%lf, %lf\n", nodeList[i].id, nodeList[i].group, nodeList[i].lat, nodeList[i].lon);
    }
}

/**
 * @brief returns the squared distance between two points
 * 
 * @param x1 
 * @param x2 
 * @param y1 
 * @param y2 
 * @return double 
 */
double calcDistanceCoord(double x1, double x2, double y1, double y2) {
    return pow((x1 - x2), 2) + pow((y1 - y2), 2);
}

/**
 * @brief Assigns node in the classic K means way, where the node belongs to the closest centroid
 * 
 * @param centroidList 
 * @param centroidIndex 
 * @param nodeList 
 * @param nodeIndex 
 * @param dist 
 * @param minDist 
 */
void standardAssignSingleNodeToCentroid(centroid* centroidList, int centroidIndex, container* nodeList, int nodeIndex, double dist, double* minDist) {
    if (dist < *minDist) {
        *minDist = dist;
        nodeList[nodeIndex].group = centroidIndex;
    }
}

// Other functions

/**
 * @brief Assigns nodes up until a max of 10
 * 
 * @param centroidList 
 * @param numCentroids 
 * @param nodeList 
 * @param numNodes 
 */
void assignNodesNaiveXmax(centroid* centroidList, int numCentroids, container* nodeList, int numNodes) {
    double minDistanceSquared,
           distanceSquared;
    for (int i = 0; i < numNodes; i++){
        minDistanceSquared = DBL_MAX;
        for (int j = 0; j < numCentroids; j++) {
            // we use the formula for distance between two points, however we keep the distance squared, to avoid doing squaredroot operations
            distanceSquared = calcDistanceCoord(nodeList[i].lat, centroidList[j].lat, nodeList[i].lon, centroidList[j].lon);
            naiveXmaxAssignSingleNodeToCentroid(centroidList, j, nodeList, i, distanceSquared, &minDistanceSquared);
        }
        centroidList[nodeList[i].group].elements++;
        printf("%i,%i,%lf, %lf\n", nodeList[i].id, nodeList[i].group, nodeList[i].lat, nodeList[i].lon);
    }
}

/**
 * @brief Implements a safeguard where there can't be more than 10 nodes assigned per centroid
 * 
 * @param centroidList 
 * @param centroidIndex 
 * @param nodeList 
 * @param nodeIndex 
 * @param dist 
 * @param minDist 
 */
void naiveXmaxAssignSingleNodeToCentroid(centroid* centroidList, int centroidIndex, container* nodeList, int nodeIndex, double dist, double* minDist) {
    if (dist < *minDist && centroidList[centroidIndex].elements < 10) {
        *minDist = dist;
        nodeList[nodeIndex].group = centroidIndex;
    }
}

/**
 * @brief Haha segmentation fault goes brrrrr, require srand call earlier
 * 
 * @param numCentroids 
 * @param latMin 
 * @param latMax 
 * @param lonMin 
 * @param lonMax 
 * @return centroid* 
 */
centroid *initCentroidRandom(int numCentroids, double latMin, double latMax, double lonMin, double lonMax) {
    centroid *centroidList = (centroid*)malloc(sizeof(centroid) * numCentroids);
    if (centroidList == NULL) {
        printf("Cannot allocate memory. Bye\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < numCentroids; i++) {
        centroid val;
        val.lat = ((((double)(rand() % 101)) / 100) * (latMax-latMin))+latMin;
        val.lon = ((((double)(rand() % 101)) / 100) * (lonMax-lonMin)) + lonMin;
        centroidList[i] = val;
       // printf("Created centroid on: %lf, %lf\n", val.lat, val.lon);
    }
    return centroidList;
}
