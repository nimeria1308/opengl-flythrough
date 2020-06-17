#pragma once

#include <memory>
#include <vector>

#include "IRenderable.h"
#include "Model.h"

class World : public IRenderable
{
public:
	virtual ~World() { }

	virtual void render(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& camera);
	void update(float deltaTime);

	std::vector<std::shared_ptr<Model>> models;
};
