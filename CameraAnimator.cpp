#include "CameraAnimator.h"

using namespace glm;

void CameraAnimator::updateCamera(Camera& camera, float deltaTime)
{
	// check which keyframe
	while (true)
	{
		CameraKeyframe& frame = keyFrames[current % keyFrames.size()];
		frame.elapsed += deltaTime;
		if (frame.elapsed < frame.duration) {
			break;
		}
		// remainder
		deltaTime = frame.elapsed - frame.duration;
		frame.elapsed = 0;
		current++;
	}

	// normalize elapsed
	CameraKeyframe& frame = keyFrames[current % keyFrames.size()];
	float t = frame.elapsed / frame.duration;

	// interpolate
	CameraKeyframe& next = keyFrames[(current + 1) % keyFrames.size()];
	
	vec3 position = frame.position + ((next.position - frame.position) * t);
	float yaw = frame.yaw + ((next.yaw - frame.yaw) * t);
	float pitch = frame.pitch + ((next.pitch - frame.pitch) * t);
	float zoom = frame.zoom + ((next.zoom - frame.zoom) * t);

	camera.position = position;
	camera.yaw = yaw;
	camera.pitch = pitch;
	camera.updateCameraVectors();
	//camera.zoom = zoom;
}
