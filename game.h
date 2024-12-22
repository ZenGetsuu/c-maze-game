#ifndef GAME_H
#define GAME_H

#include <termios.h> 
#include "undo.h"  

void printMap(char** map, int rows, int cols, int playerRow, int playerCol, int visibility);
void initializeGameState(char** map, int rows, int cols, int* playerRow, int* playerCol, int* snakeRow, int* snakeCol);
void updateGameState(char command, char** map, int rows, int cols, int* playerRow, int* playerCol, int* snakeRow, int* snakeCol, GameState** gameState, int* visibility, struct termios* orig_termios);
void moveSnake(char** map, int* snakeRow, int* snakeCol, int rows, int cols, int playerRow, int playerCol);
void movePlayer(char** map, int* playerRow, int* playerCol, int newRow, int newCol);
int gameOver(char** map, int rows, int cols, int playerRow, int playerCol);

void disableRawMode(struct termios* orig_termios);

#endif
