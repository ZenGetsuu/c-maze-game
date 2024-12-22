#include "undo.h"
#include "map_utils.h"
#include <stdlib.h>

/* External declarations for map utilities */
extern char** deepCopyMap(char** src, int rows, int cols);
extern void freeMap(char** map, int rows);

/* Pushes the current game state onto the stack */
GameState* pushGameState(GameState* top, char** map, int playerRow, int playerCol, int snakeRow, int snakeCol) {
    GameState* newTop = (GameState*)malloc(sizeof(GameState));
    if (newTop == NULL) return NULL;  /* Memory allocation failed */

    newTop->map = deepCopyMap(map, playerRow, playerCol);  /* Deep copy the map */
    newTop->playerRow = playerRow;
    newTop->playerCol = playerCol;
    newTop->snakeRow = snakeRow;
    newTop->snakeCol = snakeCol;
    newTop->next = top;  /* Set new top of the stack */
    return newTop;
}

/* Pops the top game state from the stack and frees it */
GameState* popGameState(GameState* top) {
    GameState* oldTop;
    if (top == NULL) return NULL;  /* Stack is empty */

    oldTop = top;
    top = top->next;  /* Move top to the next state */
    freeGameState(oldTop);  /* Free the popped state */
    return top;
}

/* Frees memory allocated for a game state */
void freeGameState(GameState* state) {
    if (state) {
        freeMap(state->map, state->playerRow);  /* Free the map */
        free(state);  /* Free the game state struct */
    }
}

/* Restores the game state from the stack */
void restoreGameState(GameState* state, char** map, int* playerRow, int* playerCol, int* snakeRow, int* snakeCol) {
    if (state) {
        freeMap(map, state->playerRow);  /* Free current map */

        map = deepCopyMap(state->map, state->playerRow, state->playerCol);  /* Restore map from saved state */
        *playerRow = state->playerRow;  /* Restore player position */
        *playerCol = state->playerCol;
        *snakeRow = state->snakeRow;  /* Restore snake position */
        *snakeCol = state->snakeCol;
    }
}
