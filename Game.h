#pragma once

#include <SDL.h>
#include <SDL_opengl.h>

#include "Camera.h"

class Game
{
public:
	Game(int width, int height);
	virtual ~Game();
	void loop();

protected:
	void init();
	void render(float deltaTime);

	// window and GL
	SDL_Window* window;
	SDL_GLContext context;
	int width;
	int height;

	Camera camera;
};
