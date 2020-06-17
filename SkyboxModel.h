#pragma once

#include <string>
#include <vector>

#include "BoxModel.h"

class SkyboxModel : public BoxModel
{
public:
	SkyboxModel(const std::vector<std::string>& faces);

protected:
	void init(const std::vector<std::string>& faces);
};

