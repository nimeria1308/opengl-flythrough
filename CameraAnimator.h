#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Camera.h"

class CameraKeyframe {
public:
	CameraKeyframe()
		: yaw(0), pitch(0), zoom(0), duration(10), elapsed(0)
	{
	}

	glm::vec3 position;
	float yaw;
	float pitch;
	float zoom;
	float duration;
	float elapsed;
};

class CameraAnimator {
public:
	CameraAnimator()
		: current(0)
	{
	}

	std::vector<CameraKeyframe> keyFrames;
	void updateCamera(Camera& camera, float deltaTime);

protected:
	int current;
};