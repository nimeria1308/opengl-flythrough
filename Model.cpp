#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "Model.h"

using namespace std;
using namespace glm;

Model::Model()
	: model(1)
{
}

void Model::render(const mat4& view, const mat4& proj)
{
	shader->use();

	shader->setMat4("view", view);
	shader->setMat4("proj", proj);
	shader->setMat4("model", model);

	for (int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		textures[i]->bind();
	}
}
