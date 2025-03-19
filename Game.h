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
const int HERO_SPEED = 6;
const int TIME_STICK = 100;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* heroCharacter;
extern SDL_Texture* homeBackground;
extern SDL_Texture* levelMenuBackground;
extern SDL_Texture* gameBackground;
extern std::vector<Platform> platforms;
extern SDL_Rect hero;
extern SDL_Rect stick;

extern bool stickLength;
extern bool stickTurn;
extern bool stickDown;
extern bool heroWalk;
extern bool heroFall;

extern double stickAngle;
extern int platformsPassed;
extern int platformsWin;
extern int newLevel;
extern int stickTime;
extern int heroPos;

enum gameState {
	MAIN_MENU,
	LEVEL_MENU,
	PLAYING,
	EXIT
};

void createPlatforms();
void Levels(int level);
void MainMenu();
void LevelMenu();
void saveLevels();
void openLevels();
void problemGame();
void faceGame();
void resetGame();
