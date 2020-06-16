#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

#include "Shader.h"
#include "Texture.h"

struct ComplexMeshVertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};

struct ComplexMeshTexture {
	std::shared_ptr<Texture> texture;
	std::string type;
	std::string path;
};

class ComplexMesh {
public:
	/*  Mesh Data  */
	std::vector<ComplexMeshVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<ComplexMeshTexture> textures;
	unsigned int VAO;

	/*  Functions  */
	// constructor
	ComplexMesh(std::vector<ComplexMeshVertex>& vertices, std::vector<unsigned int>& indices, std::vector<ComplexMeshTexture>& textures);
	virtual ~ComplexMesh();

	// render the mesh
	void draw(Shader& shader);

private:
	/*  Render data  */
	unsigned int VBO;
	unsigned int EBO;

	/*  Functions    */
	// initializes all the buffer objects/arrays
	void init();
};
