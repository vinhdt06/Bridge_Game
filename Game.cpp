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
bool scrollScreen = false;
bool camePlatforms = false;
bool wonGame = false;
bool showWin = false;

double stickAngle = 0;
int platformsPassed = 0;
int platformsWin = WIN_PLATFORMS;
int newLevel = 1;
int stickTime = 0;
int stickTimeOk = TIME_STICK;
int heroPos = 0;
int selectLevel = 0;
int scrollSpeed = SCROLL_SPEED;

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
	stick = { platforms[0].x + platforms[0].w - STICK_WIDTH, platforms[0].y, STICK_WIDTH, 0 };
	if (platforms.size() > 2) {
		int indexDisappear = rand() % (platforms.size() - 1) + 1;
		platforms[indexDisappear].platformsDisappear = true;
		platforms[indexDisappear].timeDisappear = 120;
		int indexMove;
		do {
			indexMove = rand() % (platforms.size() - 1) + 1;
		} while (indexMove == indexDisappear);
		platforms[indexMove].velocity = (rand() % 5 - 2) * 0.5f;
		platforms[indexMove].platformsMove = rand() % 50 + 20;
		for (size_t i = 1; i < platforms.size(); i++) {
			if (i != indexDisappear && i != indexMove) {
				if (rand() % 100 < 30) {
					platforms[i].platformsDisappear = true;
					platforms[i].timeDisappear = 120;
				}
				if (rand() % 100 < 40) {
					platforms[i].velocity = (rand() % 5 - 2) * 0.5f;
					platforms[i].platformsMove = rand() % 50 + 20;
				}
			}
		}
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
	scrollScreen = false;
	camePlatforms = false;
	wonGame = false;
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

	int levelWidthAll = 5 * LEVEL_WIDTH_BUTTON + 4 * LEVEL_DIS_BUTTON;
	int levelHeightAll = 4 * LEVEL_WIDTH_BUTTON + 3 * LEVEL_DIS_BUTTON;
	int startPosX = (SCREEN_WIDTH - levelWidthAll) / 2;
	int startPosY = (SCREEN_HEIGHT - levelHeightAll) / 2 + 50;

	for (int i = 0; i < 4; i++) {
		for (int i = 0; i < 5; i++) {
			int indexLevel = 5 * i + j + 1;
			SDL_Rect levelSelect = {
				startPosX + j * (LEVEL_WIDTH_BUTTON + LEVEL_DIS_BUTTON),
				startPosY + i * (LEVEL_HEIGHT_BUTTON + LEVEL_DIS_BUTTON),
				LEVEL_WIDTH_BUTTON,
				LEVEL_HEIGHT_BUTTON
			};
			if (completeLevel[indexLevel]) {
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
			}
			SDL_RenderFillRect(renderer, &levelSelect);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(renderer, &levelSelect);
			if (indexLevel == selectLevel) {
				SDL_SetRenderDrawColor(render, 0, 255, 255, 255);
				SDL_RenderDrawRect(renderer, &levelSelect);
			}
		}
	}
	SDL_Rect backButton = { 10, 10, 100, 40 };
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderFillRect(renderer, &backButton);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &backButton);

}

void ifWinGame() {
	SDL_SetRenderDrawColor(render, 0, 0, 0, 150);
	SDL_Rect cover = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_RenderFillRect(renderer, &cover);

	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	SDL_Rect appear = { SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 100, 300, 250 };
	SDL_RenderFillRect(renderer, &appear);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect textWin = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 100, 300, 250 };
	SDL_RenderFillRect(renderer, &textWin);

	SDL_Rect replayButton = { SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2, 100, 40 };
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &replayButton);

	SDL_Rect continueButton = { SCREEN_WIDTH / 2 + 40, SCREEN_HEIGHT / 2, 100, 40 };
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderFillRect(render, &continueButton);

	SDL_Rect backButton = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 60, 100, 40 };
	SDL_SetRenderDrawColor(renderer,255, 255, 0, 255);
	SDL_RenderFillRect(renderer, &backButton);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &replayButton);
	SDL_RenderDrawRect(renderer, &continueButton);
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
		else if (heroWalk) {
			if (hero.x < heroPos) {
				hero.x += HERO_SPEED;
			}
			if (abs(hero.x - heroPos) <= HERO_SPEED) {
				hero.x = heroPos;
				heroWalk = false;
				if (heroAfterWalk) heroFall = true;
				else {
					platformsPassed++;
					camePlatforms = true;
					if (platformsPassed >= platformsWin - 1) {
						wonGame = true;
						gameState = WON;
						completeLevel[newLevel] = true;
						saveLevels();
					}
				}
			}
		}
		else if (camePlatforms) {
			stickDown = false;
			stick.h = 0;
			stick.w = STICK_WIDTH;
			stickAngle = 0;
			getIndexPlatforms = indexPlatforms();
			if (getIndexPlatforms != -1) {
				if (platforms[getIndexPlatforms].velocity != 0) {
					platforms[getIndexPlatforms].velocity = 0;
					hero.x = platforms[getIndexPlatforms].x + (platforms[getIndexPlatforms].w - hero.w) / 2;
				}
				stick.x = platforms[getIndexPlatforms].x + platforms[getIndexPlatforms].w - STICK_WIDTH;
				stick.y = platforms[getIndexPlatforms].y;
			}
			if (platforms.size() < platformsWin) {
				scrollScreen = true;
			}
			camePlatforms = false;
		}
		else if (scrollScreen && platforms.size() < platformsWin) {
			for (auto& plat : platforms) {
				plat.x -= scrollSpeed;
				plat.firstPos -= scrollSpeed;
			}
			hero.x -= scrollSpeed;
			if (platforms[0].x + platforms[0].w < 0) {
				platforms.erase(platforms.begin());
				Platform newPlat;
				newPlat.x = platforms.back().x + platforms.back().w + (rand() % (PLAT_DIS_MAX - PLAT_DIS_MIN) + PLAT_DIS_MIN);
				newPlat.y = PLATFORM_POS;
				newPlat.w = PLATFORM_WIDTH + rand() % 10;
				newPlat.h = PLATFORM_HEIGHT;
				newPlat.firstPos = newPlat.x;
				newPlat.platformsDisappear = false;
				newPlat.timeDisappear = 0;
				newPlat.velocity = 0;
				newPlat.platformsMove = 0;
				bool checkDisappear = false;
				bool checkMove = false;
				for (const auto& plat : platforms) {
					if (plat.platformsDisappear) checkDisappear = true;
					if (plat.velocity != 0) checkMove = true;
				}
				if (!checkDisappear) {
					newPlat.platformsDisappear = true;
					newPlat.timeDisappear = 120;
				}
				else if (!checkMove) {
					newPlat.velocity = (rand() % 5 - 2) * 0.5f;
					newPlat.platformsMove = rand() % 50 + 20;
				}
				else {
					if (rand() % 100 < 30) {
						newPlat.platformsDisappear = true;
						newPlat.timeDisappear = 120;
					}
					if (rand() % 100 < 40) {
						newPlat.velocity = (rand() % 5 - 2) * 0.5f;
						newPlat.platformsMove = rand() % 50 + 20;
					}
				}
				platforms.push_back(newPlat);
				scrollScreen = false;
				if (platforms.size() >= 2) {
					stick = { platforms[0].x + platforms[0].w - STICK_WIDTH, platforms[0].y, STICK_WIDTH, 0 };
					stickAngle = 0;
				}
			}
		}
		else if (heroFall) {
			hero.y += 5;
			if (hero.y > SCREEN_HEIGHT) {
				gameState = LOST;
			}
		}
		if (stickDown && stickTime > 0) {
			stickTime--;
			if (stickTime == 0) stickDown = false;
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
		for (const auto plat : platforms) {
			if (plat.w > 0 && plat.h > 0) {
				if (plat.platformsDisappear && plat.timeDisappear <= 70) {
					if (rand() % 2 == 0) {
						SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
					}
					else {
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					}
				}
				else {
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				}
				SDL_Rect rect = { plat.x, plat.y, plat.w, plat.h };
				SDL_RenderFillRect(renderer, &rect);
			}
		}

		SDL_RenderCopy(renderer, heroCharacter, nullptr, &hero);
		SDL_Rect backButton = { 10, 10, 100, 40 };
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderFillRect(renderer, &backButton);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &backButton);
	}
	else if (gameState == WON) {
		ifWinGame();
	}
	else if (gameState == LOST) {
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_Rect lose = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 200, 100 };
		SDL_RenderFillRect(renderer, &lose);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &lose);

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_Rect rePlayButton = { SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 20, 100, 40 };
		SDL_RenderFillRect(renderer, &rePlayButton);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &rePlayButton);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_Rect homeButton = { SCREEN_WIDTH / 2 + 20, SCREEN_HEIGHT / 2 + 20, 100, 40 };
		SDL_RenderFillRect(renderer, &homeButton);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &homeButton);
	}

	SDL_RenderPresent(renderer);
}

void setEvent(SDL_Event& event, bool& running) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = false;
		}
		if (gameState == MAIN_MENU) {
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				int mouseX = event.button.x;
				int mouseY = event.button.y;
				SDL_Rect playButton = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 10, 100, 40 };
				if (mouseX >= playButton.x && mouseX <= playButton.x + playButton.w && mouseY >= playButton.y && mouseY <= playButton.y + playButton.h) {
					gameState = LEVEL_MENU;
				}
				SDL_Rect exitButton = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 70, 100, 40 };
				if (mouseX >= exitButton.x && mouseX <= exitButton.x + exitButton.w && mouseY >= exitButton.y && mouseY <= exitButton.y + exitButton.h) {
					gameState = EXIT;
				}
			}
			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_UP;
						selectLevel = (selectLevel > 0) ? selectLevel - 1 : 1;
						break;
					case SDLK_DOWN:
						selectLevel = (selectLevel < 1) ? selectLevel + 1 : 0;
						break;
					case SDLK_RETURN:
						if (selectLevel == 0) gameState = LEVEL_MENU;
						else if (selectLevel == 1) gameState = EXIT;
						break;
				}
			}
		}
		else if (gameState == LEVEL_MENU) {
			if (event.type == MOUSEBUTTONDOWN) {
				int mouseX = event.button.x;
				int mouseY = event.button.y;
				const int LEVEL_WIDTH_BUT = 80;
				const int LEVEL_HEIGHT_BUT = 80;
				const int LEVEL_DIS_BUT = 20;
				int levelWidth = 5 * LEVEL_WIDTH_BUT + 4 * LEVEL_DIS_BUT;
				int levelHeight = 4 * LEVEL_WIDTH_BUT + 3 * LEVEL_DIS_BUT;
				int startPosX = (SCREEN_WIDTH - levelWidth) / 2;
				int startPosY = (SCREEN_HEIGHT - levelHeight) / 2 + 50;

				for (int i = 0; i < 4; i++) {
					for (int i = 0; i < 5; i++) {
						int indexLevel = 5 * i + j + 1;
						SDL_Rect levelBut = {
							startPosX + j * (LEVEL_WIDTH_BUT + LEVEL_DIS_BUT),
							startPosY + i * (LEVEL_HEIGHT_BUT + LEVEL_DIS_BUT),
							LEVEL_WIDTH_BUT,
							LEVEL_HEIGHT_BUT
						};
						if (mouseX >= levelBut.x && mouseX <= levelBut.x + levelBut.w && mouseY >= levelBut.y && mouseY <= levelBut.y + levelBut.h) {
							selectLevel = indexLevel;
							newLevel = selectLevel;
							resetGame();
							gameState == PLAYING;
							break;
						}
					}
				}
				SDL_Rect backButton = { 10, 10, 100, 40 };
				if (mouseX >= backButton.x && mouseX <= backButton.x + backButton.w && mouseY >= backButton.y && mouseY <= backButton.y + backButton.h) {
					gameState == MAIN_MENU;
				}
			}
			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					selectLevel = (selectLevel > 4) ? selectLevel - 4 : selectLevel;
					break;
				case SDLK_DOWN:
					selectLevel = (selectLevel <= 20 - 4) ? selectLevel + 4 : selectLevel;
					break;
				case SDLK_LEFT:
					selectLevel = (selectLevel > 1) ? selectLevel - 1 : selectLevel;
					break;
				case SDLK_RIGHT:
					selectLevel = (selectLevel < 20) ? selectLevel + 1 : selectLevel;
					break;
				case SDLK_RETURN:
					newLevel = selectLevel;
					resetGame();
					gameState = PLAYING;
					break;
				case SDLK_ESCAPE:
					gameState = MAIN_MENU;
					break;
				}
			}
		}
		else if (gameState == PLAYING) {
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				int mouseX = event.button.x;
				int mouseY = event.button.y;
				SDL_Rect backButton = { 10, 10, 100, 40 };
				if (mouseX >= backButton.x && mouseX <= backButton.x + backButton.w && mouseY >= backButton.y && mouseY <= backButton.y + backButton.h) {
					gameState = LEVEL_MENU;
				}
				else if (!stickTurn && !heroWalk && !heroFall && !scrollScreen && platforms.size() >= 2) {
					stickLength = true;
				}
			}
			if (event.type == SDL_MOUSEBUTTONUP && stickLength) {
				stickLength = false;
				stickTurn = true;
			}
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
				gameState == LEVEL_MENU;
			}
		}
		else if (gameState = WON) {
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				int mouseX = event.button.x;
				int mouseY = event.button.y;
				SDL_Rect replayButton = { SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2, 100, 40 };
				if (mouseX >= replayButton.x && mouseX <= replayButton.x + replayButton.w && mouseY >= replayButton.y && mouseY <= replayButton.y + replayButton.h) {
					resetGame();
					gameState = PLAYING;
					showWin = false;
				}
				SDL_Rect nextButton = { SCREEN_WIDTH / 2 + 40, SCREEN_HEIGHT / 2, 100, 40 };
				if (mouseX >= nextButton.x && mouseX <= nextButton.x + nextButton.w && mouseY >= nextButton.y && mouseY <= nextButton.y + nextButton.h) {
					newLevel++;
					resetGame();
					gameState = PLAYING;
					showWin = false;
				}
				SDL_Rect backButton = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 60, 100, 40 };
				if (mouseX >= backButton.x && mouseX <= backButton.x + backButton.w && mouseY >= backButton.y && mouseY <= backButton.y + backButton.h) {
					gameState = LEVEL_MENU;
				}
			}
		}
		else if (gameState == LOST) {
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				int mouseX = event.button.x;
				int mouseY = event.button.y;
				SDL_Rect replayButton = { SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 20, 100, 40 };
				if (mouseX >= replayButton.x && mouseX <= replayButton.x + replayButton.w && mouseY >= replayButton.y && mouseY <= replayButton.y + replayButton.h) {
					resetGame();
					gameState = PLAYING;
				}
				SDL_Rect homeButton = { SCREEN_WIDTH / 2 + 20, SCREEN_HEIGHT / 2 + 20, 100, 40 };
				if (mouseX >= homeButton.x && mouseX <= homeButton.x + homeButton.w && mouseY >= homeButton.y && mouseY <= homeButton.y + homeButton.h) {
					gameState = LEVEL_MENU;
				}
			}
		}
		else if (gameState == EXIT) {
			running = false;
		}
	}
}

void resetGame() {
	Levels(newLevel);
}
