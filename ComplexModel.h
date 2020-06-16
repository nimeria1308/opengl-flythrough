#pragma once 

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <memory>
#include <string>
#include <vector>

#include "ComplexMesh.h"
#include "Shader.h"

class ComplexModel
{
public:
	/*  Model Data */
	std::vector<ComplexMeshTexture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<std::shared_ptr<ComplexMesh>> meshes;
	std::string directory;
	bool gammaCorrection;

	/*  Functions   */
	// constructor, expects a filepath to a 3D model.
	ComplexModel(const std::string& path, bool gamma = false);
	virtual ~ComplexModel();

	// draws the model, and thus all its meshes
	void draw(Shader& shader);

private:
	/*  Functions   */
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void init(const std::string& path);

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene);
	std::shared_ptr<ComplexMesh> processMesh(aiMesh* mesh, const aiScene* scene);

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	std::vector<ComplexMeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
};
