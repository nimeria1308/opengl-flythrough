#pragma once

#include "Model.h"

class PlaneModel : public Model
{
public:
	PlaneModel(unsigned int xRes, unsigned int zRes);
	virtual void render(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& camera);

protected:
	void init();

	unsigned int xRes, zRes;
	unsigned int vbo, ebo, vao;
};
