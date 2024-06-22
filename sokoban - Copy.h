#ifndef SOKOBAN_GAME_H
#define SOKOBAN_GAME_H

#include "inc/SDL.h"  // Includes the SDL header file
#include <iostream>   // Includes the header for input-output stream
#include <vector>     // Includes the header for the vector container
#include <string>     // Includes the header for string handling
#include <map>        // Includes the header for the map container
#include "inc/SDL_ttf.h"  // Includes the SDL_ttf header file

const int SCREEN_WIDTH = 1366;   // Constant for screen width
const int SCREEN_HEIGHT = 768;    // Constant for screen height
const int TILE_SIZE = 57;         // Constant for tile size

enum GameState { MENU, PLAYING, LEVELS, QUIT };  // Enumeration for different game states

enum TileType { EMPTY, WALL, BOX, PLAYER, STORAGE, BOX_ON_STORAGE, PLAYER_ON_STORAGE };  // Enumeration for different types of tiles

struct Point {   // Struct for a point (x, y) on the screen
    int x, y;    // x and y coordinates
};

struct Tile {    // Struct for a tile in the game
    TileType type;  // Type of the tile
    Point position; // Position of the tile on the screen
};

class SokobanGame {  // Definition of the SokobanGame class
public:
    SokobanGame();   // Constructor
    ~SokobanGame();  // Destructor
    bool initialize();  // Method to initialize the game
    void run();         // Method to run the game loop
    void cleanup();     // Method to clean up resources

private:
    void processInput();     // Method to process user input
    void update();           // Method to update the game state
    void render();           // Method to render the game
    void renderMenu();       // Method to render the menu screen
    void renderLevelsMenu(); // Method to render the levels menu screen
    void resetGame(int levelIndex); // Method to reset the game to a specific level
    bool movePlayer(int dx, int dy); // Method to move the player
    void renderText(const std::string& text, int x, int y, SDL_Color color); // Method to render text on the screen
    void loadLevels();       // Method to load game levels
    void loadTextures();     // Method to load textures
    SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer); // Method to load a texture

    SDL_Window* window;      // Pointer to the SDL window
    SDL_Renderer* renderer;  // Pointer to the SDL renderer
    bool running;            // Flag indicating whether the game is running
    std::vector<std::vector<std::vector<Tile>>> levels;  // Vector to store game levels
    std::vector<std::vector<Tile>> level;               // Vector to store the current level
    Point playerPosition;    // Position of the player
    int currentLevelIndex;   // Index of the current level
    int movesCount = 0;      // Count of player moves
    GameState currentState;  // Current game state
    std::map<TileType, SDL_Texture*> textures;  // Map to store textures

};

#endif
