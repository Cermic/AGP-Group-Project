#include "Lights.h"

Lights::Lights()
{
	// Default Constructor
}

// Creates a light object with position and attenuation
Lights::Lights(int lightNum, vec4 lightPosition, GLfloat attenuation)
{
	lightPos = lightPosition;
	attenuationConstant = attenuation;

	//This sets the parameters for any lights created by this class, specifically the ambient,diffuse and specular values
	light[lightNum] = {
		{ 0.5f, 0.5f, 0.5f, 1.0f }, // ambient
		{ 1.0f, 1.0f, 1.0f, 1.0f }, // diffuse
		{ 1.0f, 1.0f, 1.0f, 1.0f }, // specular
		{ lightPos.x, lightPos.y, lightPos.z, 1.0f },  // position
		{ attenuationConstant }, //Const
		{ 0.01f }, //Linear
		{ 0.01f }  //quadratic
	};
}



// Draws a light that takes a modelView stack, a shader program and a projection matrix as parameters.
// The light can be moved in the scene. The light can also have it's attenuation adjusted.
void Lights::draw(int lightNum, stack<glm::mat4> passedStack, GLuint shaderProgram, mat4 projectionMatrix, GLfloat attenuation)
{
	// Uses Utility object to retrieve a shaderProgram ie - util.getLightMapProgram() when drawn
	glUseProgram(shaderProgram);
	stack<glm::mat4> mvStack = passedStack;//Assigns the stack used by the light
	glm::vec4 tmp = mvStack.top() * lightPos;
	light[lightNum].position[0] = tmp.x;
	light[lightNum].position[1] = tmp.y;
	light[lightNum].position[2] = tmp.z;
	rt3d::setLightPos(shaderProgram, value_ptr(tmp));//Adds the shader to the light, taken from the utilities program.
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(projectionMatrix));
}

// Updates the light
void Lights::update(int lightNum, GLfloat attenuation)
{
	light[lightNum].attConst = attenuation;
}
