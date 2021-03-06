#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <memory>

#include "Camera.h"
#include "CameraAnimator.h"
#include "World.h"

class Game
{
public:
	Game(int width, int height);
	virtual ~Game();
	void loop();
	std::shared_ptr<World> world;

	Camera camera;
	CameraAnimator animator;

protected:
	void init();
	void render(float deltaTime, bool manualCamera);

	// window and GL
	SDL_Window* window;
	SDL_GLContext context;
	int width;
	int height;
	float ratio;
};
