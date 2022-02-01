/**
 * @file Cluster.c
 * @brief Includes the functions for executing a same-size K-means clustering
 * 
 */

#include "Cluster.h"

/**
 * @brief Does kmeans in even clusters
 * 
 * @param containerList 
 * @param numContainers 
 * @param numCluster 
 * @param clusterSize 
 * @return int** 
 */
int** kmeansSameSize(container* containerList, int numContainers, int numCluster, int clusterSize) {
    int globalHasMoved;
    int count = 0;
    centroid* centroidList = kppCentroidInit(numCluster, containerList, numContainers);
    clusterElement* elementList = elementInit(containerList, numContainers);
    
    centroidElementInit(centroidList, numCluster);
    //printCentsToFile(outputFile, centroidList, numCluster);
    memoryCheck(centroidList);
    memoryCheck(elementList);
    sortElementList(elementList, numContainers, centroidList,numCluster);
    assignSortedNodes(elementList, numContainers, centroidList, numCluster, clusterSize);
    //printNodesToFile(outputFile, containerList, numContainers);
    printToFile(centroidList, numCluster, containerList, numContainers);
    do {
        moveCentroids(centroidList, numCluster, containerList, numContainers);
        globalHasMoved = 0;
        for (int i = 0; i < numContainers; i++) {
            elementList[i].deltaDist = calcBestAltDeltaDist(elementList[i], centroidList, numCluster);
        }
        qsort(elementList, numContainers, sizeof(clusterElement), compareClusterElements);
        nodeOptimiseSwitch(centroidList, numCluster, elementList, numContainers, &globalHasMoved, /* DEBUG */containerList);
        //printCentsToFile(outputFile, centroidList, numCluster);
        //printNodesToFile(outputFile, containerList, numContainers);
        count++;
    } while ((globalHasMoved || count < 4) && count < 50);

    printToFile(centroidList, numCluster, containerList, numContainers);

    free(centroidList);
    free(elementList);

    // Create output clusterArray
    int **clusterArray = (int**)malloc(sizeof(int*)*numCluster);
    int *groupElementCountArray = (int*)malloc(sizeof(int)*numCluster);
    memoryCheck(clusterArray);
    memoryCheck(groupElementCountArray);
    for (int i = 0; i < numCluster; i++){
        groupElementCountArray[i] = 0;
        clusterArray[i] = (int*) malloc(sizeof(int)*clusterSize);
    }
    for (int i = 0; i < numContainers; i++) {   
        int group = containerList[i].group;
        int currentlistIndex = groupElementCountArray[group];
        clusterArray[group][currentlistIndex] = containerList[i].id;
        groupElementCountArray[group] += 1;
    }

    free(groupElementCountArray);
    return clusterArray;
}

/**
 * @brief Can insert filepath to be NULL, then it uses default debug file.
 * 
 * @param centroidList 
 * @param numCluster 
 * @param containerList 
 * @param numContainers 
 */
void printToFile (centroid* centroidList, int numCluster, container *containerList, int numContainers) {
    FILE* outputFile = fopen(DEBUG_OUT_FILE, "w");
    fprintf(outputFile,"id,group,lat,lon\n");
    printCentsToFile(outputFile, centroidList, numCluster);
    printNodesToFile(outputFile, containerList, numContainers);
    fclose(outputFile);
}

/**
 * @brief Kmeans++ where centroids one centroid is placed on a random node, and the others are placed furthest away from all previous centroids
 * 
 * @param numCentroids 
 * @param nodeList 
 * @param numNodes 
 * @return centroid* 
 */
centroid* kppCentroidInit(int numCentroids, container* nodeList, int numNodes) {
    int numAssigned = 0,
        chosenNode;

    centroid *centroidList = (centroid*)malloc(sizeof(centroid) * numCentroids);
    memoryCheck(centroidList);
    int* hasBeenChosen = (int*)malloc(sizeof(int)*numNodes);
    memoryCheck(hasBeenChosen);

    // Centroid position assignment
    for (int i = 0; i < numCentroids; i++) {
        if (i == 0) {
            chosenNode = rand() % numNodes;
        }
        else {
            // for each node, find the nearest already initialised center
            chosenNode = findBestNodeToInitCentroid(centroidList, hasBeenChosen, numNodes, nodeList, numAssigned);
        }
        kppCentroidAssign(centroidList, i, nodeList, chosenNode, hasBeenChosen, &numAssigned);
    }
    free(hasBeenChosen);
    return centroidList;
}

/**
 * @brief Find best node for the next initialization of centroid
 * 
 * @param centroidList 
 * @param hasBeenChosen 
 * @param numNodes 
 * @param nodeList 
 * @param numAssigned 
 * @return int 
 */
int findBestNodeToInitCentroid(centroid *centroidList, int* hasBeenChosen, int numNodes, container* nodeList, int numAssigned) {
    double minCentroidDist = DBL_MAX;
    double maxCentroidDist = 0;
    double currentCentroidDist;
    int chosenNode = 0;
    for (int i = 0; i < numNodes; i++) {
        // Calculate distance to centroids and assigns furthest distance unless already chosen
        if (!hasBeenChosen[i]) {
            for (int j = 0; j < numAssigned; j++) {
                currentCentroidDist = calcDistanceCoord(nodeList[i].lat, centroidList[j].lat, nodeList[i].lon, centroidList[j].lon);
                if (currentCentroidDist < minCentroidDist) {
                    minCentroidDist = currentCentroidDist;
                }
            }
            if (minCentroidDist > maxCentroidDist){
                chosenNode = i;
                maxCentroidDist = minCentroidDist;
            }
        }
    }
    return chosenNode;
}

/**
 * @brief Places centroid on node and registers in countList
 * 
 * @param centroidList 
 * @param centroid 
 * @param nodeList 
 * @param node 
 * @param chosenList 
 * @param count 
 */
void kppCentroidAssign(centroid* centroidList, int centroid, container* nodeList, int node, int* chosenList, int* count) {
    chosenList[node] = 1;
    centroidList[centroid].lat = nodeList[node].lat;
    centroidList[centroid].lon = nodeList[node].lon;
    (*count)++;
}

/**
 * @brief Sets all centroid number of elements to 0
 * 
 * @param centroidList 
 * @param numCentroids 
 */
void centroidElementInit(centroid* centroidList, int numCentroids) {
    for (int i = 0; i < numCentroids; i++) {
        centroidList[i].elements = 0;
    }
}

/**
 * @brief Prints centroids to stdout
 * 
 * @param centroidList 
 * @param numCentroids 
 */
void centroidPrint(centroid* centroidList, int numCentroids) {
    for (int i = 0; i < numCentroids; i++) {
        printf("centroid %i,%i,%lf,%lf\n", i, i, centroidList[i].lat, centroidList[i].lon);
    }
}

/**
 * @brief Returns dynamicly allocated list that points to corresponding nodes in nodeList
 * 
 * @param nodeList 
 * @param numNodes 
 * @return clusterElement* 
 */
clusterElement* elementInit(container* nodeList, int numNodes) {
    clusterElement* elementList = (clusterElement*) malloc(sizeof(clusterElement) * numNodes);
    memoryCheck(elementList);
    for (int i = 0; i < numNodes; i++) {
        elementList[i].nodePtr = &(nodeList[i]);
        elementList[i].deltaDist = DBL_MAX;
    }
    return elementList;
}

/**
 * @brief Sorts elementList based upon the deltaDistance between the closest and the furthest centroid
 * 
 * @param elementList 
 * @param numElements 
 * @param centroidList 
 * @param numCentroids 
 */
void sortElementList(clusterElement* elementList, int numElements, centroid* centroidList, int numCentroids) {
    double minDist,
           maxDist,
           currentDist;
    for (int i = 0; i < numElements; i++) {
        minDist = DBL_MAX;
        maxDist = 0;
        for (int j = 0; j < numCentroids; j++) {
            currentDist = calcDistanceCoord(elementList[i].nodePtr->lat, centroidList[j].lat, elementList[i].nodePtr->lon, centroidList[j].lon);
            if (currentDist < minDist) {
                minDist = currentDist;
            }
            if (currentDist > maxDist){
                maxDist = currentDist;
            }
        }
        elementList[i].deltaDist = minDist-maxDist;
    }
    qsort(elementList, numElements, sizeof(clusterElement), compareClusterElements);
}

/**
 * @brief Takes a sorted list of type clusteredElement, and assigns to clusters as long as they arent full
 * 
 * @param elementList 
 * @param numElements 
 * @param centroidList 
 * @param numCentroids 
 * @param clusterSize 
 */
void assignSortedNodes(clusterElement* elementList, int numElements, centroid* centroidList, int numCentroids, int clusterSize) {
    double minDist,
           currentDist;
    container *nodePtr;
    for (int i = 0; i < numElements; i++) {
        nodePtr = elementList[i].nodePtr;
        minDist = DBL_MAX;
        for (int j = 0; j < numCentroids; j++){
            currentDist = calcDistanceCoord(centroidList[j].lat, nodePtr->lat, centroidList[j].lon, nodePtr->lon);
            if (currentDist < minDist && centroidList[j].elements < clusterSize){
                minDist = currentDist;
                nodePtr->group = j;
            }
        }
        centroidList[nodePtr->group].elements++;
    }
}

/**
 * @brief Prints nodes to file
 * 
 * @param printFile 
 * @param containers 
 * @param listSize 
 */
void printNodesToFile(FILE *printFile, container* containers, int listSize){
    for (int i = 0; i < listSize; i++ ){
        int id = containers[i].id;
        fprintf(printFile,"%i,%i,%lf,%lf\n",id,containers[i].group,containers[i].lat,containers[i].lon);
    }
}

/**
 * @brief Prints centroids to file
 * 
 * @param printFile 
 * @param centroids 
 * @param listSize 
 */
void printCentsToFile(FILE *printFile, centroid* centroids, int listSize){
    for (int i = 0; i < listSize; i++ ){
        fprintf(printFile,"centroid :%i,%i,%lf,%lf\n",i,i,centroids[i].lat,centroids[i].lon);
    }
}

/**
 * @brief Moves centroid and returns the max amount the centroids moves.
 * 
 * @param centroidList 
 * @param numCentroids 
 * @param nodeList 
 * @param numNodes 
 */
void moveCentroids(centroid* centroidList, int numCentroids, container* nodeList, int numNodes) {
    for (int i = 0; i < numCentroids; i++) {
        centroidList[i].lat = 0;
        centroidList[i].lon = 0;
    }
    for (int i = 0; i < numNodes; i++) {
        centroidList[nodeList[i].group].lat += nodeList[i].lat;
        centroidList[nodeList[i].group].lon += nodeList[i].lon;
    }
    for (int i = 0; i < numCentroids; i++) {
        if (centroidList[i].elements != 0) {
            centroidList[i].lat = centroidList[i].lat / (double)centroidList[i].elements;
            centroidList[i].lon = centroidList[i].lon / (double)centroidList[i].elements;
        }
    }
}

/**
 * @brief Assigns node with an evenly method
 * 
 * @param centroidList 
 * @param numCentroids 
 * @param elementList 
 * @param numElements 
 * @param globalMoved 
 * @param containerList 
 */
void nodeOptimiseSwitch(centroid* centroidList, int numCentroids, clusterElement* elementList, int numElements, int* globalMoved, container *containerList) {
    clusterDist* distanceList = (clusterDist*) malloc(sizeof(clusterDist) * numCentroids);
    memoryCheck(distanceList);
    List** transferList = (List**) malloc(sizeof(List*)*numCentroids);
    memoryCheck(transferList);
    for (int i = 0; i < numCentroids; i++) {
        transferList[i] = listCreate(0);
    }
    
    for(int i = 0; i < numElements; i++) {
        for(int j = 0; j < numCentroids; j++) {
            distanceList[j].id = j;
            distanceList[j].dist = calcDistBetweenNodeAndCent(elementList[i],centroidList[j]);
        }
        qsort(distanceList, numCentroids, sizeof(clusterDist), compareDistanceElements);
        int hasMoved = 0;
        for (int j = 0; !hasMoved && (j < numCentroids); j++) {
            int centIndex = distanceList[j].id;
            int group = elementList[i].nodePtr->group;
            if (centIndex != group) {
                if (transferList[centIndex]->size > 0) {
                    hasMoved = evalSwapTransferList(transferList[centIndex],elementList, i, centroidList);
                    if(hasMoved == 1){
                        *globalMoved = 1;
                    }
                }
            }
        }
        if (!hasMoved){
            listPush(transferList[elementList[i].nodePtr->group], (void*) &(elementList[i]));
        }
    }
    // frees all transfer lists
    for (int i = 0; i < numCentroids;i++){
        listFree(transferList[i]);
    }
    free(transferList);
    free(distanceList);
}
/**
 * @brief Evaluates if an element can be swapped with any one node in the transferlist
 * 
 * @param transferList 
 * @param elementList 
 * @param elemIndex 
 * @param centroidList 
 * @return 1 if it swapped 0 if it didnt 
 */
int evalSwapTransferList(List* transferList, clusterElement* elementList,int elemIndex,centroid* centroidList){
    int transferListIndex = 0;
    int hasMoved = 0;
    while (transferListIndex < transferList->size && !hasMoved) {
                        clusterElement* transferElement = (clusterElement*)listGet(transferList, transferListIndex);
                        int transferGroup = transferElement->nodePtr->group;
                        int currGroup = elementList[elemIndex].nodePtr->group;
                        double newDist = calcDistBetweenNodeAndCent(elementList[elemIndex], centroidList[transferGroup]) + calcDistBetweenNodeAndCent(*transferElement, centroidList[currGroup]);
                        double oldDist = calcDistBetweenNodeAndCent(elementList[elemIndex], centroidList[currGroup]) + calcDistBetweenNodeAndCent(*transferElement,centroidList[transferGroup]);
                        if (newDist < oldDist) {
                            int oldGroup = transferElement->nodePtr->group;
                            transferElement->nodePtr->group = elementList[elemIndex].nodePtr->group;
                            elementList[elemIndex].nodePtr->group = oldGroup;
                            listRemoveAt(transferList, transferListIndex);
                            hasMoved = 1;
                        }
                        transferListIndex++;
    }
    return hasMoved;
}
/**
 * @brief Calculates delta between current and alternative best distance for an element
 * 
 * @param element 
 * @param centroids 
 * @param numCentroids 
 * @return double 
 */
double calcBestAltDeltaDist(clusterElement element, centroid* centroids, int numCentroids) {
    double altDist,
           bestAltDist = DBL_MAX,
           currentDist;
    currentDist = calcDistanceCoord(element.nodePtr->lat, centroids[element.nodePtr->group].lat, element.nodePtr->lon,centroids[element.nodePtr->group].lon);
    for (int i = 0; i < numCentroids; i++) {
        if (i != element.nodePtr->group){
            altDist = calcDistanceCoord(element.nodePtr->lat, centroids[i].lat, element.nodePtr->lon,centroids[i].lon);
            if (altDist < bestAltDist) {
                bestAltDist = altDist;
            }
        }
    }
    return currentDist-bestAltDist;
}

// Smaller functions

/**
 * @brief checks if allocated memory
 * 
 * @param ptr 
 */
void memoryCheck(void* ptr) {
    if (ptr == NULL) {
        printf("Cannot allocate memory. Bye\n");
        exit(EXIT_FAILURE);
    }
}

double calcDistBetweenNodeAndCent(clusterElement element, centroid centElem){
    return calcDistanceCoord(element.nodePtr->lat, centElem.lat, element.nodePtr->lon, centElem.lon);
}
/**
 * @brief Returns the squared distance between two points
 * 
 * @param x1 
 * @param x2 
 * @param y1 
 * @param y2 
 * @return double 
 */
double calcDistanceCoord(double x1, double x2, double y1, double y2) {
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

/**
 * @brief Compares two cluster elements based on deltaDistance
 * 
 * @param ptr1 
 * @param ptr2 
 * @return int 
 */
int compareClusterElements(const void* ptr1, const void* ptr2) {
    clusterElement* element1 = (clusterElement*)ptr1,
                  * element2 = (clusterElement*)ptr2;
    if (element1->deltaDist == element2->deltaDist) {
        return 0;
    }
    else {
        return (element1->deltaDist > element2->deltaDist) ? (1) : (-1);
    }
    return EXIT_FAILURE;
}

/**
 * @brief Compares two cluster distances to a node
 * 
 * @param ptr1 
 * @param ptr2 
 * @return int 
 */
int compareDistanceElements(const void* ptr1, const void* ptr2) {
    clusterDist *element1 = (clusterDist*)ptr1,
                *element2 = (clusterDist*)ptr2;
    if (element1->dist == element2->dist) {
        return 0;
    }
    else {
        return (element1->dist > element2->dist) ? 1 : -1;
    }
    return EXIT_FAILURE;
}
