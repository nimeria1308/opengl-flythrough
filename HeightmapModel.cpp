#define GLEW_STATIC
#include <GL/glew.h>

#include <memory>

#include "HeightmapModel.h"

using namespace std;
using namespace glm;

HeightmapModel::HeightmapModel(unsigned int xRes, unsigned int zRes, const string& height, const string& color)
	: PlaneModel(xRes, zRes)
{
	init(height, color);
}


void HeightmapModel::init(const string& height, const string& color)
{
	shader = shared_ptr<Shader>(new Shader("./shaders/heightmap.vert", "./shaders/heightmap.frag"));
	shader->use();
	shader->setFloat("width", xRes);
	shader->setFloat("length", zRes);
	shader->setInt("heightmap", 0); //set the heightmap uniform sampler to read from GL_TEXTURE0
	shader->setInt("heightcolor", 1);

	textures.push_back(shared_ptr<Texture>(Texture::create2D(height)));
	textures.push_back(shared_ptr<Texture>(Texture::create2D(color)));
}
