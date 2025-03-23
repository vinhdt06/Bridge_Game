#include "Game.h"

int main() {
	bool running = true;
	SDL_Event event;
	srand(time(0));
	initSDL();
	saveLevels();
	cleanup();
	return 0;
}