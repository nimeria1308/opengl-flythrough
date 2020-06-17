#pragma once

#include <string>
#include "PlaneModel.h"

class HeightmapModel : public PlaneModel {
public:
	HeightmapModel(unsigned int xRes, unsigned int zRes, const std::string& height, const std::string& color);

protected:
	void init(const std::string& height, const std::string& color);
};
