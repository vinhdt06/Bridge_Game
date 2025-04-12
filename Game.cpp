#include "Game.h"
#include <cmath>
#include <fstream>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* homeBackground = nullptr;
SDL_Texture* levelMenuBackground = nullptr;
SDL_Texture* gameBackground = nullptr;
SDL_Texture* replayButton = nullptr;
SDL_Texture* continueButton = nullptr;
SDL_Texture* exitButton = nullptr;
SDL_Texture* playButton = nullptr;
SDL_Texture* levelButton[20] = { nullptr };
SDL_Texture* lockedLevel = nullptr;
SDL_Texture* popup = nullptr;
SDL_Texture* title = nullptr;
SDL_Texture* idleTextures[5] = { nullptr };
SDL_Texture* walkTextures[6] = { nullptr };
std::vector<Platform> platforms;
SDL_Rect hero = { 100, SCREEN_HEIGHT - 100, 52, 52 };
SDL_Rect stick = { 0, 0, STICK_WIDTH, 0 };
SDL_Texture* soundOnTexture = nullptr;
SDL_Texture* soundOffTexture = nullptr;

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
bool hasPlayedFallSound = false;
bool hasPlayedWinSound = false;
bool hasPlayedHitGroundSound = false;
bool hasPlayedPlaceSound = false;
bool isSoundOn = true;
bool isMainMusicPlaying = false;
bool isGameMusicPlaying = false;
bool isSoundButtonHovered = false;
bool isPlayButtonHovered = false;
bool isExitButtonHovered = false;
bool isBackButtonHovered = false;
bool isReplayButtonHovered = false;
bool isContinueButtonHovered = false;
bool levelButtonHovered[20] = { false };

double stickAngle = 0;
int platformsPassed = 0;
int platformsWin = WIN_PLATFORMS;
int newLevel = 1;
int stickTime = 0;
int stickTimeOk = TIME_STICK;
int heroPos = 0;
int selectLevel = 0;
int scrollSpeed = SCROLL_SPEED;
int background = 1;
int currentFrame = 0;
int backgroundX = 0;
int walkSoundChannel = -1;
int stretchSoundChannel = -1;
int fallSoundChannel = -1;
int shakeDuration = 0;
int shakeMagnitude = 3;
int shakeX = 0;
int shakeY = 0;

Mix_Chunk* fallSound = nullptr;
Mix_Chunk* hitGroundSound = nullptr;
Mix_Chunk* winSound = nullptr;
Mix_Music* mainMusic = nullptr;
Mix_Music* playingMusic = nullptr;
Mix_Chunk* clickSound = nullptr;
Mix_Chunk* walkSound = nullptr;
Mix_Chunk* stretchSound = nullptr;
Mix_Chunk* placeSound = nullptr;

Uint32 lastFrameTime = 0;
GameState gameState = MAIN_MENU;
TTF_Font* font = nullptr;
SDL_Color textColor = { 255, 255, 255, 255 };

void renderText(const char* text, int x, int y, SDL_Color color) {
	if (!font) return;
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_Rect textRect = { x, y, textSurface->w, textSurface->h };
	SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}

void createPlatforms() {
	platforms.clear();
	int randomPlatform = PLATFORM_WIDTH + rand() % 10;
	platforms.push_back({ 50, PLATFORM_POS, randomPlatform, PLATFORM_HEIGHT });
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
	heroAfterWalk = false;
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

	if (title != nullptr) {
		int texWidth, texHeight;
		SDL_QueryTexture(title, nullptr, nullptr, &texWidth, &texHeight);

		float scale = 0.6f;
		int scaledWidth = static_cast<int>(texWidth * scale);
		int scaledHeight = static_cast<int>(texHeight * scale);

		SDL_Rect titleRect = {
			SCREEN_WIDTH / 2 - scaledWidth / 2,
			10,
			scaledWidth,
			scaledHeight
		};
		SDL_RenderCopy(renderer, title, nullptr, &titleRect);
	}

	int playButtonSize = isPlayButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) : BUTTON_SIZE;
	SDL_Rect playButtonRect = {
		SCREEN_WIDTH / 2 - playButtonSize / 2,
		SCREEN_HEIGHT / 2 - playButtonSize / 2 + 50,
		playButtonSize,
		playButtonSize
	};
	if (playButton != nullptr) {
		SDL_RenderCopy(renderer, playButton, nullptr, &playButtonRect);
	}
	else {
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_Rect fallbackRect = { SCREEN_WIDTH / 2 - playButtonSize / 2, SCREEN_HEIGHT / 2 - playButtonSize / 2 + 50, playButtonSize, playButtonSize };
		SDL_RenderFillRect(renderer, &fallbackRect);
	}
}

void LevelMenu() {
	SDL_RenderCopy(renderer, levelMenuBackground, nullptr, nullptr);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect titleGame = { SCREEN_WIDTH / 2 - 150, 50, 300, 60 };
	SDL_RenderFillRect(renderer, &titleGame);

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Select Level", textColor);
	if (textSurface) {
		int textWidth = textSurface->w;
		int textHeight = textSurface->h;
		renderText("Select Level", SCREEN_WIDTH / 2 - textWidth / 2, 50 + (60 - textHeight) / 2, textColor);
		SDL_FreeSurface(textSurface);
	}
	else {
		renderText("Select Level", SCREEN_WIDTH / 2 - 90, 65, textColor);
	}
	const int LEVEL_BUTTON_WIDTH = 100;
	const int LEVEL_BUTTON_HEIGHT = 100;
	const int LEVEL_BUTTON_MARGIN = 20;
	int totalLevelsWidth = 5 * LEVEL_BUTTON_WIDTH + 4 * LEVEL_BUTTON_MARGIN;
	int totalLevelsHeight = 4 * LEVEL_BUTTON_HEIGHT + 3 * LEVEL_BUTTON_MARGIN;
	int startX = (SCREEN_WIDTH - totalLevelsWidth) / 2;
	int startY = (SCREEN_HEIGHT - totalLevelsHeight) / 2 + 50;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 5; j++) {
			int indexLevel = 5 * i + j + 1;
			int levelArrayIndex = indexLevel - 1;
			if (indexLevel > 20) continue;

			int levelButtonSize = levelButtonHovered[levelArrayIndex] ? static_cast<int>(LEVEL_BUTTON_WIDTH * HOVER_SCALE) : LEVEL_BUTTON_HEIGHT;
			SDL_Rect levelRect = {
				startX + j * (LEVEL_BUTTON_WIDTH + LEVEL_BUTTON_MARGIN) + (LEVEL_BUTTON_WIDTH - levelButtonSize) / 2,
				startY + i * (LEVEL_BUTTON_HEIGHT + LEVEL_BUTTON_MARGIN) + (LEVEL_BUTTON_HEIGHT - levelButtonSize) / 2,
				levelButtonSize,
				levelButtonSize
			};

			bool isLocked = (indexLevel > 1 && !completeLevel[indexLevel - 1]);
			if (isLocked && lockedLevel != nullptr) {
				SDL_SetTextureAlphaMod(lockedLevel, 255);
				SDL_RenderCopy(renderer, lockedLevel, nullptr, &levelRect);
			}
			else if (levelButton[levelArrayIndex] != nullptr) {
				SDL_SetTextureAlphaMod(levelButton[levelArrayIndex], completeLevel[indexLevel] ? 255 : 100);

				int texWidth, texHeight;
				SDL_QueryTexture(levelButton[levelArrayIndex], nullptr, nullptr, &texWidth, &texHeight);

				float scaleX = static_cast<float>(levelRect.w) / texWidth;
				float scaleY = static_cast<float>(levelRect.h) / texHeight;
				float scale = std::min(scaleX, scaleY);

				SDL_Rect scaledRect = {
					levelRect.x + (levelRect.w - static_cast<int>(texWidth * scale)) / 2,
					levelRect.y + (levelRect.h - static_cast<int>(texHeight * scale)) / 2,
					static_cast<int>(texWidth * scale),
					static_cast<int>(texHeight * scale)
				};

				SDL_RenderCopy(renderer, levelButton[levelArrayIndex], nullptr, &scaledRect);
			}
			else {
				SDL_SetRenderDrawColor(renderer, completeLevel[indexLevel] ? 0 : 200, completeLevel[indexLevel] ? 255 : 200, 0, 255);
				SDL_RenderFillRect(renderer, &levelRect);
			}
		}
	}

	int backButtonSize = isBackButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) : BUTTON_SIZE;
	SDL_Rect backButtonRect = {
		10 + (BUTTON_SIZE - backButtonSize) / 2,
		10 + (BUTTON_SIZE - backButtonSize) / 2,
		backButtonSize,
		backButtonSize
	};
	if (exitButton != nullptr) {
		SDL_RenderCopy(renderer, exitButton, nullptr, &backButtonRect);
	}
	else {
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_Rect fallbackRect = { 10 + (BUTTON_SIZE - backButtonSize) / 2, 10 + (BUTTON_SIZE - backButtonSize) / 2, backButtonSize, backButtonSize };
		SDL_RenderFillRect(renderer, &fallbackRect);
	}
}

void ifWinGame() {
	SDL_Rect popupRect = { SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 125, 300, 250 };
	if (popup != nullptr) {
		SDL_RenderCopy(renderer, popup, nullptr, &popupRect);
	}
	else {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &popupRect);

		SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
		SDL_Rect brownRect = { SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 125, 300, 50 };
		SDL_RenderFillRect(renderer, &brownRect);
	}

	renderText("You Won", SCREEN_WIDTH / 2 - 62, SCREEN_HEIGHT / 2 - 75, textColor);

	int replayButtonSize = isReplayButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) : BUTTON_SIZE;
	SDL_Rect replayButtonRect = {
		SCREEN_WIDTH / 2 - 75 - replayButtonSize / 2,
		SCREEN_HEIGHT / 2 - replayButtonSize / 2,
		replayButtonSize,
		replayButtonSize
	};
	if (replayButton != nullptr) {
		SDL_RenderCopy(renderer, replayButton, nullptr, &replayButtonRect);
	}
	else {
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_Rect fallbackRect = { SCREEN_WIDTH / 2 - 100 - replayButtonSize / 2, SCREEN_HEIGHT / 2 - 20 - replayButtonSize / 2, replayButtonSize, replayButtonSize };
		SDL_RenderFillRect(renderer, &fallbackRect);
	}

	int continueButtonSize = isContinueButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) : BUTTON_SIZE;
	SDL_Rect continueButtonRect = {
		SCREEN_WIDTH / 2 + 74 - continueButtonSize / 2,
		SCREEN_HEIGHT / 2 - continueButtonSize / 2,
		continueButtonSize,
		continueButtonSize
	};
	if (continueButton != nullptr) {
		SDL_RenderCopy(renderer, continueButton, nullptr, &continueButtonRect);
	}
	else {
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_Rect fallbackRect = { SCREEN_WIDTH / 2 + 100 - continueButtonSize / 2, SCREEN_HEIGHT / 2 - 20 - continueButtonSize / 2, continueButtonSize, continueButtonSize };
		SDL_RenderFillRect(renderer, &fallbackRect);
	}

	int backButtonSize = isBackButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) : BUTTON_SIZE;
	SDL_Rect backButtonRect = {
		SCREEN_WIDTH / 2 - backButtonSize / 2,
		SCREEN_HEIGHT / 2 + 60 - backButtonSize / 2,
		backButtonSize,
		backButtonSize
	};
	if (exitButton != nullptr) {
		SDL_RenderCopy(renderer, exitButton, nullptr, &backButtonRect);
	}
	else {
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		SDL_Rect fallbackRect = { SCREEN_WIDTH / 2 - backButtonSize / 2, SCREEN_HEIGHT / 2 + 60 - backButtonSize / 2, backButtonSize, backButtonSize };
		SDL_RenderFillRect(renderer, &fallbackRect);
	}
}

void renderSoundButton() {
	int soundButtonSize = isSoundButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) : BUTTON_SIZE;
	SDL_Rect soundButtonRect = {
		SCREEN_WIDTH - soundButtonSize - 10, 10, soundButtonSize, soundButtonSize
	};

	if (isSoundOn && soundOnTexture != nullptr) {
		SDL_RenderCopy(renderer, soundOnTexture, nullptr, &soundButtonRect);
	}
	else if (!isSoundOn && soundOffTexture != nullptr) {
		SDL_RenderCopy(renderer, soundOffTexture, nullptr, &soundButtonRect);
	}
	else {
		SDL_SetRenderDrawColor(renderer, isSoundOn ? 0 : 255, isSoundOn ? 255 : 0, 0, 255);
		SDL_RenderFillRect(renderer, &soundButtonRect);
	}
}

void saveLevels() {
	std::ofstream file("level_completion.dat", std::ios::binary);
	if (file.is_open()) {
		file.write(reinterpret_cast<char*>(completeLevel), sizeof(completeLevel));
		file.close();
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
	for (int i = 0; i < (int)platforms.size(); i++) {
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
					if (stick.x + stick.w >= platforms[nextPlatforms].x && stick.x + stick.w <= platforms[nextPlatforms].x + platforms[nextPlatforms].w) {
						heroWalk = true;
						heroAfterWalk = false;
						heroPos = platforms[nextPlatforms].x + (platforms[nextPlatforms].w - hero.w) / 2;
					}
					else {
						heroAfterWalk = true;
						heroWalk = true;
						heroPos = stick.x + stick.w;
					}
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
		for (size_t i = 0; i < platforms.size(); i++) {
			auto& plat = platforms[i];
			if (plat.w > 0 && plat.h > 0 && plat.velocity != 0) {
				plat.x += plat.velocity;
				if (plat.x < plat.firstPos - plat.platformsMove) {
					plat.x = plat.firstPos - plat.platformsMove;
					plat.velocity = -plat.velocity;
				}
				else if (plat.x > plat.firstPos + plat.platformsMove) {
					plat.x = plat.firstPos + plat.platformsMove;
					plat.velocity = -plat.velocity;
				}
				if (!heroWalk && !stickTurn && !stickLength && !heroFall && hero.x + hero.w > plat.x && hero.x < plat.x + plat.w && hero.y + hero.h == plat.y) {
					hero.x += plat.velocity;
				}
			}
		}
		for (auto& plat : platforms) {
			if (plat.platformsDisappear && plat.timeDisappear > 0) {
				if (hero.x + hero.w > plat.x && hero.x < plat.x + plat.w && hero.y + hero.h == plat.y) {
					plat.timeDisappear--;
					if (plat.timeDisappear == 0) {
						plat.w = 0;
						plat.h = 0;
						if (!heroFall) {
							heroFall = true;
							stickLength = false;
							stick.h = 0;
							stickAngle = 0;
						}
					}
				}
			}
		}
		bool flagPlatforms = false;
		for (const auto& plat : platforms) {
			if (hero.x + hero.w > plat.x && hero.x < plat.x + plat.w && hero.y + hero.h == plat.y) {
				flagPlatforms = true;
				break;
			}
		}
		if (!flagPlatforms && !heroWalk && !stickTurn && !stickLength && !heroFall) heroFall = true;
	}
}

void faceGame() {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	if (gameState == MAIN_MENU) {
		MainMenu();
	}
	else if (gameState == LEVEL_MENU) {
		LevelMenu();
	}
	else if (gameState == PLAYING || gameState == WON || gameState == LOST) {
		SDL_Rect bgRect1 = { backgroundX + shakeX, shakeY, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_Rect bgRect2 = { backgroundX + SCREEN_WIDTH + shakeX, shakeY, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_RenderCopy(renderer, gameBackground, nullptr, &bgRect1);
		SDL_RenderCopy(renderer, gameBackground, nullptr, &bgRect2);

		if (gameState == PLAYING || gameState == WON) {
			if (stickLength) {
				SDL_SetRenderDrawColor(renderer, 80, 40, 20, 255);
				SDL_Rect growingStick = { stick.x + shakeX, stick.y + shakeY, stick.w, stick.h };
				SDL_RenderFillRect(renderer, &growingStick);
			}
			else if (stickTurn || stickDown) {
				SDL_SetRenderDrawColor(renderer, 80, 40, 20, 255);
				SDL_Rect rotatedStick = { stick.x + shakeX, stick.y + shakeY, stick.w, stick.h };
				SDL_RenderFillRect(renderer, &rotatedStick);
			}

			for (const auto& plat : platforms) {
				if (plat.w > 0 && plat.h > 0) {
					if (gameState == PLAYING && plat.platformsDisappear && plat.timeDisappear <= 70) {
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
					SDL_Rect rect = { plat.x + shakeX, plat.y + shakeY, plat.w, plat.h };
					SDL_RenderFillRect(renderer, &rect);
				}
			}

			SDL_Rect monkeyRect = { hero.x + shakeX, hero.y + shakeY, hero.w, hero.h };
			if (heroWalk) {
				if (walkTextures[currentFrame] != nullptr) {
					SDL_RenderCopy(renderer, walkTextures[currentFrame], nullptr, &monkeyRect);
				}
				else {
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
					SDL_RenderFillRect(renderer, &monkeyRect);
				}
			}
			else {
				if (idleTextures[currentFrame] != nullptr) {
					SDL_RenderCopy(renderer, idleTextures[currentFrame], nullptr, &monkeyRect);
				}
				else {
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
					SDL_RenderFillRect(renderer, &monkeyRect);
				}
			}

			if (gameState == PLAYING) {
				int backButtonSize = isBackButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) : BUTTON_SIZE;
				SDL_Rect backButtonRect = {
					10 + shakeX + (BUTTON_SIZE - backButtonSize) / 2,
					10 + shakeY + (BUTTON_SIZE - backButtonSize) / 2,
					backButtonSize,
					backButtonSize
				};
				if (exitButton != nullptr) {
					SDL_RenderCopy(renderer, exitButton, nullptr, &backButtonRect);
				}
				else {
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
					SDL_Rect fallbackRect = { 10 + shakeX + (BUTTON_SIZE - backButtonSize) / 2, 10 + shakeY + (BUTTON_SIZE - backButtonSize) / 2, backButtonSize, backButtonSize };
					SDL_RenderFillRect(renderer, &fallbackRect);
				}
			}
		}
		if (gameState == WON) {
			ifWinGame();
		}
		if (gameState == LOST) {
			SDL_Rect popupRect = { SCREEN_WIDTH / 2 - 150 + shakeX, SCREEN_HEIGHT / 2 - 125 + shakeY, 300, 250 };

			if (popup != nullptr) {
				SDL_RenderCopy(renderer, popup, nullptr, &popupRect);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderFillRect(renderer, &popupRect);

				SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
				SDL_Rect brownRect = { SCREEN_WIDTH / 2 - 150 + shakeX, SCREEN_HEIGHT / 2 - 125 + shakeY, 300, 50 };
				SDL_RenderFillRect(renderer, &brownRect);
			}
			renderText("You Lose", SCREEN_WIDTH / 2 - 66 + shakeX, SCREEN_HEIGHT / 2 - 76 + shakeY, textColor);

			int replayButtonSize = isReplayButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) : BUTTON_SIZE;
			SDL_Rect replayButtonRect = {
				SCREEN_WIDTH / 2 - 60 - replayButtonSize / 2 + shakeX,
				SCREEN_HEIGHT / 2 + 20 - replayButtonSize / 2 + shakeY,
				replayButtonSize,
				replayButtonSize
			};
			if (replayButton != nullptr) {
				SDL_RenderCopy(renderer, replayButton, nullptr, &replayButtonRect);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
				SDL_Rect fallbackRect = { SCREEN_WIDTH / 2 - 80 + shakeX, SCREEN_HEIGHT / 2 + 20 - replayButtonSize / 2 + shakeY, replayButtonSize, replayButtonSize };
				SDL_RenderFillRect(renderer, &fallbackRect);
			}

			int exitButtonSize = isExitButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) : BUTTON_SIZE;
			SDL_Rect exitButtonRect = {
				SCREEN_WIDTH / 2 + 55 - exitButtonSize / 2 + shakeX,
				SCREEN_HEIGHT / 2 + 22 - exitButtonSize / 2 + shakeY,
				exitButtonSize,
				exitButtonSize
			};
			if (exitButton != nullptr) {
				SDL_RenderCopy(renderer, exitButton, nullptr, &exitButtonRect);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_Rect fallbackRect = { SCREEN_WIDTH / 2 + 80 + shakeX, SCREEN_HEIGHT / 2 + 20 - exitButtonSize / 2 + shakeY, exitButtonSize, exitButtonSize };
				SDL_RenderFillRect(renderer, &fallbackRect);
			}
		}
	}
	renderSoundButton();
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
				case SDLK_UP:
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
			if (event.type == SDL_MOUSEBUTTONDOWN) {
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
					for (int j = 0; j < 5; j++) {
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
