#pragma once
#include "./Utilities/rt3d.h"
#include "../Game/Utilities/Utilities.h"
#include <ctime>
#include <SDL_ttf.h>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>

using glm::distance;
using glm::vec3;

using namespace std;

class ParticleArray {
private:
	unsigned int ui_prevticks;
	int numParticles;
	GLfloat* positions;
	GLfloat* startPosition;
	GLfloat* colours;
	GLfloat* vel;
	GLfloat dt;
	GLfloat accel; // If we want to change it it can be a pointer
	GLuint vao[1];
	GLuint vbo[2];
	Utilities util;
	GLuint texture;
public:
	ParticleArray(const int n, char * textureName);
	ParticleArray(vec3 position, const int n, char * textureName);
	~ParticleArray();
	int getNumParticles(void) const { return numParticles; }
	GLfloat* getPositions(void) const { return positions; }
	GLfloat* getColours(void) const { return colours; }
	GLfloat* getVel(void) const { return vel; }
	void update(void);
	void draw(stack<glm::mat4> passedStack, GLuint shaderProgram, mat4 projectionMatrix);
	void createBuffers();
};

