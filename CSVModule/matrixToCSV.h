#if !defined(MATRIXTOCSVGUARD)
#define MATRIXTOCSVGUARD

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

void outputCSV(int *IDList, int routeNumber, int dataRows);
void make_directory(const char* name);

#endif //MATRIXTOCSVGUARD
