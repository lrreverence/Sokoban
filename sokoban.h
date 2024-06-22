#ifndef SOKOBAN_GAME_H
#define SOKOBAN_GAME_H

#include "inc/SDL.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "inc/SDL_ttf.h"
#include "inc/SDL_mixer.h"


const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;
const int TILE_SIZE = 57;


enum GameState { MENU, PLAYING, LEVELS, QUIT };


enum TileType { EMPTY, WALL, BOX, PLAYER, STORAGE, BOX_ON_STORAGE, PLAYER_ON_STORAGE };


struct Point {
    int x, y;
};


struct Tile {
    TileType type;
    Point position;
};


class SokobanGame {
public:
    SokobanGame();
    ~SokobanGame();
    bool initialize();
    void run();
    void cleanup();


private:
    void processInput();
    void update();
    void render();
    void renderMenu();
    void renderLevelsMenu();
    void resetGame(int levelIndex);
    bool movePlayer(int dx, int dy);
    void renderText(const std::string& text, int x, int y, SDL_Color color);
    void loadLevels();
    void loadTextures();
    void loadMusic();
    SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer);


    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    std::vector<std::vector<std::vector<Tile>>> levels;
    std::vector<std::vector<Tile>> level;
    Point playerPosition;
    int currentLevelIndex;
    int movesCount = 0;
    GameState currentState;
    Mix_Music* backgroundMusic;
    std::map<TileType, SDL_Texture*> textures;

};

#endif


