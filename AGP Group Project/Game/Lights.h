#pragma once

#include "../Game/Utilities/Utilities.h"
#include "../Game/Utilities/rt3d.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>
// Includes Utilties for loading shaders and glm operationd for transformations, rotations, scales and so on. 

using glm::vec4;

class Lights
{
private:

	rt3d::lightStruct light[3]; // initialises a light structure array
	vec4 lightPos;// vector 4 for light position
	GLfloat attenuationConstant;
	GLuint shaderProgram; // GLuint for the shader
	Utilities utilObject; // declares Utility object

public:
	Lights();// Default Constructor
	Lights(int lightNum, vec4 lightPosition, GLfloat attenuation);// constructor takes in lightPosition

	rt3d::lightStruct getLight(int lightNum) { return light[lightNum]; }
	vec4 getLightPos() { return lightPos; }
	void setLightPos(vec4 pos) { lightPos = pos; }
	GLfloat getAttenuationConstant() { return attenuationConstant; }
	void setAttenuationConstant(GLfloat attConst) { attenuationConstant = attConst; }
	// Accesors and mutators
	void draw(int lightNum, stack<glm::mat4> mvStack, GLuint shaderProgram, mat4 projectionMatrix, GLfloat attenuation); // overloaded draw method for light that allows for adjusting attenuation
	void update(int lightNum, GLfloat attenuation); // updates the light
};