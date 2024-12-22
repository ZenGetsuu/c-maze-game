#ifndef UNDO_H
#define UNDO_H

#include "map_utils.h"  /* If deepCopyMap or freeMap are used in undo.c */

typedef struct GameState {
    char** map;
    int playerRow;
    int playerCol;
    int snakeRow;
    int snakeCol;
    struct GameState* next;
} GameState;

GameState* pushGameState(GameState* top, char** map, int playerRow, int playerCol, int snakeRow, int snakeCol);
GameState* popGameState(GameState* top);
void freeGameState(GameState* state);

void restoreGameState(GameState* state, char** map, int* playerRow, int* playerCol, int* snakeRow, int* snakeCol);

#endif
