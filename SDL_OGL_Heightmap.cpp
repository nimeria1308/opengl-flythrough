#define GLEW_STATIC
#include <GL\glew.h>

#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <gl\GLU.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <iostream>

#include "Shader.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool init();
bool initGL();
void render();
GLuint CreatePlane(GLuint&, GLuint&);
void DrawPlane(GLuint id);
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

Shader gShader;

GLuint gVAO, gVBO, gEBO;

GLuint texID;
//resolution = number of vertices per axis
int xRes = 50, zRes = 50; //y axis is for height
float step = 1.0f; //the step between the vertices

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 20.0f));
float lastX = -1;
float lastY = -1;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

bool LoadTexture(const char* filename, GLuint& texID);

//event handlers
void HandleKeyDown(const SDL_KeyboardEvent& key);
void HandleMouseMotion(const SDL_MouseMotionEvent& motion);
void HandleMouseWheel(const SDL_MouseWheelEvent& wheel);
void HandleMouseButtonUp(const SDL_MouseButtonEvent& button);

int main(int argc, char* args[])
{
	init();

	SDL_Event e;
	//While application is running
	bool quit = false;
	while (!quit)
	{
		// per-frame time logic
		// --------------------
		float currentFrame = SDL_GetTicks() / 1000.0f;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					quit = true;
				}
				else
				{
					HandleKeyDown(e.key);
				}
				break;
			case SDL_MOUSEMOTION:
				HandleMouseMotion(e.motion);
				break;
			case SDL_MOUSEWHEEL:
				HandleMouseWheel(e.wheel);
				break;
			case SDL_MOUSEBUTTONUP:
				HandleMouseButtonUp(e.button);
				break;
			}
		}

		//Render
		render();

		//Update screen
		SDL_GL_SwapWindow(gWindow);
	}

	close();

	return 0;
}

void HandleKeyDown(const SDL_KeyboardEvent& key)
{
	switch (key.keysym.sym)
	{
	case SDLK_w:
		camera.ProcessKeyboard(FORWARD, deltaTime);
		break;
	case SDLK_s:
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		break;
	case SDLK_a:
		camera.ProcessKeyboard(LEFT, deltaTime);
		break;
	case SDLK_d:
		camera.ProcessKeyboard(RIGHT, deltaTime);
		break;
	}
}

void HandleMouseMotion(const SDL_MouseMotionEvent& motion)
{
	if (firstMouse)
	{
		lastX = motion.x;
		lastY = motion.y;
		firstMouse = false;
	}
	else
	{
		camera.ProcessMouseMovement(motion.x - lastX, lastY - motion.y);
		lastX = motion.x;
		lastY = motion.y;
	}
}

void HandleMouseWheel(const SDL_MouseWheelEvent& wheel)
{
	camera.ProcessMouseScroll(wheel.y);
}

void HandleMouseButtonUp(const SDL_MouseButtonEvent& button)
{
	if (button.clicks == 1 && button.button == SDL_BUTTON_LEFT)
	{
	}
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Use OpenGL 3.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);


		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN /*| SDL_WINDOW_FULLSCREEN*/);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create context
			gContext = SDL_GL_CreateContext(gWindow);
			if (gContext == NULL)
			{
				printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Use Vsync
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
				}

				//Initialize OpenGL
				if (!initGL())
				{
					printf("Unable to initialize OpenGL!\n");
					success = false;
				}
			}
		}
	}

	return success;
}

bool initGL()
{
	bool success = true;
	GLenum error = GL_NO_ERROR;

	glewInit();

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		success = false;
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	gShader.Load("./shaders/vertex.vert", "./shaders/fragment.frag");
	gShader.use();
	gShader.setFloat("width", xRes);
	gShader.setFloat("length", zRes);
	gShader.setInt("heightmap", 0); //set the heightmap uniform sampler to read from GL_TEXTURE0
	
	if (!LoadTexture("./images/height.jpg", texID))
	{
		printf("Unable to load heighmap image!\n");
	}

	gVAO = CreatePlane(gVBO, gEBO);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //other modes GL_FILL, GL_POINT

	return success;
}

void close()
{
	//delete GL programs, buffers and objects
	glDeleteProgram(gShader.ID);
	glDeleteVertexArrays(1, &gVAO);
	glDeleteBuffers(1, &gVBO);
	glDeleteBuffers(1, &gEBO);

	//Delete OGL context
	SDL_GL_DeleteContext(gContext);
	//Destroy window	
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void render()
{
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 model = glm::mat4(1);
	model = glm::translate(model, glm::vec3(-xRes / 2.0f, 0, 0));

	glUseProgram(gShader.ID);

	gShader.setMat4("view", view);
	gShader.setMat4("proj", proj);
	gShader.setMat4("model", model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	DrawPlane(gVAO);
}

GLuint CreatePlane(GLuint& VBO, GLuint& EBO)
{
	//procedurally generate vertices for a plane
	float *vertices = new float[xRes * zRes * 3];
	//todo: add code to generate the vertices using xRes, zRes and step (global variables)

	//generate the indices to draw the plane with triangle strips using multiple glDrawElements calls, see DrawPlane
	int* indices = new int[xRes * 2 * (zRes - 1)];
	//todo: add code to generate the indices to build triangle strips from the vertices

	GLuint VAO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, xRes * zRes * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, xRes * 2 * (zRes - 1) * sizeof(int), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //the data comes from the currently bound GL_ARRAY_BUFFER
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	//the elements buffer must be unbound after the vertex array otherwise the vertex array will not have an associated elements buffer array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] vertices;
	delete[] indices;

	return VAO;
}

void DrawPlane(GLuint vaoID)
{
	gShader.use();
	glBindVertexArray(vaoID);

	//glDrawElements uses the indices in the EBO to get to the vertices in the VBO
	//all indices are xRes * 2 * (zRes - 1)
	//a single row of triangle contains xRes * 2 indices (for triangle strip)
	for (int z = 0; z < zRes - 1; z++)
	{
		glDrawElements(GL_TRIANGLE_STRIP, xRes * 2, GL_UNSIGNED_INT, (void*)(z*xRes*2*sizeof(int)));
	}
	//this uses multiple glDrawElements calls
	//another option is to use degenerate triangles - see http://www.learnopengles.com/tag/triangle-strips/

	glBindVertexArray(0);
}

bool LoadTexture(const char* filename, GLuint& texID)
{
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //these are the default values for warping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// read the texture
	GLint width, height, channels;
	stbi_set_flip_vertically_on_load(true); //flip the image vertically while loading
	unsigned char* img_data = stbi_load(filename, &width, &height, &channels, 0); //read the image data

	if (img_data)
	{   //3 channels - rgb, 4 channels - RGBA
		GLenum format;
		switch (channels)
		{
		case 4:
			format = GL_RGBA;
			break;
		case 1:
			format = GL_R;
			break;
		default:
			format = GL_RGB;
			break;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, img_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture\n");
	}
	stbi_image_free(img_data);

	return true;
}
