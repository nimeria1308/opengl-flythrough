#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>

#include "Image.h"
#include "Texture.h"

using namespace std;

Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}

void Texture::bind()
{
	glBindTexture(type, ID);
}

static GLenum channelsToFormat(int channels)
{
	switch (channels)
	{
	case 4:
		return GL_RGBA;
	case 3:
		return GL_RGB;
	case 1:
		return GL_R;
	default:
		cerr << "Unknown channel count" << channels << endl;
	}
}

shared_ptr<Texture> Texture::create2D(const string& filename, bool verticalFlip)
{
	GLuint ID;
	glGenTextures(1, &ID);
	shared_ptr<Texture> texture(new Texture(ID, GL_TEXTURE_2D));
	texture->bind();

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //these are the default values for warping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// read the texture
	Image image(filename, verticalFlip);
	GLenum format = channelsToFormat(image.getChannels());

	glTexImage2D(GL_TEXTURE_2D, 0, format, image.getWidth(), image.getHeight(), 0, format, GL_UNSIGNED_BYTE, image.getData());
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}

std::shared_ptr<Texture> Texture::createCubemap(const std::vector<std::string>& faces, bool verticalFlip)
{
	GLuint ID;
	glGenTextures(1, &ID);

	shared_ptr<Texture> texture(new Texture(ID, GL_TEXTURE_CUBE_MAP));
	texture->bind();

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		Image image(faces[i], verticalFlip);
		GLenum format = channelsToFormat(image.getChannels());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, format, image.getWidth(), image.getHeight(), 0, format, GL_UNSIGNED_BYTE, image.getData());
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return texture;
}
