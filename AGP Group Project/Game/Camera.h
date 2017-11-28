#pragma once

#include "../Game/Utilities/Utilities.h"
#include "../Game/Utilities/AABB.hpp"
#include <stack>
#include <glm/glm.hpp>
#include <GL\glew.h>
// Camera only needs utilities to load shaders and the stack for matrix operations.

using std::stack;
using glm::vec3;

class Camera
{
private:
	//camera properties
	vec3 eye;
	vec3 at;
	vec3 up;
	GLfloat rotation;
	float boundingRadius;
	// Varibales for camera declared
	Utilities util; //util object for access to movement methods
	CPM_GLM_AABB_NS::AABB aabb;
	vec3 boundingBoxPosition;
	// Bounding box for the camera;
public:
	Camera() {};
	Camera(vec3 eye, vec3 at, vec3 up, GLfloat rotation) :eye(eye), at(at), up(up), rotation(rotation) {};
	Camera(vec3 eye, vec3 at, vec3 up, GLfloat rotation, float boundingBoxRadius);
	//Camera Constructor
	vec3 getEyePos() { return eye; }
	vec3 getAt() { return at; }
	vec3 getUp() { return up; }
	GLfloat getRotation() { return rotation; }
	void setEyePos(vec3 e) { eye = e; }
	void setAt(vec3 a) { at = a; }
	void setUp(vec3 u) { up = u; }
	void setRotation(GLfloat rot) { rotation = rot; }
	CPM_GLM_AABB_NS::AABB getBoundingBox() { return aabb; }
	void setBoundingBox(CPM_GLM_AABB_NS::AABB newBoundingBox) { aabb = newBoundingBox; }
	float getBoundingRadius() { return boundingRadius; }
	void setBoundingRadius(float br) { boundingRadius = br; }
	vec3 getBoundingBoxPosition() { return boundingBoxPosition; }
	void setBoundingBoxPos(vec3 boxPosition) { boundingBoxPosition = boxPosition; }
	// Accessors and mutators
	void update(vec3 atPos, GLfloat rotation);
	

};
