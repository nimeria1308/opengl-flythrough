#define GLEW_STATIC
#include <GL/glew.h>

#include <memory>

#include "WaterModel.h"

using namespace std;
using namespace glm;

WaterModel::WaterModel(unsigned int xRes, unsigned int zRes)
	: PlaneModel(xRes, zRes), t(0)
{
	init();
}


void WaterModel::init()
{
	shader = shared_ptr<Shader>(new Shader("./shaders/water.vert", "./shaders/water.frag"));
	shader->use();

	shader->setFloat("width", xRes);
	shader->setFloat("length", zRes);
	shader->setInt("heightmap", 0); //set the heightmap uniform sampler to read from GL_TEXTURE0

	textures.push_back(shared_ptr<Texture>(Texture::create2D("./textures/water.jpg")));
}

void WaterModel::update(float deltaTime)
{
	t += deltaTime;
	shader->use();
	shader->setFloat("time", t);
}

