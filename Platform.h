#ifndef PLATFORM_H
#define PLATFORM_H
#include <SDL.h>

struct Platform {
	int x, y, w, h;
	int firstPos = 0;
	int timeDisappear = 0;
	bool platformsDisappear = false;
	int velocity = 0;
	int platformsMove = 0;
	bool hasMark = false;
	SDL_Rect markRect;
};

#endif
