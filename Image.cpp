#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Image.h"

using namespace std;

Image::Image(const std::string& filename, bool verticalFlip)
	: data(NULL), width(0), height(0), channels(0)
{
	init(filename, verticalFlip);
}

Image::~Image()
{
	stbi_image_free(data);
}

void Image::init(const std::string& filename, bool verticalFlip)
{
	stbi_set_flip_vertically_on_load(verticalFlip);

	data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
	if (data == NULL) {
		cerr << "Could not load " << filename << endl;
		abort();
	}
}
