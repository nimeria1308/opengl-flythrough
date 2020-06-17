#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>
#include <glm/glm.hpp>

#include "Game.h"

using namespace std;
using namespace glm;

Game::Game(int width, int height)
	: window(NULL), context(NULL),
	width(width), height(height), ratio((float)width / height)
{
	init();
}

Game::~Game()
{
	//Delete OGL context
	if (context) {
		SDL_GL_DeleteContext(context);
	}

	//Destroy window
	if (window) {
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
}

void Game::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cerr << "SDL could not initialize: " << SDL_GetError() << endl;
		abort();
	}

	//Use OpenGL 3.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	//Create window
	window = SDL_CreateWindow(
		"OpenGL Flythrough",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		cerr << "Window could not be created: " << SDL_GetError() << endl;
		abort();
	}

	//Create context
	context = SDL_GL_CreateContext(window);
	if (context == NULL)
	{
		cerr << "OpenGL context could not be created: " << SDL_GetError() << endl;
		abort();
	}

	//Use Vsync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		cerr << "Warning: Unable to set VSync: " << SDL_GetError() << endl;
	}

	//Init GL
	glewInit();
	GLenum gl_error = glGetError();
	if (gl_error != GL_NO_ERROR)
	{
		cerr << "Error initializing OpenGL: " << gl_error << endl;
		abort();
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Game::loop()
{
	// mouse
	bool firstMouse = true;
	float lastX = 0;
	float lastY = 0;

	// timing
	float lastFrame = 0;
	float deltaTime;

	while (true)
	{
		// per-frame time logic
		// --------------------
		float currentFrame = SDL_GetTicks() / 1000.0f;
		deltaTime = (currentFrame - lastFrame) * 10;
		lastFrame = currentFrame;

		//Handle events on queue
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{

			case SDL_WINDOWEVENT:
			{
				switch (e.window.event) {
				case SDL_WINDOWEVENT_ENTER:
					// reset the mouse when entering the window
					// so that one can pitch / law fully
					firstMouse = true;
					break;
				}
				break;
			}

			case SDL_QUIT:
				return;

			case SDL_KEYDOWN:
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					return;

				case SDLK_w:
					camera.processKeyboard(FORWARD, deltaTime);
					break;

				case SDLK_s:
					camera.processKeyboard(BACKWARD, deltaTime);
					break;

				case SDLK_a:
					camera.processKeyboard(LEFT, deltaTime);
					break;

				case SDLK_d:
					camera.processKeyboard(RIGHT, deltaTime);
					break;
				}

				break;
			}

			case SDL_MOUSEMOTION:
			{
				if (!firstMouse)
				{
					camera.processMouseMovement(e.motion.x - lastX, lastY - e.motion.y);
				}

				lastX = e.motion.x;
				lastY = e.motion.y;
				firstMouse = false;
				break;
			}

			case SDL_MOUSEWHEEL:
				camera.processMouseScroll(e.wheel.y);
				break;
			}
		}

		//Render
		render(deltaTime);

		//Update screen
		SDL_GL_SwapWindow(window);
	}
}

void Game::render(float deltaTime)
{
	world->update(deltaTime);
	animator.updateCamera(camera, deltaTime);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 view = camera.getViewMatrix();
	mat4 proj = glm::perspective(radians(camera.zoom), ratio, 0.1f, 10000.0f);
	world->render(view, proj);
}
