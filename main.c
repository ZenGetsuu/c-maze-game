#include <termios.h>  /* For terminal control */
#include <unistd.h>   /* For STDIN_FILENO */
#include <signal.h>   /* For handling signals like Ctrl+C */
#include "game.h"
#include "undo.h"
#include "map_utils.h"  /* For deepCopyMap and freeMap */
#include "map.h"        /* For loadMap */
#include <stdio.h>
#include <stdlib.h>

struct termios orig_termios;

/* Enable raw mode for character-by-character input */
void enableRawMode() {
    struct termios raw;

    /* Get current terminal attributes */
    tcgetattr(STDIN_FILENO, &orig_termios);

    /* Set the terminal to raw mode */
    raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);  /* Disable canonical mode and echo */
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/* Disable raw mode, restoring terminal settings */
void disableRawMode(struct termios* orig) {
    /* Restore the original terminal attributes */
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig);
}

/* Cleanup function to handle exit and signal interruptions */
void cleanup() {
    disableRawMode(&orig_termios);  /* Ensure raw mode is disabled on exit */
}

/* Signal handler for Ctrl+C */
void handleSignal(int signal) {
    cleanup();
    exit(0);
}

int main(int argc, char** argv) {
    int rows, cols;
    int playerRow, playerCol, snakeRow, snakeCol;
    int visibility;
    GameState* gameState = NULL;
    char** gameMap;
    char command;
    int i;

    /* Register cleanup and signal handler */
    atexit(cleanup);
    signal(SIGINT, handleSignal);

    if (argc != 2) {
        printf("Usage: ./treasure <map_file>\n");
        return 1;
    }

    visibility = 3; /* Initial visibility for DARK mode */

    gameMap = loadMap(argv[1], &rows, &cols);
    if (gameMap == NULL) {
        printf("Failed to load the game map.\n");
        return 1;
    }

    initializeGameState(gameMap, rows, cols, &playerRow, &playerCol, &snakeRow, &snakeCol);

    /* Enable raw mode */
    enableRawMode();

    do {
        printMap(gameMap, rows, cols, playerRow, playerCol, visibility);
        printf("Controls:\n");
        printf("Press 'w' to move up\n");
        printf("Press 's' to move down\n");
        printf("Press 'a' to move left\n");
        printf("Press 'd' to move right\n");
        printf("Press 'u' to undo\n");
        printf("Press 'q' to quit\n");
        fflush(stdout); 

        command = getchar();
        updateGameState(command, gameMap, rows, cols, &playerRow, &playerCol, &snakeRow, &snakeCol, &gameState, &visibility, &orig_termios);
    } while (command != 'q' && !gameOver(gameMap, rows, cols, playerRow, playerCol));

    /* Restore terminal settings */
    disableRawMode(&orig_termios);

    /* Free the game resources */
    for (i = 0; i < rows; i++) {
        free(gameMap[i]);
    }
    free(gameMap);
    freeGameState(gameState);

    return 0;
}
