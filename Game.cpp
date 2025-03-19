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

GameState gameState = MAIN_MENU;
int selectLevel = 0;

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
	SDL_RenderCopy(renderer, homeBackground, nullptr, nullptr);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect titleGame = { SCREEN_WIDTH / 2 - 150, 100, 300, 50 };
	SDL_RenderFillRect(renderer, &titleGame);

	SDL_Rect playButton = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 10, 100, 40 };
	if (selectLevel == 0) {
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	}
	else {
		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	}
	SDL_RenderFillRect(renderer, &playButton);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &playButton);
	SDL_Rect exitButton = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 70, 100, 40 };
	if (selectLevel == 1) {
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	}
	else {
		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	}
	SDL_RenderFillRect(renderer, &exitButton);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &exitButton);


}

void LevelMenu() {
	SDL_RenderCopy(renderer, levelMenuBackground, nullptr, nullptr);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect titleGame = { SCREEN_WIDTH / 2 - 100, 50, 200, 50 };
	SDL_RenderFillRect(renderer, &titleGame);

	SDL_Rect backButton = { 10, 10, 100, 40 };
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderFillRect(renderer, &backButton);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &backButton);

}

void saveLevels() {

}

void openLevels() {

}

void problemGame() {

} 

void faceGame() {
	if(gameState == MAIN_MENU) {
		SDL_RenderCopy(renderer, homeBackground, nullptr, nullptr);
		MainMenu();
	}
	else if (gameState == LEVEL_MENU) {
		SDL_RenderCopy(renderer, levelMenuBackground, nullptr, nullptr);
		LevelMenu();
	}
	else if (gameState == PLAYING) {
		SDL_RenderCopy(renderer, gameBackground, nullptr, nullptr);

		SDL_RenderCopy(renderer, heroCharacter, nullptr, &hero);
		SDL_Rect backButton = { 10, 10, 100, 40 };
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderFillRect(renderer, &backButton);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &backButton);
	}


	SDL_RenderPresent(renderer);
}

void resetGame() {
	Levels(newLevel);
}
