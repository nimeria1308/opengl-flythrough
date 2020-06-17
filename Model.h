#pragma once

#include <memory>
#include <vector>

#include "IRenderable.h"
#include "Shader.h"
#include "Texture.h"

class Model : public IRenderable {
public:
	Model();
	virtual ~Model() { }
	virtual void render(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& camera);
	virtual void update(float deltaTime) { }
	glm::mat4 model;

protected:
	std::shared_ptr<Shader> shader;
	std::vector<std::shared_ptr<Texture>> textures;
};
