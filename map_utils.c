#include "map_utils.h"
#include <stdlib.h>
#include <string.h>  /* For memcpy */

/* Creates a deep copy of a 2D map array */
char** deepCopyMap(char** src, int rows, int cols) {
    int i, j;
    char** copy = (char**)malloc(rows * sizeof(char*));
    if (copy == NULL) return NULL;  /* Memory allocation failed */

    /* Allocate each row and copy data */
    for (i = 0; i < rows; i++) {
        copy[i] = (char*)malloc(cols * sizeof(char));
        if (copy[i] == NULL) {
            /* Free any previously allocated rows if allocation fails */
            for (j = 0; j < i; j++) {
                free(copy[j]);
            }
            free(copy);
            return NULL;
        }
        /* Copy each row from src to the new copy */
        memcpy(copy[i], src[i], cols * sizeof(char));
    }

    return copy;  /* Return the copied 2D map */
}

/* Frees a 2D map array */
void freeMap(char** map, int rows) {
    int i;
    if (map != NULL) {
        for (i = 0; i < rows; i++) {
            free(map[i]);  /* Free each row */
        }
        free(map);  /* Free the map array */
    }
}
