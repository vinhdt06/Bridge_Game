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
const int PLATFORM_POS = 330;
const int PLAT_DIS_MAX = 100;
const int PLAT_DIS_MIN = 200;
const int WIN_PLATFORMS = 4;

enum gameState {
	MAIN_MENU,
	LEVEL_MENU,
	PLAYING,
	EXIT
};

void createPlatforms();
void Levels();
void MainMenu();
void LevelMenu();
void saveLevels();
void openLevels();
void update();
void resetGame();
