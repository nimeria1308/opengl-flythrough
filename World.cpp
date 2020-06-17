#pragma once

#include "World.h"

void World::render(const glm::mat4& view, const glm::mat4& proj)
{
	for (int i = 0; i < models.size(); i++) {
		models[i]->render(view, proj);
	}
}

void World::update(float deltaTime)
{
	for (int i = 0; i < models.size(); i++) {
		models[i]->update(deltaTime);
	}
}
