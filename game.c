#include "game.h"
#include "undo.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <termios.h>

/* Moves the player to a new position on the map */
void movePlayer(char** map, int* playerRow, int* playerCol, int newRow, int newCol) {
    map[*playerRow][*playerCol] = ' ';  /* Clear previous player position */
    *playerRow = newRow;
    *playerCol = newCol;
    map[newRow][newCol] = 'P';  /* Place player at new position */
}

/* Moves the snake on the map; prioritizes moving toward the player if one block away */
void moveSnake(char** map, int* snakeRow, int* snakeCol, int rows, int cols, int playerRow, int playerCol) {
    int directions[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    int validMoves[8];
    int numValidMoves = 0;
    int i, choice, newRow, newCol;

    /* If the player is one block away, move directly to the player's position */
    if (abs(*snakeRow - playerRow) + abs(*snakeCol - playerCol) == 1) {
        *snakeRow = playerRow;
        *snakeCol = playerCol;
        return;
    }

    /* Find all valid moves for the snake that avoid walls and borders */
    for (i = 0; i < 8; i++) {
        newRow = *snakeRow + directions[i][0];
        newCol = *snakeCol + directions[i][1];

        /* Check if the position is within bounds and not blocked by walls ('O') or borders ('*') */
        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols &&
            map[newRow][newCol] != 'O' && map[newRow][newCol] != '*' && map[newRow][newCol] != 'P') {
            validMoves[numValidMoves] = i;
            numValidMoves++;
        }
    }

    /* Randomly select a valid move if available */
    if (numValidMoves > 0) {
        choice = validMoves[rand() % numValidMoves];
        newRow = *snakeRow + directions[choice][0];
        newCol = *snakeCol + directions[choice][1];
        map[*snakeRow][*snakeCol] = ' ';  /* Clear previous snake position */
        *snakeRow = newRow;
        *snakeCol = newCol;
        map[newRow][newCol] = '~';  /* Place snake at new position */
    }
}

/* Initializes the game state with player and snake starting positions */
void initializeGameState(char** map, int rows, int cols, int* playerRow, int* playerCol, int* snakeRow, int* snakeCol) {
    srand(time(NULL));  /* Seed random for snake movement */
    *playerRow = 2;
    *playerCol = 3;
    map[*playerRow][*playerCol] = 'P';  /* Place player on map */
    *snakeRow = rows - 3;
    *snakeCol = cols - 2;
    map[*snakeRow][*snakeCol] = '~';  /* Place snake on map */
}

/* Updates game state based on player command, and checks for win/lose conditions */
void updateGameState(char command, char** map, int rows, int cols, int* playerRow, int* playerCol, int* snakeRow, int* snakeCol, GameState** gameState, int* visibility, struct termios* orig_termios) {
    int newRow = *playerRow, newCol = *playerCol;

    /* Determine new player position based on command, restricting movement to avoid walls and borders */
    switch (command) {
        case 'w': /* Move player up */
            if (*playerRow > 0 && map[*playerRow - 1][*playerCol] != 'O' && map[*playerRow - 1][*playerCol] != '*') {
                newRow = *playerRow - 1;
            }
            break;
        case 's': /* Move player down */
            if (*playerRow < rows - 1 && map[*playerRow + 1][*playerCol] != 'O' && map[*playerRow + 1][*playerCol] != '*') {
                newRow = *playerRow + 1;
            }
            break;
        case 'a': /* Move player left */
            if (*playerCol > 0 && map[*playerRow][*playerCol - 1] != 'O' && map[*playerRow][*playerCol - 1] != '*') {
                newCol = *playerCol - 1;
            }
            break;
        case 'd': /* Move player right */
            if (*playerCol < cols - 1 && map[*playerRow][*playerCol + 1] != 'O' && map[*playerRow][*playerCol + 1] != '*') {
                newCol = *playerCol + 1;
            }
            break;
        case 'u': /* Undo the last move */
            if (*gameState) {
                *gameState = popGameState(*gameState);
                restoreGameState(*gameState, map, playerRow, playerCol, snakeRow, snakeCol);
            }
            return;
    }

    /* Check if player moves to a cell with a lantern */
    if (map[newRow][newCol] == '@') {
        *visibility = 6;  /* Increase visibility range upon reaching the lantern */
        map[newRow][newCol] = ' ';  /* Lantern disappears after pickup */
    }

    /* Move player and check for win condition if they reach the treasure ('$') */
    if (map[newRow][newCol] == '$') {
        movePlayer(map, playerRow, playerCol, newRow, newCol);
        disableRawMode(orig_termios);
        printf("You win!\n");
        exit(0);
    }
    movePlayer(map, playerRow, playerCol, newRow, newCol);

    /* Move the snake and check for lose condition if it reaches the player */
    moveSnake(map, snakeRow, snakeCol, rows, cols, *playerRow, *playerCol);
    if (*snakeRow == *playerRow && *snakeCol == *playerCol) {
        disableRawMode(orig_termios);
        printf("You lose!\n");
        exit(0);
    }
}

/* Checks if the game is over based on player's position */
int gameOver(char** map, int rows, int cols, int playerRow, int playerCol) {
    /* Game over if player reaches the treasure ('$') or if the snake reaches the player ('~') */
    if (map[playerRow][playerCol] == '$' || map[playerRow][playerCol] == '~') return 1;
    return 0;
}

/* Prints the game map, applying visibility constraints if in DARK mode */
void printMap(char** map, int rows, int cols, int playerRow, int playerCol, int visibility) {
    int i, j;
    #ifdef DARK_MODE
    int distance;
    #endif

    system("clear"); /* Clear the screen before printing the new map state */

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
                printf("* "); /* Print borders for the map */
            } else {
                #ifdef DARK_MODE
                /* Calculate Manhattan distance to determine if the cell should be visible */
                distance = abs(playerRow - i) + abs(playerCol - j);
                if (distance <= visibility) {
                    if (map[i][j] == ' ') {
                        printf(". "); /* Show boundary of visibility with '.' */
                    } else {
                        printf("%c ", map[i][j]); /* Show visible elements */
                    }
                } else {
                    printf("  "); /* Print empty space for areas out of visibility */
                }
                #else
                printf("%c ", map[i][j]); /* Print the map content normally */
                #endif
            }
        }
        printf("\n");
    }
}
