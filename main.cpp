#include <iostream>

#include "Game.h"
#include "World.h"
#include "HeightmapModel.h"
#include "SkyboxModel.h"
#include "WaterModel.h"

using namespace std;
using namespace glm;

int main(int argc, char* args[])
{
	Game game(1280, 720);
	//Camera camera(vec3(12, 75, 84) - vec3(1024.0f / 20), vec3(0, 1, 0), -40, -55);
	//game.camera = camera;
	game.camera.movementSpeed *= 100;

	// camera keyframes
	CameraKeyframe f1;
	CameraKeyframe f2;
	CameraKeyframe f3;

	f1.position = vec3(0, 50, -50);
	f1.yaw = 10;
	f1.pitch = -45;
	f1.duration = 100;

	f2.position = vec3(-5, 20, 0);
	f2.yaw = 50;
	f2.pitch = -20;
	f2.duration = 70;

	f3.position = vec3(-30, 25, 10);
	f3.yaw = 10;
	f3.pitch = -10;
	f3.duration = 100;

	game.animator.keyFrames.push_back(f1);
	game.animator.keyFrames.push_back(f2);
	game.animator.keyFrames.push_back(f3);


	mat4 model;

	shared_ptr<HeightmapModel> terrain(
		new HeightmapModel(1024, 1024, "./textures/greenland_HF.jpg", "./textures/greenland_TX.jpg"));

	model = mat4(1);
	model = translate(model, vec3(-1024.0f / 20, 0, -1024.0f / 20));
	model = scale(model, vec3(0.1));
	terrain->model = model;

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
	model = scale(model, vec3(1024.0f / 4));
	skybox->model = model;

	shared_ptr<WaterModel> water(new WaterModel(1024, 1024));
	model = mat4(1);
	model = translate(model, vec3(-1024.0f / 4, 5.0f, -1024.0f / 4));
	model = scale(model, vec3(0.5));
	water->model = model;
	
	shared_ptr<World> world(new World());
	world->models.push_back(terrain);
	world->models.push_back(water);
	world->models.push_back(skybox);

	game.world = world;
	game.loop();

	return 0;
}
