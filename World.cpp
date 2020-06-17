#pragma once

#include "World.h"

void World::render(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& camera)
{
	for (int i = 0; i < models.size(); i++) {
		models[i]->render(view, proj, camera);
	}
}

void World::update(float deltaTime)
{
	for (int i = 0; i < models.size(); i++) {
		models[i]->update(deltaTime);
	}
}
