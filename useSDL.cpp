#include "useSDL.h"
#include "Game.h"
#include <string>

void initSDL() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(16);
	window = SDL_CreateWindow("Bridge", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	font = TTF_OpenFont("Graphics/arial.ttf", 34);

	for (int i = 0; i < 5; i++) {
		std::string filename = "Graphics/Hero" + std::to_string(i + 1) + ".png";
		SDL_Surface* surface = IMG_Load(filename.c_str());
		idleTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}
	for (int i = 0; i < 6; i++) {
		std::string filename = "Graphics/Jump_Hero" + std::to_string(i + 1) + ".png";
		SDL_Surface* surface = IMG_Load(filename.c_str());
		walkTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}

	SDL_Surface* surface = IMG_Load("Graphics/Background2.png");
	homeBackground = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("Graphics/Background2.png");
	levelMenuBackground = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("Graphics/Background1.png");
	gameBackground = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("Graphics/ButtonReplay.png");
	replayButton = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("Graphics/ButtonContinue.png");
	continueButton = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("Graphics/ButtonExit.png");
	exitButton = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("Graphics/ButtonPlay.png");
	playButton = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	for (int i = 0; i < 20; i++) {
		std::string filename = "Graphics/Level" + std::to_string(i + 1) + ".png";
		surface = IMG_Load(filename.c_str());
		levelButton[i] = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}

	surface = IMG_Load("Graphics/LockLevel.png");
	lockedLevel = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("Graphics/Popup.png");
	popup = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("Graphics/TitleMain.png");
	title = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("Graphics/TitleLevel.png");
	SelectLevel = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("Graphics/Platform.png");
	platform = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("Graphics/Lives.png");
	Lives = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	fallSound = Mix_LoadWAV("Audio/Falling.wav");
	Mix_VolumeChunk(fallSound, MIX_MAX_VOLUME / 2);
	hitGroundSound = Mix_LoadWAV("Audio/fall.wav");
	Mix_VolumeChunk(hitGroundSound, MIX_MAX_VOLUME / 2);
	winSound = Mix_LoadWAV("Audio/win.wav");
	mainMusic = Mix_LoadMUS("Audio/Main.wav");
	Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
	playingMusic = Mix_LoadMUS("Audio/Playing.wav");
	Mix_VolumeMusic(MIX_MAX_VOLUME / 6);
	clickSound = Mix_LoadWAV("Audio/ClickButton.wav");
	Mix_VolumeChunk(clickSound, MIX_MAX_VOLUME / 10);
	walkSound = Mix_LoadWAV("Audio/Running.wav");
	Mix_VolumeChunk(walkSound, MIX_MAX_VOLUME);

	stretchSound = Mix_LoadWAV("Audio/StickLength.wav");
	placeSound = Mix_LoadWAV("Audio/StickDown.wav");
	Mix_VolumeChunk(stretchSound, MIX_MAX_VOLUME / 4);
	Mix_VolumeChunk(placeSound, MIX_MAX_VOLUME);

	surface = IMG_Load("Graphics/SoundTurnOn.png");
	soundOnTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("Graphics/SoundTurnOff.png");
	soundOffTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	if (isSoundOn && mainMusic != nullptr) {
		Mix_PlayMusic(mainMusic, -1);
		isMainMusicPlaying = true;
	}
}

void cleanup() {
	for (int i = 0; i < 5; i++) {
		SDL_DestroyTexture(idleTextures[i]);
	}
	for (int i = 0; i < 6; i++) {
		SDL_DestroyTexture(walkTextures[i]);
	}
	SDL_DestroyTexture(homeBackground);
	SDL_DestroyTexture(levelMenuBackground);
	SDL_DestroyTexture(gameBackground);
	SDL_DestroyTexture(replayButton);
	SDL_DestroyTexture(continueButton);
	SDL_DestroyTexture(exitButton);
	SDL_DestroyTexture(playButton);
	for (int i = 0; i < 20; i++) {
		SDL_DestroyTexture(levelButton[i]);
	}
	SDL_DestroyTexture(lockedLevel);
	SDL_DestroyTexture(popup);
	SDL_DestroyTexture(title);
	SDL_DestroyTexture(SelectLevel);
	SDL_DestroyTexture(platform);
	SDL_DestroyTexture(Lives);

	if (fallSound != nullptr) Mix_FreeChunk(fallSound);
	if (hitGroundSound != nullptr) Mix_FreeChunk(hitGroundSound);
	if (winSound != nullptr) Mix_FreeChunk(winSound);
	if (mainMusic != nullptr) Mix_FreeMusic(mainMusic);
	if (playingMusic != nullptr) Mix_FreeMusic(playingMusic);
	if (clickSound != nullptr) Mix_FreeChunk(clickSound);
	if (walkSound != nullptr) Mix_FreeChunk(walkSound);
	if (stretchSound != nullptr) Mix_FreeChunk(stretchSound);
	if (placeSound != nullptr) Mix_FreeChunk(placeSound);
	Mix_CloseAudio();

	SDL_DestroyTexture(soundOnTexture);
	SDL_DestroyTexture(soundOffTexture);

	if (font) TTF_CloseFont(font);
	TTF_Quit();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}