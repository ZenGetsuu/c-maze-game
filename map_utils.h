#ifndef MAP_UTILS_H
#define MAP_UTILS_H

/* Deeply copies a 2D map array */
char** deepCopyMap(char** src, int rows, int cols);

/* Frees a 2D map array */
void freeMap(char** map, int rows);

#endif 
