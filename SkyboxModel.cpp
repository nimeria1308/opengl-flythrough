#define GLEW_STATIC
#include <GL/glew.h>

#include <memory>

#include "SkyboxModel.h"

using namespace std;
using namespace glm;

SkyboxModel::SkyboxModel(const vector<string>& faces)
{
	init(faces);
}

void SkyboxModel::init(const vector<string>& faces)
{
	shader = shared_ptr<Shader>(new Shader("./shaders/skybox.vert", "./shaders/skybox.frag"));
	shader->use();
	shader->setInt("skybox", 0); //set the skybox uniform sampler to read from GL_TEXTURE0

	textures.push_back(shared_ptr<Texture>(Texture::createCubemap(faces)));
}
