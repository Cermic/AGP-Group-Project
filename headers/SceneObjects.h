#pragma once

#include "Utilities.h"
#include "AABB.hpp"
#include "Lights.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include "VBO.h" // needed to create the tangent vbo during object initialisation
#include "Calculations.h" //needed to create tangent vector during object initialisation

// Scene objects includes Utilties to load shaders and texutures
// Includes Lights to allow the objects to determine how they will react to a light
// The stack for matrix operations

using glm::mat4;
using glm::vec3;

class SceneObjects
{
private:
	vec3 objectPos;
	// Object position in the scene
	GLuint texture;
	GLuint texture2;
	GLuint texture3;
	// Textures for the objects
	int i_texture_isvisible = 0;
	int texel_specular_value = 0;
	// ints to be passed into the shader
	vec3 objectSize;
	GLfloat rotationAmount;
	GLuint meshIndexCount = 0;
	GLuint meshObject;
	Utilities util;
	Lights light;
	rt3d::materialStruct material;
	CPM_GLM_AABB_NS::AABB aabb;
	GLuint lightOn = 1;
	bool doorMove = false;
	vec3 boundingBoxPosition;
	vec3 objectScale;
	bool isDrawn = true;

	// Utilities object loaded to access the shaders, the light to allow for setLight commmands
	// To be called and a material is needed as a filler value for the cubes initially.
public:
	//constructor for basic buildings
	SceneObjects();
	//constructor for scalable objects with a bounding volume
	//constructors without rotation
	SceneObjects(vec3 objectPos, vec3 objectScale, char * textureName);
	SceneObjects(vec3 objectPos, vec3 objectScale, char * textureName, char * textureName2);
	SceneObjects(vec3 objectPos, vec3 objectScale, GLfloat rotation, char * textureName);
	SceneObjects(vec3 objectPos, vec3 objectScale, GLfloat rotation, char * textureName, char * textureName2);
	SceneObjects(vec3 objectPosition, vec3 objectScale, GLfloat rotation, char * textureName, char * textureName2, char * textureName3);
	//Constrcutors with rotation
	// There are a number of constructors to allow for the creation of objects with and without double texturing and with or without rotation.
	GLfloat getRotation() { return rotationAmount; }
	void setRotation(GLfloat newRotation) { rotationAmount = newRotation; }
	vec3 getPosition() { return objectPos; }
	void setPosition(vec3 newPos) { objectPos = newPos; }
	vec3 getBoundingBoxPosition() { return boundingBoxPosition; }
	void setBoundingBoxPos(vec3 boxPosition) { boundingBoxPosition = boxPosition; }
	vec3 getObjectScale() { return objectScale; }
	void setObjectScale(vec3 newObjectScale) { newObjectScale = objectScale; }
	bool getIsDrawn() { return isDrawn; }
	void setIsDrawn(bool newIsDrawn) { isDrawn = newIsDrawn; }
	bool getDoorMovement() { return doorMove; }
	void setDoorMovement(bool newDoorMove) { doorMove = newDoorMove; }
	int getTextureVisible() { return i_texture_isvisible; }
	void setTextureVisible(int textureV) { i_texture_isvisible = textureV; }
	int getSpecularValue() { return texel_specular_value; }
	void setSpecularValue(int specularV) { texel_specular_value = specularV; }
	void setTextures(char * textureName, char * textureName2);
	void setTextures(char * textureName, char * textureName2, char * textureName3);// try not to make overloaded mutators. Call them something else!
	GLuint getLightOn() { return lightOn; }
	void setLightOn(GLuint lightOnOROff) { lightOn = lightOnOROff; }
	//int getKeycardCount() { return keycards;  }
	//void decrementKeycardCount() { keycards--; }

	CPM_GLM_AABB_NS::AABB getBoundingBox() { return aabb; }
	// Accesors and mutators.
	void draw(stack<glm::mat4> mvStack, GLuint shaderProgram, mat4 projectionMatrix);
	void draw(stack<glm::mat4> mvStack, GLuint shaderProgram, mat4 projectionMatrix, bool twoTextures, rt3d::lightStruct light);
	// Boxes without rotation
	void draw(stack<glm::mat4> mvStack, GLuint shaderProgram, mat4 projectionMatrix, GLfloat rotation, rt3d::lightStruct light);// Used for drawing the keycards
	void draw(stack<glm::mat4> mvStack, GLuint shaderProgram, mat4 projectionMatrix, bool twoTextures, GLfloat rotation, rt3d::lightStruct light);
	void draw(stack<glm::mat4> mvStack, GLuint shaderProgram, mat4 projectionMatrix, bool twoTextures, int textureVisible, int specularValue, GLfloat rotation, rt3d::lightStruct light);
	//Boxes with rotation


	void drawWithTwoLights(stack<glm::mat4> mvStack, GLuint shaderProgram, mat4 projectionMatrix, GLfloat rotation,rt3d::lightStruct light0, rt3d::lightStruct light1, GLuint lightOn);

	void drawWithTwoTexturesAndTwoLights(stack<glm::mat4> passedStack, GLuint shaderProgram, mat4 projectionMatrix, bool twoTextures, int textureVisible, int specularValue, GLfloat rotation, rt3d::lightStruct light0, rt3d::lightStruct light1, int lightOn);

	void drawWithVariableTextures(stack<glm::mat4> passedStack, GLuint shaderProgram, mat4 projectionMatrix, int texturesCount, int textureVisible, int specularValue, GLfloat rotation, rt3d::lightStruct light0, rt3d::lightStruct light1, vec3 eye, mat4 view);
		// Updates position and rotation

	void update(vec3 objectPosition, GLfloat rotation);

};