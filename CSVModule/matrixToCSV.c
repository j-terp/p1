/**
 * @file matrixToCSV.c
 * @brief CSV from Matrix convertion functions
 * 
 */

#include "matrixToCSV.h"

static void createCSVFileName(char *fileName);
static void createCSV(int *IDList, char *fileName, char *filePath, int dataRows);
static void createCSVPath(char *filePath);

/**
 * @brief Outputs list to File object
 * 
 * @param IDList 
 * @param routeNumber 
 * @param dataRows 
 */
void outputCSV(int *IDList, int routeNumber, int dataRows) {
    char filePath[100] = "./Output/";
    char fileName[100] = "pickupList";
    char number[4] = "-";
    sprintf(number+1, "%d", routeNumber+1);
    number[2] = '-';
    number[3] = '\0';
    strcat(fileName, number);
    createCSVPath(filePath);
    createCSVFileName(fileName);
    createCSV(IDList, fileName, filePath, dataRows);
}

/**
 * @brief Creates Name for File Object
 * 
 * @param fileName 
 */
static void createCSVFileName(char *fileName){
    int stringLen = strlen(fileName) - 1;
    fileName[stringLen] = '\0'; // To remove a new line in the c time function.
    strcat(fileName, ".csv");
}

/**
 * @brief Creates File object
 * 
 * @param IDList 
 * @param fileName 
 * @param filePath 
 * @param dataRows 
 */
static void createCSV(int *IDList, char *fileName, char *filePath, int dataRows){   
    FILE *filePtr;
    printf("%s, is in %s\n", fileName, filePath);
    strcat(filePath, fileName);
    if ((filePtr = fopen(filePath, "w")) == NULL) {
        printf("\nError in creating folder\n");
    }
    else {
        for (int i = 0; i < dataRows - 1; i++ ) {
            fprintf(filePtr, "%d,", IDList[i]);
        }
        fprintf(filePtr, "%d", IDList[dataRows - 1]);
    }
    fclose(filePtr);
}

/**
 * @brief Creates path to File object
 * 
 * @param filePath 
 */
static void createCSVPath(char *filePath){
    time_t now = time(NULL);
    strcat(filePath, ctime(&now)); // to get the current time as a string
    int stringLen = strlen(filePath) - 1;
   // To remove spaces and : in the file name.
    for (int i = 0; i < stringLen; i++){
        if (filePath[i] == ' '){
            filePath[i] = '-';
        }
        else if (filePath[i] == ':'){
            filePath[i] = '.';
        }
    }
    filePath[stringLen] = '\0'; // To remove a new line in the c time function.
    strcat(filePath, "/");
    make_directory(filePath);
}

/**
 * @brief makes directory with correct use of mkdir dependong on OS
 * 
 * @param folderPath
 */
void make_directory(const char* folderPath){
    #ifdef _WIN32 //If windows 32 or 64 bit
        mkdir(folderPath);
    #else //anything else
        mkdir(folderPath, 0000777);
    #endif
}