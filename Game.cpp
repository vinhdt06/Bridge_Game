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
SDL_Texture* SelectLevel = nullptr;
SDL_Texture* platform = nullptr;
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
bool waitingForRestart = false;
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
bool isShaking = false;

double stickAngle = 0;
int platformsPassed = 0;
int platformsWin = WIN_PLATFORMS;
int newLevel = 1;
int stickTime = 0;
int stickTimeOk = TIME_STICK;
int heroPos = 0;
int selectLevel = 1;
int scrollSpeed = SCROLL_SPEED;
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
	stickAngle = 0;
	stickLength = false;
	stickTurn = false;
	heroWalk = false;
	heroFall = false;
	stickDown = false;
	waitingForRestart = false;
	scrollScreen = false;
	heroAfterWalk = false;
	camePlatforms = false;
	wonGame = false;
	heroPos = hero.x;
	stickTime = 0;
	platformsPassed = 0;
	currentFrame = 0;
	lastFrameTime = SDL_GetTicks();
	backgroundX = 0;

	isShaking = false;
	shakeDuration = 0;
	shakeMagnitude = 3;
	shakeX = 0;
	shakeY = 0;

	isPlayButtonHovered = false;
	isExitButtonHovered = false;
	isBackButtonHovered = false;
	isReplayButtonHovered = false;
	isContinueButtonHovered = false;
	isSoundButtonHovered = false;
	for (int i = 0; i < 20; i++) {
		levelButtonHovered[i] = false;
	}
	selectLevel = 1;
	levelButtonHovered[0] = true;

	hasPlayedFallSound = false;
	hasPlayedHitGroundSound = false;
	hasPlayedWinSound = false;
	hasPlayedPlaceSound = false;
	if (walkSoundChannel != -1) {
		Mix_HaltChannel(walkSoundChannel);
		walkSoundChannel = -1;
	}

	if (stretchSoundChannel != -1) {
		Mix_HaltChannel(stretchSoundChannel);
		stretchSoundChannel = -1;
	}
	if (fallSoundChannel != -1) {
		Mix_HaltChannel(fallSoundChannel);
		fallSoundChannel = -1;
	}
	if (isMainMusicPlaying) {
		Mix_HaltMusic();
		isMainMusicPlaying = false;
	}
	if (isSoundOn && playingMusic != nullptr && !isGameMusicPlaying) {
		Mix_PlayMusic(playingMusic, -1);
		isGameMusicPlaying = true;
	}
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
	if (SelectLevel != nullptr) {
		int texWidth, texHeight;
		SDL_QueryTexture(SelectLevel, nullptr, nullptr, &texWidth, &texHeight);
		float scale = 0.4f;
		int scaledWidth = static_cast<int>(texWidth * scale);
		int scaledHeight = static_cast<int>(texHeight * scale);
		SDL_Rect selectLevelRect = {
			SCREEN_WIDTH / 2 - scaledWidth / 2, -11, scaledWidth, scaledHeight
		};
		SDL_RenderCopy(renderer, SelectLevel, nullptr, &selectLevelRect);
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

bool isPointInCircle(int x, int y, int centerX, int centerY, int radius) {
	int dx = x - centerX;
	int dy = y - centerY;
	return (dx * dx + dy * dy) <= (radius * radius);
}

int indexPlatforms() {
	for (int i = 0; i < (int)platforms.size(); i++) {
		if (hero.x + hero.w > platforms[i].x && hero.x < platforms[i].x + platforms[i].w && hero.y + hero.h == platforms[i].y) return i;
	}
	return -1;
}

void problemGame() {
	if (gameState == PLAYING) {
		int currentPlatformIndex = indexPlatforms();
		Uint32 currentTime = SDL_GetTicks();
		if (currentTime - lastFrameTime >= ANIMATION_SPEED) {
			if (heroWalk) {
				currentFrame = (currentFrame + 1) % 6;
			}
			else {
				currentFrame = (currentFrame + 1) % 5;
			}
			lastFrameTime = currentTime;
		}

		if (stickLength && currentPlatformIndex != -1) {
			stick.h += 5;
			stick.y = platforms[currentPlatformIndex].y - stick.h;
			stick.x = platforms[currentPlatformIndex].x + platforms[currentPlatformIndex].w - STICK_WIDTH;

			hasPlayedPlaceSound = false;

			if (isSoundOn && stretchSound != nullptr) {
				if (stretchSoundChannel == -1 || !Mix_Playing(stretchSoundChannel)) {
					stretchSoundChannel = Mix_PlayChannel(-1, stretchSound, -1);
				}
			}
		}

		if (!stickLength && stretchSoundChannel != -1) {
			Mix_HaltChannel(stretchSoundChannel);
			stretchSoundChannel = -1;
		}
		else if (stickTurn && currentPlatformIndex != -1) {
			stickAngle += 5;
			if (stickAngle >= 90) {
				stickAngle = 90;
				stickTurn = false;
				stickDown = true;
				stick.w = stick.h;
				stick.h = STICK_WIDTH;
				stick.y = platforms[currentPlatformIndex].y;
				stick.x = platforms[currentPlatformIndex].x + platforms[currentPlatformIndex].w - STICK_WIDTH;
				stickTime = stickTimeOk;

				if (isSoundOn && !hasPlayedPlaceSound && placeSound != nullptr) {
					Mix_PlayChannel(-1, placeSound, 0);
					hasPlayedPlaceSound = true;
				}

				int nextPlatformIndex = currentPlatformIndex + 1;
				if (nextPlatformIndex < platforms.size()) {
					if (stick.x + stick.w >= platforms[nextPlatformIndex].x && stick.x + stick.w <= platforms[nextPlatformIndex].x + platforms[nextPlatformIndex].w) {
						heroWalk = true;
						heroAfterWalk = false;
						heroPos = platforms[nextPlatformIndex].x + (platforms[nextPlatformIndex].w - hero.w) / 2;
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
			if (isSoundOn && walkSound != nullptr) {
				if (walkSoundChannel == -1 || !Mix_Playing(walkSoundChannel)) {
					walkSoundChannel = Mix_PlayChannel(-1, walkSound, -1);
				}
			}

			if (hero.x < heroPos) {
				hero.x += HERO_SPEED;
			}
			else if (hero.x > heroPos) {
				hero.x -= HERO_SPEED;
			}

			if (abs(hero.x - heroPos) <= HERO_SPEED) {
				hero.x = heroPos;
				heroWalk = false;
				if (heroAfterWalk) {
					heroFall = true;
				}
				else {
					camePlatforms = true;
					platformsPassed++;
					if (platformsPassed >= platformsWin - 1) {
						wonGame = true;
						gameState = WON;
						showWin = true;
						completeLevel[newLevel] = true;
						saveLevels();
						stickLength = false;
						stickTurn = false;
						heroWalk = false;
						heroFall = false;
						stickDown = false;
						scrollScreen = false;
						heroAfterWalk = false;
						camePlatforms = false;
						isShaking = false;
						shakeDuration = 0;
						shakeX = 0;
						shakeY = 0;

						if (isGameMusicPlaying) {
							Mix_HaltMusic();
							isGameMusicPlaying = false;
						}
						if (isSoundOn && !hasPlayedWinSound && winSound != nullptr) {
							Mix_PlayChannel(-1, winSound, 0);
							hasPlayedWinSound = true;
						}
					}
				}
			}
		}
		else if (camePlatforms) {
			stickDown = false;
			stick.h = 0;
			stick.w = STICK_WIDTH;
			stickAngle = 0;
			currentPlatformIndex = indexPlatforms();
			if (currentPlatformIndex != -1) {
				if (platforms[currentPlatformIndex].velocity != 0) {
					platforms[currentPlatformIndex].velocity = 0;
					hero.x = platforms[currentPlatformIndex].x + (platforms[currentPlatformIndex].w - hero.w) / 2;
				}
				stick.x = platforms[currentPlatformIndex].x + platforms[currentPlatformIndex].w - STICK_WIDTH;
				stick.y = platforms[currentPlatformIndex].y;
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
			backgroundX -= static_cast<int>(scrollSpeed * BACKGROUND_SCROLL);

			if (backgroundX <= -SCREEN_WIDTH) {
				backgroundX += SCREEN_WIDTH;
			}

			if (platforms[0].x + platforms[0].w < 0) {
				platforms.erase(platforms.begin());
				Platform newPlatform;
				newPlatform.x = platforms.back().x + platforms.back().w + (rand() % (PLAT_DIS_MAX - PLAT_DIS_MIN) + PLAT_DIS_MIN);
				newPlatform.y = PLATFORM_POS;
				newPlatform.w = PLATFORM_WIDTH + rand() % 10;
				newPlatform.h = PLATFORM_HEIGHT;
				newPlatform.firstPos = newPlatform.x;
				newPlatform.platformsDisappear = false;
				newPlatform.timeDisappear = 0;
				newPlatform.velocity = 0;
				newPlatform.platformsMove = 0;

				bool hasDisappearing = false;
				bool hasMoving = false;
				for (const auto& plat : platforms) {
					if (plat.platformsDisappear) hasDisappearing = true;
					if (plat.velocity != 0) hasMoving = true;
				}

				if (!hasDisappearing) {
					newPlatform.platformsDisappear = true;
					newPlatform.timeDisappear = 120;
				}
				else if (!hasMoving) {
					newPlatform.velocity = (rand() % 5 - 2) * 0.5f;
					newPlatform.platformsMove = rand() % 50 + 20;
				}
				else {
					if (rand() % 100 < 30) {
						newPlatform.platformsDisappear = true;
						newPlatform.timeDisappear = 120;
					}
					if (rand() % 100 < 40) {
						newPlatform.velocity = (rand() % 5 - 2) * 0.5f;
						newPlatform.platformsMove = rand() % 50 + 20;
					}
				}

				platforms.push_back(newPlatform);
				scrollScreen = false;
				if (platforms.size() >= 2) {
					stick = { platforms[0].x + platforms[0].w - STICK_WIDTH, platforms[0].y, STICK_WIDTH, 0 };
					stickAngle = 0;
				}
			}
		}
		else if (heroFall) {
			if (isSoundOn && !hasPlayedFallSound && fallSound != nullptr) {
				fallSoundChannel = Mix_PlayChannel(-1, fallSound, 0);
				hasPlayedFallSound = true;
			}
			hero.y += 9;
			if (hero.y > SCREEN_HEIGHT) {
				if (fallSoundChannel != -1) {
					Mix_HaltChannel(fallSoundChannel);
					fallSoundChannel = -1;
				}
				if (isSoundOn && !hasPlayedHitGroundSound && hitGroundSound != nullptr) {
					Mix_PlayChannel(-1, hitGroundSound, 0);
					hasPlayedHitGroundSound = true;
				}
				gameState = LOST;
				if (!isShaking) {
					isShaking = true;
					shakeDuration = 30;
					shakeMagnitude = 3;
				}
				if (isGameMusicPlaying) {
					Mix_HaltMusic();
					isGameMusicPlaying = false;
				}
			}
		}

		if (!heroWalk && walkSoundChannel != -1) {
			Mix_HaltChannel(walkSoundChannel);
			walkSoundChannel = -1;
		}

		if (stickDown && stickTime > 0) {
			stickTime--;
			if (stickTime == 0) {
				stickDown = false;
			}
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
				if (!heroWalk && !stickTurn && !stickLength && !heroFall &&
					hero.x + hero.w > plat.x && hero.x < plat.x + plat.w &&
					hero.y + hero.h == plat.y) {
					hero.x += plat.velocity;
				}
			}
		}

		for (auto& plat : platforms) {
			if (plat.platformsDisappear && plat.timeDisappear > 0) {
				if (hero.x + hero.w > plat.x && hero.x < plat.x + plat.w &&
					hero.y + hero.h == plat.y) {
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
		bool onPlatform = false;
		for (const auto& plat : platforms) {
			if (hero.x + hero.w > plat.x && hero.x < plat.x + plat.w &&
				hero.y + hero.h == plat.y) {
				onPlatform = true;
				break;
			}
		}
		if (!onPlatform && !heroWalk && !stickTurn && !stickLength && !heroFall) {
			heroFall = true;
		}
	}

	if (isShaking) {
		if (shakeDuration > 0) {
			shakeX = (rand() % (2 * shakeMagnitude + 1)) - shakeMagnitude;
			shakeY = (rand() % (2 * shakeMagnitude + 1)) - shakeMagnitude;
			shakeDuration--;
			shakeMagnitude = (shakeMagnitude * shakeDuration) / 30;
		}
		else {
			isShaking = false;
			shakeX = 0;
			shakeY = 0;
			shakeMagnitude = 3;
		}
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
					SDL_Rect rect = { plat.x + shakeX, plat.y + shakeY, plat.w, plat.h };
					if (gameState == PLAYING && plat.platformsDisappear && plat.timeDisappear <= 70) {
						if (rand() % 2 == 0) {
							SDL_SetTextureAlphaMod(platform, 128);
						}
						else {
							SDL_SetTextureAlphaMod(platform, 255);
						}
						SDL_RenderCopy(renderer, platform, nullptr, &rect);
						SDL_SetTextureAlphaMod(platform, 255);
					}
					else {
						SDL_RenderCopy(renderer, platform, nullptr, &rect);
					}
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
		if (event.type == SDL_MOUSEMOTION) {
			int mouseX = event.motion.x;
			int mouseY = event.motion.y;

			int soundCenterX = SCREEN_WIDTH - BUTTON_SIZE / 2 - 10;
			int soundCenterY = 10 + BUTTON_SIZE / 2;
			int soundRadius = isSoundButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) / 2 : BUTTON_SIZE / 2;
			isSoundButtonHovered = isPointInCircle(mouseX, mouseY, soundCenterX, soundCenterY, soundRadius);

			if (gameState == MAIN_MENU) {
				int playCenterX = SCREEN_WIDTH / 2;
				int playCenterY = SCREEN_HEIGHT / 2 + 50;
				int playRadius = isPlayButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) : BUTTON_SIZE / 2;
				isPlayButtonHovered = isPointInCircle(mouseX, mouseY, playCenterX, playCenterY, playRadius);
			}
			else if (gameState == LEVEL_MENU) {
				int backCenterX = 10 + BUTTON_SIZE / 2;
				int backCenterY = 10 + BUTTON_SIZE / 2;
				int backRadius = isBackButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) : BUTTON_SIZE / 2;
				isBackButtonHovered = isPointInCircle(mouseX, mouseY, backCenterX, backCenterY, backRadius);
				const int LEVEL_BUTTON_WIDTH = 100;
				const int LEVEL_BUTTON_HEIGHT = 100;
				const int LEVEL_BUTTON_MARGIN = 20;
				int totalLevelsWidth = 5 * LEVEL_BUTTON_WIDTH + 4 * LEVEL_BUTTON_MARGIN;
				int totalLevelsHeight = 4 * LEVEL_BUTTON_HEIGHT + 3 * LEVEL_BUTTON_MARGIN;
				int startX = (SCREEN_WIDTH - totalLevelsWidth) / 2;
				int startY = (SCREEN_HEIGHT - totalLevelsHeight) / 2 + 50;

				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 5; j++) {
						int levelIndex = i * 5 + j + 1;
						int levelArrayIndex = levelIndex - 1;
						int levelButtonSize = levelButtonHovered[levelArrayIndex] ? static_cast<int>(LEVEL_BUTTON_WIDTH * HOVER_SCALE) : LEVEL_BUTTON_WIDTH;
						int centerX = startX + j * (LEVEL_BUTTON_WIDTH + LEVEL_BUTTON_MARGIN) + LEVEL_BUTTON_WIDTH / 2;
						int centerY = startY + i * (LEVEL_BUTTON_HEIGHT + LEVEL_BUTTON_MARGIN) + LEVEL_BUTTON_HEIGHT / 2;
						int radius = levelButtonSize / 2;
						bool isLocked = (levelIndex > 1 && !completeLevel[levelIndex - 1]);
						if (!isLocked) {
							levelButtonHovered[levelArrayIndex] = (mouseX >= centerX - radius && mouseX <= centerX + radius &&
								mouseY >= centerY - radius && mouseY <= centerY + radius);
						}
					}
				}
			}
			else if (gameState == PLAYING) {
				int backCenterX = 10 + BUTTON_SIZE / 2 + shakeX;
				int backCenterY = 10 + BUTTON_SIZE / 2 + shakeY;
				int backRadius = isBackButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) : BUTTON_SIZE / 2;
				isBackButtonHovered = isPointInCircle(mouseX, mouseY, backCenterX, backCenterY, backRadius);
			}
			else if (gameState == WON) {
				int replayCenterX = SCREEN_WIDTH / 2 - 75;
				int replayCenterY = SCREEN_HEIGHT / 2;
				int replayRadius = isReplayButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) / 2 : BUTTON_SIZE / 2;
				isReplayButtonHovered = isPointInCircle(mouseX, mouseY, replayCenterX, replayCenterY, replayRadius);

				int continueCenterX = SCREEN_WIDTH / 2 + 74;
				int continueCenterY = SCREEN_HEIGHT / 2;
				int continueRadius = isContinueButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) / 2 : BUTTON_SIZE / 2;
				isContinueButtonHovered = isPointInCircle(mouseX, mouseY, continueCenterX, continueCenterY, continueRadius);

				int backCenterX = SCREEN_WIDTH / 2;
				int backCenterY = SCREEN_HEIGHT / 2 + 60;
				int backRadius = isBackButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) / 2 : BUTTON_SIZE / 2;
				isBackButtonHovered = isPointInCircle(mouseX, mouseY, backCenterX, backCenterY, backRadius);
			}
			else if (gameState == LOST) {
				int replayCenterX = SCREEN_WIDTH / 2 - 60 + shakeX;
				int replayCenterY = SCREEN_HEIGHT / 2 + 20 + shakeY;
				int replayRadius = isReplayButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) / 2 : BUTTON_SIZE / 2;
				isReplayButtonHovered = isPointInCircle(mouseX, mouseY, replayCenterX, replayCenterY, replayRadius);

				int exitCenterX = SCREEN_WIDTH / 2 + 55 + shakeX;
				int exitCenterY = SCREEN_HEIGHT / 2 + 22 + shakeY;
				int exitRadius = isExitButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) / 2 : BUTTON_SIZE / 2;
				isExitButtonHovered = isPointInCircle(mouseX, mouseY, exitCenterX, exitCenterY, exitRadius);
			}
		}

		if (event.type == SDL_MOUSEBUTTONDOWN) {
			int mouseX = event.button.x;
			int mouseY = event.button.y;

			int soundCenterX = SCREEN_WIDTH - BUTTON_SIZE / 2 - 10;
			int soundCenterY = 10 + BUTTON_SIZE / 2;
			int soundRadius = isSoundButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) / 2 : BUTTON_SIZE / 2;
			if (isPointInCircle(mouseX, mouseY, soundCenterX, soundCenterY, soundRadius)) {
				if (isSoundOn && clickSound != nullptr) {
					Mix_PlayChannel(-1, clickSound, 0);
				}
				isSoundOn = !isSoundOn;
				if (!isSoundOn) {
					Mix_Volume(-1, 0);
					Mix_HaltMusic();
					if (walkSoundChannel != -1) {
						Mix_HaltChannel(walkSoundChannel);
						walkSoundChannel = -1;
					}
					if (stretchSoundChannel != -1) {
						Mix_HaltChannel(stretchSoundChannel);
						stretchSoundChannel = -1;
					}
					if (fallSoundChannel != -1) {
						Mix_HaltChannel(fallSoundChannel);
						fallSoundChannel = -1;
					}
					isMainMusicPlaying = false;
					isGameMusicPlaying = false;
				}
				else {
					Mix_Volume(-1, MIX_MAX_VOLUME);
					Mix_VolumeMusic(MIX_MAX_VOLUME / 8);
					if (gameState == MAIN_MENU || gameState == LEVEL_MENU) {
						if (mainMusic != nullptr && !isMainMusicPlaying) {
							Mix_PlayMusic(mainMusic, -1);
							isMainMusicPlaying = true;
						}
					}
					else if (gameState == PLAYING) {
						if (playingMusic != nullptr && !isGameMusicPlaying) {
							Mix_PlayMusic(playingMusic, -1);
							isGameMusicPlaying = true;
						}
					}
				}
			}
		}

		if (gameState == MAIN_MENU) {
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				int mouseX = event.button.x;
				int mouseY = event.button.y;

				int playCenterX = SCREEN_WIDTH / 2;
				int playCenterY = SCREEN_HEIGHT / 2 + 50;
				int playRadius = isPlayButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) : BUTTON_SIZE / 2;
				if (isPointInCircle(mouseX, mouseY, playCenterX, playCenterY, playRadius)) {
					if (isSoundOn && clickSound != nullptr) {
						Mix_PlayChannel(-1, clickSound, 0);
					}
					wonGame = true;
					gameState = LEVEL_MENU;
					if (isSoundOn && mainMusic != nullptr && !isMainMusicPlaying) {
						Mix_PlayMusic(mainMusic, -1);
						isMainMusicPlaying = true;
					}
				}
			}
		}
		else if (gameState == LEVEL_MENU) {
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				int mouseX = event.button.x;
				int mouseY = event.button.y;
				const int LEVEL_BUTTON_WIDTH = 100;
				const int LEVEL_BUTTON_HEIGHT = 100;
				const int LEVEL_BUTTON_MARGIN = 20;
				int totalLevelsWidth = 5 * LEVEL_BUTTON_WIDTH + 4 * LEVEL_BUTTON_MARGIN;
				int totalLevelsHeight = 4 * LEVEL_BUTTON_HEIGHT + 3 * LEVEL_BUTTON_MARGIN;
				int startX = (SCREEN_WIDTH - totalLevelsWidth) / 2;
				int startY = (SCREEN_HEIGHT - totalLevelsHeight) / 2 + 50;

				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 5; j++) {
						int levelIndex = i * 5 + j + 1;
						if (levelIndex > 20) continue;
						SDL_Rect levelRect = {
							startX + j * (LEVEL_BUTTON_WIDTH + LEVEL_BUTTON_MARGIN),
							startY + i * (LEVEL_BUTTON_HEIGHT + LEVEL_BUTTON_MARGIN),
							LEVEL_BUTTON_WIDTH,
							LEVEL_BUTTON_HEIGHT
						};

						bool isLocked = (levelIndex > 1 && !completeLevel[levelIndex - 1]);
						if (!isLocked && mouseX >= levelRect.x && mouseX <= levelRect.x + levelRect.w &&
							mouseY >= levelRect.y && mouseY <= levelRect.y + levelRect.h) {
							if (isSoundOn && clickSound != nullptr) {
								Mix_PlayChannel(-1, clickSound, 0);
							}
							selectLevel = levelIndex;
							newLevel = selectLevel;
							resetGame();
							gameState = PLAYING;
							break;
						}
					}
				}

				int backCenterX = 10 + BUTTON_SIZE / 2;
				int backCenterY = 10 + BUTTON_SIZE / 2;
				int backRadius = isBackButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) : BUTTON_SIZE / 2;
				if (isPointInCircle(mouseX, mouseY, backCenterX, backCenterY, backRadius)) {
					if (isSoundOn && clickSound != nullptr) {
						Mix_PlayChannel(-1, clickSound, 0);
					}
					gameState = MAIN_MENU;
					if (isSoundOn && mainMusic != nullptr && !isMainMusicPlaying) {
						Mix_PlayMusic(mainMusic, -1);
						isMainMusicPlaying = true;
					}
				}
			}
			else if (event.type == SDL_KEYDOWN) {
				int previousSelectedLevel = selectLevel;
				int newSelectedLevel = selectLevel;

				switch (event.key.keysym.sym) {
				case SDLK_UP:
					if (selectLevel > 5) {
						newSelectedLevel = selectLevel - 5;
					}
					break;
				case SDLK_DOWN:
					if (selectLevel <= 15) {
						newSelectedLevel = selectLevel + 5;
					}
					break;
				case SDLK_LEFT:
					if (selectLevel > 1) {
						newSelectedLevel = selectLevel - 1;
					}
					break;
				case SDLK_RIGHT:
					if (selectLevel < 20) {
						newSelectedLevel = selectLevel + 1;
					}
					break;
				case SDLK_RETURN:
					if (selectLevel >= 1 && selectLevel <= 20 && (selectLevel == 1 || completeLevel[selectLevel - 1])) {
						newLevel = selectLevel;
						resetGame();
						gameState = PLAYING;
					}
					break;
				case SDLK_ESCAPE:
					gameState = MAIN_MENU;
					if (isSoundOn && mainMusic != nullptr && !isMainMusicPlaying) {
						Mix_PlayMusic(mainMusic, -1);
						isMainMusicPlaying = true;
					}
					break;
				}

				if (newSelectedLevel >= 1 && newSelectedLevel <= 20 && (newSelectedLevel == 1 || completeLevel[newSelectedLevel - 1])) {
					selectLevel = newSelectedLevel;
				}

				if (previousSelectedLevel != selectLevel) {
					for (int i = 0; i < 20; i++) {
						levelButtonHovered[i] = false;
					}
					if (selectLevel >= 1 && selectLevel <= 20) {
						levelButtonHovered[selectLevel - 1] = true;
					}
				}
			}
		}
		else if (gameState == PLAYING) {
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				int mouseX = event.button.x;
				int mouseY = event.button.y;

				int backCenterX = 10 + BUTTON_SIZE / 2 + shakeX;
				int backCenterY = 10 + BUTTON_SIZE / 2 + shakeY;
				int backRadius = isBackButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) : BUTTON_SIZE / 2;
				if (isPointInCircle(mouseX, mouseY, backCenterX, backCenterY, backRadius)) {
					if (isSoundOn && clickSound != nullptr) {
						Mix_PlayChannel(-1, clickSound, 0);
					}
					gameState = LEVEL_MENU;
					if (walkSoundChannel != -1) {
						Mix_HaltChannel(walkSoundChannel);
						walkSoundChannel = -1;
					}
					if (stretchSoundChannel != -1) {
						Mix_HaltChannel(stretchSoundChannel);
						stretchSoundChannel = -1;
					}
					if (isGameMusicPlaying) {
						Mix_HaltMusic();
						isGameMusicPlaying = false;
					}
					if (isSoundOn && mainMusic != nullptr && !isMainMusicPlaying) {
						Mix_PlayMusic(mainMusic, -1);
						isMainMusicPlaying = true;
					}
				}
				else {
					int soundCenterX = SCREEN_WIDTH - BUTTON_SIZE / 2 - 10;
					int soundCenterY = 10 + BUTTON_SIZE / 2;
					int soundRadius = isSoundButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) / 2 : BUTTON_SIZE / 2;

					if (!isPointInCircle(mouseX, mouseY, soundCenterX, soundCenterY, soundRadius) &&
						!stickTurn && !heroWalk && !heroFall && !scrollScreen && platforms.size() >= 2) {
						stickLength = true;
					}
				}
			}
			if (event.type == SDL_MOUSEBUTTONUP && stickLength) {
				stickLength = false;
				stickTurn = true;
			}
		}
		else if (gameState == WON) {
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				int mouseX = event.button.x;
				int mouseY = event.button.y;

				int replayCenterX = SCREEN_WIDTH / 2 - 75;
				int replayCenterY = SCREEN_HEIGHT / 2;
				int replayRadius = isReplayButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) / 2 : BUTTON_SIZE / 2;
				if (isPointInCircle(mouseX, mouseY, replayCenterX, replayCenterY, replayRadius)) {
					if (isSoundOn && clickSound != nullptr) {
						Mix_PlayChannel(-1, clickSound, 0);
					}
					resetGame();
					gameState = PLAYING;
					showWin = false;
				}

				int continueCenterX = SCREEN_WIDTH / 2 + 74;
				int continueCenterY = SCREEN_HEIGHT / 2;
				int continueRadius = isContinueButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) / 2 : BUTTON_SIZE / 2;
				if (isPointInCircle(mouseX, mouseY, continueCenterX, continueCenterY, continueRadius)) {
					if (isSoundOn && clickSound != nullptr) {
						Mix_PlayChannel(-1, clickSound, 0);
					}
					newLevel++;
					resetGame();
					gameState = PLAYING;
					showWin = false;
				}

				int backCenterX = SCREEN_WIDTH / 2;
				int backCenterY = SCREEN_HEIGHT / 2 + 60;
				int backRadius = isBackButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) / 2 : BUTTON_SIZE / 2;
				if (isPointInCircle(mouseX, mouseY, backCenterX, backCenterY, backRadius)) {
					if (isSoundOn && clickSound != nullptr) {
						Mix_PlayChannel(-1, clickSound, 0);
					}
					gameState = LEVEL_MENU;
					if (walkSoundChannel != -1) {
						Mix_HaltChannel(walkSoundChannel);
						walkSoundChannel = -1;
					}
					if (stretchSoundChannel != -1) {
						Mix_HaltChannel(stretchSoundChannel);
						stretchSoundChannel = -1;
					}
					if (isGameMusicPlaying) {
						Mix_HaltMusic();
						isGameMusicPlaying = false;
					}
					if (isSoundOn && mainMusic != nullptr && !isMainMusicPlaying) {
						Mix_PlayMusic(mainMusic, -1);
						isMainMusicPlaying = true;
					}
				}
			}
		}
		else if (gameState == LOST) {
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				int mouseX = event.button.x;
				int mouseY = event.button.y;

				int replayCenterX = SCREEN_WIDTH / 2 - 60 + shakeX;
				int replayCenterY = SCREEN_HEIGHT / 2 + 20 + shakeY;
				int replayRadius = isReplayButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) / 2 : BUTTON_SIZE / 2;
				if (isPointInCircle(mouseX, mouseY, replayCenterX, replayCenterY, replayRadius)) {
					if (isSoundOn && clickSound != nullptr) {
						Mix_PlayChannel(-1, clickSound, 0);
					}
					resetGame();
					gameState = PLAYING;
				}

				int exitCenterX = SCREEN_WIDTH / 2 + 55 + shakeX;
				int exitCenterY = SCREEN_HEIGHT / 2 + 22 + shakeY;
				int exitRadius = isExitButtonHovered ? static_cast<int>(BUTTON_SIZE * HOVER_SCALE) / 2 : BUTTON_SIZE / 2;
				if (isPointInCircle(mouseX, mouseY, exitCenterX, exitCenterY, exitRadius)) {
					if (isSoundOn && clickSound != nullptr) {
						Mix_PlayChannel(-1, clickSound, 0);
					}
					gameState = LEVEL_MENU;
					if (walkSoundChannel != -1) {
						Mix_HaltChannel(walkSoundChannel);
						walkSoundChannel = -1;
					}
					if (stretchSoundChannel != -1) {
						Mix_HaltChannel(stretchSoundChannel);
						stretchSoundChannel = -1;
					}
					if (isGameMusicPlaying) {
						Mix_HaltMusic();
						isGameMusicPlaying = false;
					}
					if (isSoundOn && mainMusic != nullptr && !isMainMusicPlaying) {
						Mix_PlayMusic(mainMusic, -1);
						isMainMusicPlaying = true;
					}
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
