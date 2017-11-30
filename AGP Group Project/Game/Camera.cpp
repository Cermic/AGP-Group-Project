#include "Camera.h"


Camera::Camera(vec3 eyePos, vec3 atPos, vec3 upDir, GLfloat cameraRotation, float boundingBoxRadius)
{
	eye = eyePos;
	boundingBoxPosition = eyePos;
	boundingRadius = boundingBoxRadius;
	at = atPos;
	up = upDir;
	rotation = cameraRotation;
	aabb = CPM_GLM_AABB_NS::AABB((boundingBoxPosition), boundingRadius);
}

// Keeps the Camera position and rotation up to date
void Camera::update(vec3 atPos, GLfloat rotation)
{
	at = atPos;
	eye = util.moveForward(at, rotation, 1.0f);
	aabb = CPM_GLM_AABB_NS::AABB((eye), boundingRadius);
}
