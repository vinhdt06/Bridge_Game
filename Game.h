#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include "Platform.h"
#include "useSDL.h"

const int SCREEN_WIDTH = 850;
const int SCREEN_HEIGHT = 600;
const int PLATFORM_WIDTH = 53;
const int PLATFORM_HEIGHT = 270;
const int STICK_WIDTH = 5;
const int PLATFORM_POS = 330;
const int PLAT_DIS_MAX = 100;
const int PLAT_DIS_MIN = 200;
const int WIN_PLATFORMS = 4;
const int HERO_SPEED = 5;
const int TIME_STICK = 100;
const int LEVEL_WIDTH_BUTTON = 80;
const int LEVEL_HEIGHT_BUTTON = 80;
const int LEVEL_DIS_BUTTON = 20;
const int SCROLL_SPEED = 10;
const float BACKGROUND_SCROLL = 0.3f;
const int ANIMATION_SPEED = 200;
const int BUTTON_SIZE = 80;
const float HOVER_SCALE = 1.2f;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* homeBackground;
extern SDL_Texture* levelMenuBackground;
extern SDL_Texture* gameBackground;
extern SDL_Texture* replayButton;
extern SDL_Texture* continueButton;
extern SDL_Texture* exitButton;
extern SDL_Texture* playButton;
extern SDL_Texture* levelButton[20];
extern SDL_Texture* lockedLevel;
extern SDL_Texture* popup;
extern SDL_Texture* title;
extern SDL_Texture* idleTextures[5];
extern SDL_Texture* walkTextures[6];
extern std::vector<Platform> platforms;
extern SDL_Rect hero;
extern SDL_Rect stick;
extern SDL_Texture* soundOnTexture;
extern SDL_Texture* soundOffTexture;

extern bool stickLength;
extern bool stickTurn;
extern bool stickDown;
extern bool heroWalk;
extern bool heroFall;
extern bool heroAfterWalk;
extern bool completeLevel[21];
extern bool scrollScreen;
extern bool camePlatforms;
extern bool wonGame;
extern bool showWin;
extern bool waitingForRestart;
extern bool hasPlayedFallSound;
extern bool hasPlayedWinSound;
extern bool hasPlayedHitGroundSound;
extern bool hasPlayedPlaceSound;
extern bool isSoundOn;
extern bool isMainMusicPlaying;
extern bool isGameMusicPlaying;
extern bool isSoundButtonHovered;
extern bool isPlayButtonHovered;
extern bool isExitButtonHovered;
extern bool isBackButtonHovered;
extern bool isReplayButtonHovered;
extern bool isContinueButtonHovered;
extern bool levelButtonHovered[20];
extern bool isShaking;

extern double stickAngle;
extern int platformsPassed;
extern int platformsWin;
extern int newLevel;
extern int stickTime;
extern int stickTimeOk;
extern int heroPos;
extern int selectLevel;
extern int scrollSpeed;
extern int background;
extern int currentFrame;
extern int backgroundX;
extern int walkSoundChannel;
extern int stretchSoundChannel;
extern int fallSoundChannel;
extern int shakeDuration;
extern int shakeMagnitude;
extern int shakeX;
extern int shakeY;

extern Mix_Chunk* fallSound;
extern Mix_Chunk* hitGroundSound;
extern Mix_Chunk* winSound;
extern Mix_Music* mainMusic;
extern Mix_Music* playingMusic;
extern Mix_Chunk* clickSound;
extern Mix_Chunk* walkSound;
extern Mix_Chunk* stretchSound;
extern Mix_Chunk* placeSound;

enum GameState {
	MAIN_MENU,
	LEVEL_MENU,
	PLAYING,
	WON,
	LOST,
	EXIT
};

extern Uint32 lastFrameTime;
extern GameState gameState;
extern TTF_Font* font;
extern SDL_Color textColor;

void initSDL();
void renderText(const char* text, int x, int y, SDL_Color color);
void createPlatforms();
void Levels(int level);
void MainMenu();
void LevelMenu();
void ifWinGame();
void renderSoundButton();
void saveLevels();
void openLevels();
bool isPointInCircle(int x, int y, int centerX, int centerY, int radius);
int indexPlatforms();
void problemGame();
void faceGame();
void setEvent(SDL_Event& event, bool& running);
void resetGame();
void cleanup();

#endif