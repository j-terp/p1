#if !defined(ListGuard)
#define ListGuard

#include <stdlib.h>
#include "../dataTypes.h"

// Returns a void pointer to the data  
void* listGet(const List *list, int index);

// Inserts data at specific index, returns 1 if success and 0 if failure   
int listInsert(List *list, int index, void *data);
// Inserts data at first index, returns 1 if success and 0 if failure 
int listPush(List *list, void *data);
// Returns the data element at the first index, and removes it  
void* listPop(List *list);
/* Returns index of specified data pointer */
int listFindByPointer(const List *list, const void *data);
/* Returns index of specified data with compare function */
int listFindByCompare(const List *list, const void *data, int (*compare)(const void *el1, const void *el2));
/* Removes an element at specific index, returns 1 if success and 0 if failure  */
int listRemoveAt(List *list, int index);
/* Removes a specific element from the list */
void listRemoveData(List *list, const void *data);
/* Remove all elements */
int listremoveAllNodes(List *list);
/* Free the whole list and all elements */
int listFree(List *list);
/* Returns a pointer to a new List, input 1: free data pointer when freeing node 0: do not. */
List* listCreate(int ifFreeData);

#endif // ListGuard





