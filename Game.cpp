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

bool stickLength = false;
bool stickTurn = false;
bool stickDown = false;
bool heroWalk = false;
bool heroFall = false;
bool heroAfterWalk = false;
bool completeLevel[21] = { false };

double stickAngle = 0;
int platformsPassed = 0;
int platformsWin = WIN_PLATFORMS;
int newLevel = 1;
int stickTime = 0;
int stickTimeOk = TIME_STICK;
int heroPos = 0;
int selectLevel = 0;

GameState gameState = MAIN_MENU;


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
	stickTime = 0;
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
	std::ofstream file("level_completion.dat", std::ios::binary);
	if (file.is_open()) {
		file.write(reinterpret_cast<char*>(completeLevel), sizeof(completeLevel);
		file.close()
	}
}

void openLevels() {
	std::ifstream file("level_complation.dat", std::ios::binary);
	if (file.is_open()) {
		file.read(reinterpret_cast<char*>(completeLevel), sizeof(completeLevel));
		file.close();
	}
}

int indexPlatforms() {
	for(int i = 0; i < (int)platforms.size(); i++) {
		if (hero.x + hero.w > platforms[i].x && hero.x < platforms[i].x + platforms[i].w && hero.y + hero.h == platforms[i].y) return i;
	}
	return -1;
}

void problemGame() {
	if (gameState == PLAYING) {
		int getIndexPlatforms = indexPlatforms();
		if (stickLength && getIndexPlatforms != -1) {
			stick.h += 5;
			stick.y = platforms[getIndexPlatforms].y - stick.h;
			stick.x = platforms[getIndexPlatforms].x + platforms[getIndexPlatforms].w - STICK_WIDTH;
		}
		else if (stickTurn && getIndexPlatforms != -1) {
			stickAngle += 5;
			if (stickAngle >= 90) {
				stickAngle = 90;
				stickTurn = false;
				stickDown = true;
				stick.w = stick.h;
				stick.h = STICK_WIDTH;
				stick.y = platforms[getIndexPlatforms].y;
				stick.x = platforms[getIndexPlatforms].x + platforms[getIndexPlatforms].w - STICK_WIDTH;
				stickTime = stickTimeOk;
				int nextPlatforms = getIndexPlatforms + 1;
				if (nextPlatforms < platforms.size()) {

				}
				else {
					heroAfterWalk = true;
					heroWalk = true;
					heroPos = stick.x + stick.w;
				}
			}
		}
	}
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
		if (stickLength) {
			SDL_SetRenderDrawColor(renderer, 80, 40, 20, 255);
			SDL_Rect stickForm1 = { stick.x, stick.y, stick.w, stick.h };
			SDL_RenderFillRect(renderer, &stickForm1);
		}
		else if (stickTurn || stickDown) {
			SDL_SetRenderDrawColor(renderer, 80, 40, 20, 255);
			SDL_Rect stickForm2 = { stick.x, stick.y, stick.w, stick.h };
			SDL_RenderFillRect(renderer, &stickForm2);
		}

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
