#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <stack>
#include "Utilities.h"
using namespace glm;


class FBO
{
private:
	unsigned int depthMapFBO;
	unsigned int depthMap;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
public:
	FBO();
	void generateShadowFBO();
	void firstPass(vec3 lightPos, GLuint shader);
	void secondPass(GLuint shader);
};