#pragma once

#include <string>
#include <cstdbool>

class Image
{
public:
	Image(const std::string& filename, bool verticalFlip = false);
	virtual ~Image();

	const unsigned char* getData() const { return data; };
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	int getChannels() const { return channels; }

protected:
	void init(const std::string& filename, bool verticalFlip);
	unsigned char* data;
	int width;
	int height;
	int channels;
};