#pragma once
#include <SDL2/SDL_image.h>

class Timer
{
public:
	Timer();

	Uint32 startTicks;
	Uint32 pausedTicks;
	bool paused;
	bool started;

	void start();
	void stop();
	void pause();
	void unpause();
	Uint32 getTicks();
	bool isStarted();
	bool isPaused();
};