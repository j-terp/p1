/**
 * @file CSVtoMatrix.c
 * @brief CSV Matrix conversion functions
 * 
 */

#include "CSVtoMatrix.h"


static int countRows(char filePath[]);

/**
 * @brief Create a dist matrix From File object
 * 
 * @param filePath 
 * @param rowTotal 
 * @param columnTotal 
 * @return double** 
 */
double** createMatrixFromFile(char filePath[], int *rowTotal, int *columnTotal) {
    FILE *file_i = fopen(filePath, "r");
    FILE *file_j = fopen(filePath, "r");
    double **array;
    *columnTotal = 2;
    *rowTotal = countRows(filePath);
    //*columnTotal = countColumns(filePath);
    array = allocDoubleMatrix(*rowTotal, *rowTotal);
    // arrayInsertFileData(array, *rowTotal, *columnTotal, filePath);

    //Insert
    for (int i = 0; i < *rowTotal; i++) {
        double lat1 = fetchCellData(file_i);
        double lon1 = fetchCellData(file_i);
        for (int j = 0; j < *rowTotal; j++) {
            double lat2 = fetchCellData(file_j);
            double lon2 = fetchCellData(file_j);
            double dist = distanceInKmBetweenEarthCoordinates(lat1, lon1, lat2, lon2);
            array[i][j] = dist;
        }
        rewind(file_j);
    }
    fclose(file_i);
    fclose(file_j);
    return array;
}
/**
 * @brief Calculates the distance in km between coordinate points
 * 
 * @param lat1 
 * @param lon1 
 * @param lat2 
 * @param lon2 
 * @return double 
 */
double distanceInKmBetweenEarthCoordinates(double lat1, double lon1, double lat2, double lon2) {
    double a = 0.5 - cos((lat2 - lat1) * RADIAN)/2 + cos(lat1 * RADIAN) * cos(lat2 * RADIAN) * (1 - cos((lon2 - lon1) * RADIAN))/2;
    return 2 * EARTH_RADIUS * asin(sqrt(a));
}

/**
 * @brief Counts Rows in File object
 * 
 * @param filePath 
 * @return int 
 */
static int countRows(char filePath[]) {
    FILE *filePtr;
    filePtr = fopen(filePath, "rb");
    int stringLen = 1;
    char letter;

    do {
        letter = fgetc(filePtr);
        stringLen += (letter == '\n') ? 1 : 0;
    } while (letter != EOF);

    fclose(filePtr);
    return stringLen;
}

/**
 * @brief Fetches data from cell in File object, separated by DELIMITER and new lines.
 * 
 * @param filePtr 
 * @return double 
 */
double fetchCellData(FILE *filePtr) {
    double result = 0;
    int stringLen = 10,
        index = 0;

    char letter;
    char *string = (char *)malloc(sizeof(char) * stringLen);

    // Dynamically allocates enough memory for any length input.
    do {
        letter = fgetc(filePtr);
        string[index] = letter;
        index++;
        if (index == stringLen - 1){
            stringLen += 10;
            string = realloc(string, sizeof(char) * stringLen);
        }
    } while (letter != DELIMETER && letter != '\n' && letter != EOF);

    stringLen = index+1;
    string = realloc(string, sizeof(char) * stringLen);
    string[index] = '\0';

    // Converts the string into a double and saves it in result
    int numbersScanned = sscanf(string, "%lf", &result);
    if (numbersScanned == 0) {
        perror("No number scanned does your file have the wrong format?");
    }

    free(string);
    return result;
}
 