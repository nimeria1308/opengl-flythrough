#pragma once

#include "Model.h"

class BoxModel : public Model
{
public:
	BoxModel();
	virtual void render(const glm::mat4& view, const glm::mat4& proj);

protected:
	void init();

	unsigned int xRes, zRes;
	unsigned int vbo, vao;
};
