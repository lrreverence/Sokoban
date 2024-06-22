#include "sokoban.h"

#undef main

SokobanGame::SokobanGame()
    : window(nullptr), renderer(nullptr), running(false), currentLevelIndex(0), currentState(MENU) {}

SokobanGame::~SokobanGame() {
    cleanup();
}

bool SokobanGame::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
        return false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("Sokoban Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    loadLevels();
    loadTextures();
    loadMusic();  // Load the background music
    resetGame(currentLevelIndex);
    return true;
}


SDL_Texture* SokobanGame::loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Surface* surface = SDL_LoadBMP(path.c_str());
    if (!surface) {
        std::cerr << "Unable to load image " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
void SokobanGame::loadMusic() {
    backgroundMusic = Mix_LoadMUS("bgmusic.mp3");
    if (!backgroundMusic) {
        std::cerr << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << std::endl;
    } else {
        Mix_PlayMusic(backgroundMusic, -1);  
    }
}

void SokobanGame::loadTextures() {
    textures[EMPTY] = loadTexture("images/empty.bmp", renderer);
    textures[WALL] = loadTexture("images/wall.bmp", renderer);
    textures[BOX] = loadTexture("images/box.bmp", renderer);
    textures[PLAYER] = loadTexture("images/player.bmp", renderer);
    textures[STORAGE] = loadTexture("images/storage.bmp", renderer);
    textures[BOX_ON_STORAGE] = loadTexture("images/box.bmp", renderer);
    textures[PLAYER_ON_STORAGE] = loadTexture("images/playeronstor.bmp", renderer);
}


void SokobanGame::loadLevels() {
    std::vector<std::string> levelStrings[8] = {
     {
    // Level 1
    ".......................",
    ".......................",
    ".......................",
    ".......................",
    "#############..........",
    "#...........#..........",
    "#..$@$$######..........",
    "#...........#..........",
    "#***#.......#..........",
    "#############..........",
    ".......................",
    ".......................",
    ".......................",
    "......................."
},
{
    // Level 2
    "############...........",
    "#**..#.....###.........",
    "#**..#.$..$..#.........",
    "#**..#$####..#.........",
    "#**....@.##..#.........",
    "#**..#.#..$.##.........",
    "######.##$.$.#.........",
    "..#.$..$.$.$.#.........",
    "..#....#.....#.........",
    "..############.........",
    ".......................",
    ".......................",
    ".......................",
    "......................."
},
{
    // Level 3
    "    #####              ",
    "    #   #              ",
    "    #$  #              ",
    "  ###  $###            ",
    "  #  $  $ #            ",
    "### # ### #      ######",
    "#   # ### ########  **#",
    "# $  $              **#",
    "##### #### #@#####  **#",
    "    #      ###   ######",
    "    ########           ",
    ".......................",
    ".......................",
    "......................."
},
{
    // Level 4
    "      ####             ",
    "####### @#             ",
    "#     $  #             ",
    "#   $## $#             ",
    "##$#***# #             ",
    " # $***  #             ",
    " # #* *# ##            ",
    " #   # #$ #            ",
    " #$  $    #            ",
    " #  #######            ",
    " ####                  ",
    ".......................",
    ".......................",
    "......................."
},
{
    // Level 5
    "        ########       ",
    "    #####  #  ####     ",
    "    #   #   $    #     ",
    " #### #$$ ## ##  #     ",
    "##      # #  ## ###    ",
    "#  ### $#$  $  $  #    ",
    "#***    # ##  #   #    ",
    "#***#    @ # ### ##    ",
    "#***#  ###  $  $  #    ",
    "######## ##   #   #    ",
    "          #########    ",
    ".......................",
    ".......................",
    "......................."
},
{
    // Level 6
    "         ####          ",
    " #########  ##         ",
    "##  $      $ #####     ",
    "#   ## ##   ##***#     ",
    "# #$$ $ $$#$##***#     ",
    "# #    @  #   ***#     ",
    "#  $# ###$$   ***#     ",
    "# $  $$  $ ##****#     ",
    "###$       #######     ",
    "  #  #######           ",
    "  ####                 ",
    ".......................",
    ".......................",
    "......................."
},
{
    // Level 7
    "######  ###            ",
    "#**  # ##@##           ",
    "#**  ###   #           ",
    "#**     $$ #           ",
    "#**  # # $ #           ",
    "#**### # $ #           ",
    "#### $ #$  #           ",
    "   #  $# $ #           ",
    "   # $  $  #           ",
    "   #  ##   #           ",
    "   #########           ",
    ".......................",
    ".......................",
    "......................."
},
{
    // Level 8
    "                #####  ",
    "       ###### ###   ####",
    "   #####    ### $ $  $ #",
    "####  ## #$ $    $ #   #",
    "#****   $$ $ $  $   #$##",
    "#** # ## #   ###$## #  #",
    "#****    # ###    #    #",
    "#****    # ##  $  ###$ #",
    "#**######  $  #  #### ##",
    "####    #   ###    @  # ",
    "        ############### ",
    "........................",
    "........................",
    "........................"
}
    };



    for (const auto& levelString : levelStrings) {
        std::vector<std::vector<Tile>> level;
        for (int y = 0; y < 14; y++) {
            std::vector<Tile> row;
            for (int x = 0; x < 24; x++) {
                Tile tile;
                tile.position = { x, y };
                switch (levelString[y][x]) {
                    case '#':
                        tile.type = WALL;
                        break;
                    case '@':
                        tile.type = PLAYER;
                        playerPosition = tile.position;
                        break;
                    case '$':
                        tile.type = BOX;
                        break;
                    case '*':
                        tile.type = STORAGE;
                        break;
                    case '.':
                        tile.type = EMPTY;
                        break;
                    default:
                        tile.type = EMPTY;
                        break;
                }
                row.push_back(tile);
            }
            level.push_back(row);
        }
        levels.push_back(level);
    }
}




void SokobanGame::run() {
    running = true;
    while (running) {
        processInput();
        update();
        render();
        SDL_Delay(16); 
    }
}




void SokobanGame::cleanup() {
    if (backgroundMusic) {
        Mix_FreeMusic(backgroundMusic);
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    for (auto& pair : textures) {
        SDL_DestroyTexture(pair.second);
    }
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}




void SokobanGame::renderMenu() {
 
    SDL_Texture* backgroundTexture = loadTexture("images/bg.bmp", renderer);
    if (!backgroundTexture) {
        std::cerr << "Failed to load background image!" << std::endl;
        return;
    }
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

    SDL_DestroyTexture(backgroundTexture);

}






void SokobanGame::renderLevelsMenu() {
     SDL_Texture* backgroundTexture = loadTexture("images/bglevel.bmp", renderer);
    if (!backgroundTexture) {
        std::cerr << "Failed to load background image!" << std::endl;
        return;
    }
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

    SDL_DestroyTexture(backgroundTexture);
}




void SokobanGame::processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDLK_RETURN:
                    if (currentState == MENU) {
                        currentState = PLAYING;
                        resetGame(currentLevelIndex);
                    }
                    break;
                case SDLK_1: 
                    if (currentState == MENU) {
                        currentState = PLAYING;
                        currentLevelIndex = 0; 
                        resetGame(currentLevelIndex);
                    } else if (currentState == LEVELS) {
                        currentState = PLAYING;
                        currentLevelIndex = 0; 
                        resetGame(currentLevelIndex);
                    }
                    break;
                case SDLK_2:
                    if (currentState == MENU) {
                        currentState = LEVELS;
                    } else if (currentState == LEVELS) {
                        currentState = PLAYING;
                        currentLevelIndex = 1; 
                        resetGame(currentLevelIndex);
                    }
                    break;
                case SDLK_3: 
                    if (currentState == LEVELS) {
                        currentState = PLAYING;
                        currentLevelIndex = 2; 
                        resetGame(currentLevelIndex);
                    } else {
                        running = false;
                    }
                    break;
                     case SDLK_UP:
                    if (currentState == PLAYING) {
                        movePlayer(0, -1);
                        movesCount++;
                    }
                    break;
                case SDLK_DOWN:
                    if (currentState == PLAYING) {
                        movePlayer(0, 1);
                        movesCount++;
                    }
                    break;
                case SDLK_LEFT:
                    if (currentState == PLAYING) {
                        movePlayer(-1, 0);
                        movesCount++;
                    }
                    break;
                case SDLK_RIGHT:
                    if (currentState == PLAYING) {
                        movePlayer(1, 0);
                        movesCount++;
                    }
                    break;
                case SDLK_SPACE:
                    if (currentState == PLAYING) {
                        resetGame(currentLevelIndex);
                        movesCount=0;
                    }
                    break;
                case SDLK_F1:
                    if (currentState == PLAYING) {
                        currentState = MENU;
                        resetGame(currentLevelIndex);
                    }
                    break;
                default:
                    if (currentState == LEVELS) {
                        int firstLevelKey = SDLK_1;
                        int levelSelected = event.key.keysym.sym - firstLevelKey;
                        if (levelSelected >= 0 && levelSelected < levels.size()) {
                            currentState = PLAYING;
                            currentLevelIndex = levelSelected;
                            resetGame(currentLevelIndex);
                        }
                    }
                    break;
            }
        }
    }
}

void SokobanGame::update() {
    bool allBoxesOnStorage = true;
    for (const auto& row : level) {
        for (const auto& tile : row) {
            if (tile.type == BOX) {
                allBoxesOnStorage = false;
                break;
            }
        }
        if (!allBoxesOnStorage) break;
    }

  if (allBoxesOnStorage) {
    movesCount = 0;
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    renderText("Level Complete!", 450, 350, white);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);
    currentLevelIndex++;
    if (currentLevelIndex < levels.size()) {
        resetGame(currentLevelIndex);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        renderText("Congratulations! You completed all levels!", 350, 350, white);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
        running = false;
    }
}

}

void SokobanGame::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    switch (currentState) {
        case MENU:
            renderMenu();
            break;
        case LEVELS:
            renderLevelsMenu();
            break;
        case PLAYING:
            for (const auto& row : level) {
                for (const auto& tile : row) {
                    SDL_Rect destRect = { tile.position.x * TILE_SIZE, tile.position.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                    if (textures[tile.type]) {
                        SDL_RenderCopy(renderer, textures[tile.type], nullptr, &destRect);
                    }
                }
            }
            {
                SDL_Rect playerRect = { playerPosition.x * TILE_SIZE, playerPosition.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                SDL_RenderCopy(renderer, textures[PLAYER], nullptr, &playerRect);

                
                SDL_Color black = { 0, 0, 0, 0 };
                std::string movesText = "Moves: " + std::to_string(movesCount); 
                std::string levelText = "Level: " + std::to_string(currentLevelIndex + 1); 
                renderText(movesText, 750, 650, black);
                renderText(levelText, 200, 650, black);
                renderText("F1 - Main Menu", 60, 700, black);
                renderText("Spacebar - Reset Level", 600, 700, black);
            }

            
            break;
        case QUIT:
            running = false;
            break;
    }

    SDL_RenderPresent(renderer);
}


void SokobanGame::resetGame(int levelIndex) {
    if (levelIndex >= 0 && levelIndex < levels.size()) {
        level = levels[levelIndex];
        for (int y = 0; y < level.size(); ++y) {
            for (int x = 0; x < level[y].size(); ++x) {
                if (level[y][x].type == PLAYER) {
                    playerPosition = { x, y };
                    return;
                }
            }
        }
    }
}

bool SokobanGame::movePlayer(int dx, int dy) {
    int newX = playerPosition.x + dx;
    int newY = playerPosition.y + dy;

    if (newX < 0 || newX >= level[0].size() || newY < 0 || newY >= level.size()) {
        return false;
    }

    Tile& targetTile = level[newY][newX];
    if (targetTile.type == WALL) {
        return false;
    }

    if (targetTile.type == BOX || targetTile.type == BOX_ON_STORAGE) {
        int nextX = newX + dx;
        int nextY = newY + dy;
        if (nextX < 0 || nextX >= level[0].size() || nextY < 0 || nextY >= level.size()) {
            return false;
        }
        Tile& nextTile = level[nextY][nextX];
        if (nextTile.type != EMPTY && nextTile.type != STORAGE) {
            return false;
        }

        if (nextTile.type == STORAGE) {
            nextTile.type = BOX_ON_STORAGE;
        } else {
            nextTile.type = BOX;
        }

        if (targetTile.type == BOX_ON_STORAGE) {
            targetTile.type = STORAGE;
        } else {
            targetTile.type = EMPTY;
        }
    }

    if (targetTile.type == STORAGE) {
        targetTile.type = PLAYER_ON_STORAGE;
    } else if (targetTile.type == EMPTY) {
        targetTile.type = PLAYER;
    }
   
    Tile& playerTile = level[playerPosition.y][playerPosition.x];
    if (playerTile.type == PLAYER_ON_STORAGE) {
        playerTile.type = STORAGE;  
    } else {
        playerTile.type = EMPTY;  
    }

    playerPosition = { newX, newY };
    return true;
}

void SokobanGame::renderText(const std::string& text, int x, int y, SDL_Color color) {
    TTF_Font* font = TTF_OpenFont("Pacifico.ttf", 50);
    if (!font) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
    } else {
        SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };
        SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
        SDL_DestroyTexture(textTexture);
    }

    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
}

int main() {
    SokobanGame game;
    if (!game.initialize()) {
        std::cerr << "Failed to initialize game." << std::endl;
        return 1;
    }
    game.run();
    game.cleanup();
    return 0;
}