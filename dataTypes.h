#if !defined(DATATYPEGUARD)
#define DATATYPEGUARD


typedef struct centroid{
    int elements;
    double lat;
    double lon;
    double maxElementDist;
} centroid;

typedef struct container{
    int id,
        group;
    double lat;
    double lon;
    double filledRate;
    int users;
} container;

typedef struct clusterElement{
    container* nodePtr;
    double deltaDist;
} clusterElement;

typedef struct clusterDist{
    int id;
    double dist;
} clusterDist;

typedef struct listNode {
    void *data;
    struct listNode *prev;
    struct listNode *next;
} listNode;

typedef struct List {
    struct listNode *tail;
    int size;
    int freeData;
} List;

typedef struct simulationResult {
    double totalCost;
    double totalGarbageCollected;
    int totalContainerOverflow;
} simulationResult;

#endif // MACRO