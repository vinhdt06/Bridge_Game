#include "Game.h"

int main(int argc, char* argv[]) {
	bool running = true;
	SDL_Event event;
	srand(time(0));
	initSDL();
	openLevels();
	while (running) {
		setEvent(event, running);
		if (gameState == EXIT) running = false;
		else {
			problemGame();
			faceGame();
			SDL_Delay(16);
		}
	}
	cleanup();
	return 0;
}