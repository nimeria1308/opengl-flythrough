#include <iostream>

#include "Game.h"
#include "World.h"
#include "HeightmapModel.h"
#include "SkyboxModel.h"

using namespace std;
using namespace glm;

int main(int argc, char* args[])
{
	Game game(1280, 720);
	Camera camera(vec3(12, 75, 84) - vec3(1024.0f / 20), vec3(0, 1, 0), -40, -55);
	camera.movementSpeed *= 100;
	game.camera = camera;

	mat4 model;

	shared_ptr<HeightmapModel> terrain(
		new HeightmapModel(1024, 1024, "./textures/greenland_HF.jpg", "./textures/greenland_TX.jpg"));

	model = mat4(1);
	model = translate(model, vec3(-1024.0f / 20, 0, -1024.0f / 20));
	terrain->model = scale(model, vec3(0.1));

	std::vector<std::string> faces =
	{
		"./textures/right3.jpg",
		"./textures/left3.jpg",
		"./textures/top3.jpg",
		"./textures/bottom3.jpg",
		"./textures/front3.jpg",
		"./textures/back3.jpg"
	};

	shared_ptr<SkyboxModel> skybox(new SkyboxModel(faces));
	model = mat4(1);
	model = scale(model, vec3(1024.0f / 20));
	skybox->model = model;

	shared_ptr<World> world(new World());
	world->models.push_back(terrain);
	world->models.push_back(skybox);

	game.world = world;
	game.loop();

	return 0;
}
