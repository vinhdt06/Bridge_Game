#include "Game.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <ctime>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* heroCharacter = nullptr;
SDL_Texture* homeBackground = nullptr;
SDL_Texture* levelMenuBackground = nullptr;
SDL_Texture* gameBackground = nullptr;
std::vector<Platform> platforms;
SDL_Rect hero = { 100, SCREEN_HEIGHT - 100, 35, 35 };
SDL_Rect stick = { 0, 0, STICK_WIDTH, 0 };

double stickAngle = 0;
int platformsPassed = 0;
int platformsWin = WIN_PLATFORMS;
int newLevel = 1;
int stickTime = 0;
int heroPos = 0;

void createPlatforms() {
	platforms.clear();
	int randomPlatform = PLATFORM_WIDTH + rand() % 10;
	platforms.push_back({ 50, PLATFORM_POS, randomplatform, PLATFORM_HEIGHT });
	platforms.back().firstPos = platforms.back().x;
	for (int i = 1; i < 4; i++) {
		randomPlatform = PLATFORM_WIDTH + rand() % 10;
		int newPos = platforms.back().x + platforms.back().w + (rand() % (PLAT_DIS_MAX - PLAT_DIS_MIN) + PLAT_DIS_MIN);
		platforms.push_back({ newPos, PLATFORM_POS, randomPlatform, PLATFORM_HEIGHT });
		platforms.back().firstPos = newPos;
	}

}

void Levels(int level) {
	platformsWin = WIN_PLATFORMS + (level - 1) * 2;
	createPlatforms();
	hero.x = platforms[0].x + (platforms[0].w - hero.w) / 2;
	hero.y = platforms[0].y - hero.h;
	stickAngle = false;
	stickLength = false;
	stickTurn = false;
	heroWalk = false;
	heroFall = false;
	stickDown = false;
	heroPos = hero.x;
	platformsPassed = 0;
}

void MainMenu() {

}

void LevelMenu() {

}

void saveLevels() {

}

void openLevels() {

}

void update() {

} 

void resetGame() {
	Levels(newLevel);
}
