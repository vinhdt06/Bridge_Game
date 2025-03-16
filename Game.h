#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Platform.h"

const int SCREEN_WIDTH = 850;
const int SCREEN_HEIGHT = 600;
const int PLATFORM_WIDTH = 40;
const int PLATFORM_HEIGHT = 270;
const int STICK_WIDTH = 4;

enum GameState {
    MAIN_MENU,
    LEVEL_MENU,
    PLAYING,
    EXIT
};

void createPlatforms();
void Levels(int level);
void saveLevel();
void openLevel();
void MainMenu();
void LevelMenu();
void WinPopup();
void resetGame();
void update();

#endif