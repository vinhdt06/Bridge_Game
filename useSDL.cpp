#include "useSDL.h"
#include "Game.h"

void initSDL() {
	IMG_Init(IMG_INIT_PNG);
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Bridge", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMORE_BLEND);

	for (int i = 0; i < 5; i++) {
		std::string filename = "Hero" + std::to_string(i + 1) + ".png";
		SDL_Surface* surface = IMG_Load(filename.c_str());
		if (!surface) {
			std::cerr << "Failed to load " << filename << ": " << IMG_GetError() << std::endl;
			continue;
		}
		idleTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
 	}
	for (int i = 0; i < 6; i++) {
		std::string filename = "Iump_Hero" + std::to_string(i + 1) + ".png";
		SDL_Surface* surface = IMG_Load(filename.c_str());
		if (!surface) {
			std::cerr << "Failed to load " << filename << ": " << IMG_GetError() << std::endl;
			continue;
		}
		walkTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}

	SDL_Surface* surface = IMG_Load("Background2.png");
	homeBackground = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Surface* surface = IMG_Load("Background2.png");
	levelMenuBackground = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Surface* surface = IMG_Load("Background1.png");
	gameBackground = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
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
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}