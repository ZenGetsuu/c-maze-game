#include "map.h"
#include <stdio.h>
#include <stdlib.h>

/* Converts integer values from the map file to their respective character representations */
char convertIntToChar(int num) {
    switch (num) {
        case 0: return ' ';  /* Empty space */
        case 1: return 'O';  /* Wall */
        case 2: return '@';  /* Lantern */
        case 3: return 'P';  /* Player */
        case 4: return '~';  /* Snake */
        case 5: return '$';  /* Treasure */
        default: return ' '; /* Default to empty space for unrecognized values */
    }
}

/* Loads the game map from a file and adds borders around it */
char** loadMap(const char* filename, int* rows, int* cols) {
    FILE* file;
    char** map;
    int i, j, k, temp;
    int originalRows, originalCols;

    /* Open the map file */
    file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open map file");
        return NULL;
    }

    /* Read the original dimensions of the map */
    if (fscanf(file, "%d %d", &originalRows, &originalCols) != 2) {
        fprintf(stderr, "Error reading map dimensions.\n");
        fclose(file);
        return NULL;
    }

    /* Increase dimensions by 2 to add borders */
    *rows = originalRows + 2;
    *cols = originalCols + 2;

    /* Allocate memory for the map including the borders */
    map = (char**) malloc(*rows * sizeof(char*));
    if (map == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    /* Allocate memory for each row */
    for (i = 0; i < *rows; i++) {
        map[i] = (char*) malloc(*cols * sizeof(char));
        if (map[i] == NULL) {
            perror("Memory allocation failed for map row");
            for (k = 0; k < i; k++) {
                free(map[k]);
            }
            free(map);
            fclose(file);
            return NULL;
        }
    }

    /* Initialize borders with '*' and inner cells with empty spaces */
    for (i = 0; i < *rows; i++) {
        for (j = 0; j < *cols; j++) {
            if (i == 0 || i == *rows - 1 || j == 0 || j == *cols - 1) {
                map[i][j] = '*';  /* Border */
            } else {
                map[i][j] = ' ';  /* Initialize inner cells as empty */
            }
        }
    }

    /* Fill map data from file into the inner cells, avoiding borders */
    for (i = 1; i < *rows - 1; i++) {
        for (j = 1; j < *cols - 1; j++) {
            if (fscanf(file, "%d", &temp) != 1) {
                fprintf(stderr, "Error reading map data.\n");
                for (k = 0; k < *rows; k++) {
                    free(map[k]);
                }
                free(map);
                fclose(file);
                return NULL;
            }
            map[i][j] = convertIntToChar(temp);  /* Convert integer to map character */
        }
    }

    /* Close the file after reading */
    fclose(file);
    return map;
}