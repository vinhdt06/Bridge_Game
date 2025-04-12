#include "Game.h"
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[]) {
    srand(time(0));
    initSDL();
    openLevels();
    bool running = true;
    SDL_Event event;
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