/**
 * @file List.c
 * @brief Linked list functions
 * 
 */

#include "List.h"

static void* listGetNode(const List *list, int index);
static listNode* createNode (void *newValue);
static void freeNode(List *list,listNode *node);

/**
 * @brief Fetches pointer to data in List object
 * 
 * @param list 
 * @param index 
 * @return void* 
 */
void *listGet(const List *list, int index) {
    if (list == NULL || list->tail == NULL || index >= list->size || index < 0 || list->size <= 0) {
        return NULL;
    }
    else {
        listNode *node = listGetNode(list, index);
        return node->data;
    }
}

/**
 * @brief Fetches pointer to Node object in List object
 * 
 * @param list 
 * @param index 
 * @return void* 
 */
static void *listGetNode(const List *list, int index) {
    if (list == NULL || list->tail == NULL || index >= list->size || index < 0 || list->size <= 0) {
        return NULL;
    }
    else {
        listNode *node = list->tail->next;
        for (int i = 0; i < index; i++) {
            node = node->next;
        }
        return node;
    }
}

/**
 * @brief Insert data to Node object in List object, returns boolean by success
 * 
 * @param list 
 * @param index 
 * @param data 
 * @return int 
 */
int listInsert(List *list, int index, void *data) {
    listNode *tempNode;
    if (list == NULL || index > list->size || index < 0 || (tempNode = createNode(data)) == NULL) {
        if (list->freeData == 1) {
            free(data);
        }
        return 0;
    }

    if (list->size == 0) {
        tempNode->prev = tempNode;
        tempNode->next = tempNode;
        list->tail = tempNode;
    }
    else if (list->size == 1) {
        list->tail->next = tempNode;
        list->tail->prev = tempNode;
        tempNode->prev = list->tail;
        tempNode->next = list->tail;
        if (index == 1) {
            list->tail = tempNode;
        }
    } 
    else if (list->size == index) {
        tempNode->prev = list->tail;
        tempNode->next = list->tail->next;
        tempNode->prev->next = tempNode;
        tempNode->next->prev = tempNode;
        list->tail = tempNode;
    }
    else {
        tempNode->prev = (index == 0) ? list->tail : listGetNode(list, index-1);
        tempNode->next = tempNode->prev->next;
        tempNode->prev->next = tempNode;
        tempNode->next->prev = tempNode;
    }
    list->size++;
    return 1;
}

/**
 * @brief Insert data in first Node object in List object
 * 
 * @param list 
 * @param data 
 * @return int 
 */
int listPush(List *list, void *data) {
    return listInsert(list, 0, data);
}

/**
 * @brief Fecthes data from first Node object in List object and removes node from storage but not the data.
 * 
 * @param list 
 * @return void* 
 */
void* listPop(List *list) {
    void *result = listGet(list, 0);
    int tempFreeData = list->freeData;
    list->freeData = 0;
    listRemoveAt(list, 0);
    list->freeData = tempFreeData;
    return result;
}

/**
 * @brief Fetches index of Node object by data in List object
 * 
 * @param list 
 * @param data 
 * @return int 
 */
int listFindByPointer(const List *list, const void *data) {
    if (list == NULL || list->tail == NULL || data == NULL || list->size <= 0) {
        return -1;
    }
    else {
        int index = 0;
        listNode *currentNode = list->tail->next;
        listNode *prevNode = NULL;
        listNode *correctNode = NULL;

        while (prevNode != list->tail && correctNode == NULL) {
            if (currentNode->data == data) {
                correctNode = currentNode;
            } else {
                prevNode = currentNode;
                currentNode = currentNode->next;
                index++;
            }
        }
        if (correctNode == NULL) {
            index = -1;
        }
        return index;
    }
}

int listFindByCompare(const List *list, const void *data, int (*compare)(const void *el1, const void *el2)) {
    if (list == NULL || list->tail == NULL || data == NULL || list->size <= 0) {
        return -1;
    }
    else {
        int index = 0;
        listNode *currentNode = list->tail->next;
        listNode *prevNode = NULL;
        listNode *correctNode = NULL;

        while (prevNode != list->tail && correctNode == NULL) {
            if (compare(data, currentNode->data) == 1) {
                correctNode = currentNode;
            } else {
                prevNode = currentNode;
                currentNode = currentNode->next;
                index++;
            }
        }
        if (correctNode == NULL) {
            index = -1;
        }
        return index;
    }
}

/**
 * @brief Removes Node object from List object, returns boolean by success, data remains in memory
 * 
 * @param list 
 * @param index 
 * @return int 
 */
int listRemoveAt(List *list, int index) {
    if (list == NULL || index >= list->size || index < 0 || list->size == 0 || list->tail == NULL) {
        return 0;
    }
    else if (index == list->size - 1) {
        listNode *toRemove = listGetNode(list, index);
        list->tail = toRemove->prev;    /* 1 */
        
        toRemove->next->prev = toRemove->prev;  /* 2 */
        toRemove->prev->next = toRemove->next;  /* 3 */

        list->size--;
        freeNode(list, toRemove);
        if (list->size == 0) {
            list->tail = NULL;
        }
        return 1;
    }
    else {
        listNode *toRemove = listGetNode(list, index);
        toRemove->next->prev = toRemove->prev;  /* 2 */
        toRemove->prev->next = toRemove->next;  /* 3 */

        list->size--;
        freeNode(list, toRemove);
        if (list->size == 0) {
            list->tail = NULL;
        }
        return 1;
    }
    
}

/**
 * @brief Removes Node object by data from List object, returns boolean by success, data remains in memory 
 * 
 * @param list 
 * @param data 
 */
void listRemoveData(List *list, const void *data) {
    if (list == NULL || list->size == 0 || list->tail == NULL ||data == NULL) {
        return;
    }
    else {
        listRemoveAt(list, listFindByPointer(list, data));
    }
}



/**
 * @brief Clears List object, returns boolean if Node objects were removed
 * 
 * @param list 
 * @return int 
 */
int listRemoveAllNodes(List *list) {
    if (list == NULL) {
        return 0;
    }
    while (list->size > 0) {
        listRemoveAt(list, 0);
    }
    return 1;
}

/**
 * @brief Clears and removes List object, returns boolean if Node objects were removed
 * 
 * @param list 
 * @return int 
 */
int listFree(List *list) {
    if (list == NULL) {
        return 0;
    }
    listRemoveAllNodes(list);
    free(list);
    return 1;
}

/**
 * @brief Create a List object
 * 
 * @return List* 
 */
List* listCreate(int ifFreeData) {
    List *list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->tail = NULL;
    list->size = 0;
    list->freeData = ifFreeData;
    return list;
}

/**
 * @brief Create a Node object
 * 
 * @param newValue 
 * @return listNode* 
 */
static listNode* createNode (void *newValue) {
    listNode *tempNode = (listNode *)malloc(sizeof(listNode));
    if (tempNode == NULL) {
        return NULL;
    }
    tempNode->data = newValue;
    tempNode->next = NULL;
    tempNode->prev = NULL;
    return tempNode;
}

/**
 * @brief Removes Node object
 * 
 * @param node 
 */
static void freeNode(List *list, listNode *node) {
    if (list->freeData != 0) {
        free(node->data);
    }
    free(node);
}
