#pragma once

#include <cstdbool>
#include <memory>
#include <string>
#include <vector>

class Texture
{
public:
	Texture(unsigned int ID, unsigned int type) : ID(ID), type(type) { }
	virtual ~Texture();
	void bind();

	static std::shared_ptr<Texture> create2D(const std::string& filename, bool verticalFlip = false);
	static std::shared_ptr<Texture> createCubemap(const std::vector<std::string>& faces, bool verticalFlip = false);

protected:
	const unsigned int ID;
	const unsigned int type;
};
