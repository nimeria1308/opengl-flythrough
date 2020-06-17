#pragma once

#include <glm/glm.hpp>

class IRenderable {
public:
	virtual ~IRenderable() { }
	virtual void render(const glm::mat4 &view, const glm::mat4& proj) = 0;
};