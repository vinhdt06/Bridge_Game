#include "useSDL.h"
#include "Game.h"

void initSDL() {
	IMG_Init(IMG_INIT_PNG);
	window = SDL_CreateWindow("Bridge", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Surface* surface = IMG_Load("");
	heroCharacter = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Surface* surface = IMG_Load("");
	homeBackground = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Surface* surface = IMG_Load("");
	levelMenuBackground = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Surface* surface = IMG_Load("");
	gameBackground = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

void cleanup() {
	SDL_DestroyTexture(heroCharacter);
	SDL_DestroyTexture(homeBackground);
	SDL_DestroyTexture(levelMenuBackground);
	SDL_DestroyTexture(gameBackground);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}