#ifndef MAP_H
#define MAP_H

/* Loads the game map from a specified file and initializes the dimensions. */
char** loadMap(const char* filename, int* rows, int* cols);

/* Converts integer map codes to their corresponding character symbols.*/
char convertIntToChar(int num);

#endif
