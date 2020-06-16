#pragma once

#include <cstdbool>
#include <memory>
#include <string>
#include <vector>

class Texture
{
public:
	Texture(unsigned int ID) : ID(ID) { }
	virtual ~Texture();

	static std::shared_ptr<Texture> create2D(const std::string& filename, bool verticalFlip = false);
	static std::shared_ptr<Texture> createCubemap(std::vector<std::string>& faces, bool verticalFlip = false);

	const unsigned int ID;
};