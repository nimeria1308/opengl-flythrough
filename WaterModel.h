#pragma once

#include <string>
#include "PlaneModel.h"

class WaterModel : public PlaneModel {
public:
	WaterModel(unsigned int xRes, unsigned int zRes);

protected:
	void init();
	virtual void update(float deltaTime);
	float t;
};
#pragma once
