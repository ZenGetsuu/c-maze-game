**README**

### Project Overview
This program is a grid-based game in which the player must navigate a map, avoiding a snake and reaching the treasure to win. The game features limited visibility in "DARK mode," controlled through conditional compilation.

### Features
- **Player Movement**: Controlled via `w`, `a`, `s`, `d` commands.
- **Snake Movement**: The snake chases the player when nearby, otherwise moves randomly.
- **Walls and Boundaries**: The player and snake cannot move into walls (`O`) or the map’s borders (`*`).
- **DARK Mode**: Limits player visibility to a set range, which expands after picking up a lantern.
- **Undo Functionality**: Allows the player to undo moves to revert to previous game states.

### Files
1. **main.c**: Handles main game loop, player input, and terminal mode toggling.
2. **game.c**: Manages game logic, player and snake movement, and game state updates.
3. **map.c** and **map_utils.c**: Load the map from a file, manage map copying and freeing.
4. **undo.c**: Supports the undo functionality by saving and restoring game states.
5. **map.txt**: Stores the map layout for loading in the game.

### Compilation

To compile the game in standard mode:

    make

To compile with DARK mode enabled:

    make DARK=1

### Running the Game

Run the program with the map file as an argument:

    ./treasure map.txt


### Controls
- **`w`**: Move up
- **`a`**: Move left
- **`s`**: Move down
- **`d`**: Move right
- **`u`**: Undo last move
- **`q`**: Quit game

### Known Issues
- The snake’s movement occasionally may not immediately trigger game over when it reaches the player.
- Walls and boundaries have been calibrated based on the map dimensions but may require slight adjustment depending on `map.txt`.

