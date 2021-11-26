#pragma once
#include "ParticleArray.h"


using namespace std;

// Constructs a particle emitter that takes a defined amount of particles and a texturename
ParticleArray::ParticleArray(const int n, char * textureName)
{
	numParticles = n;
	texture = util.loadBitmap(textureName);
	ui_prevticks = 0;

	if (numParticles <= 0) // trap invalid input
		return;

	startPosition = new GLfloat[numParticles * 3];
	positions = new GLfloat[numParticles * 3];
	colours = new GLfloat[numParticles * 3];
	vel = new GLfloat[numParticles * 3];
	accel = 1.35f;
	// lets initialise with some lovely random values!
	std::srand(std::time(0));
	for (int i = 0; i < numParticles * 3; i++)
	{
		positions[i] = 0.0f; // 3 position components per particle
		startPosition[i] = 0.0f;
		colours[i] = (std::rand() % 100) / 50.0f;
		vel[i] = (std::rand() % 20 - 10) / 10.0f;
		//  Arrays initilised
	}
	createBuffers();
	// Initialise VAO and VBO in constructor, after initialising
	// the arrays:
}

// Constructs a particle emitter that takes a position, a defined amount of particles and a texturename
ParticleArray::ParticleArray(vec3 position, const int n, char * textureName)
{
	texture = util.loadBitmap(textureName);
	numParticles = n;
	ui_prevticks = 0;

	if (numParticles <= 0) // trap invalid input
		return;

	startPosition = new GLfloat[numParticles * 3];
	positions = new GLfloat[numParticles * 3];
	colours = new GLfloat[numParticles * 3];
	vel = new GLfloat[numParticles * 3];
	accel = 1.35f;
	// lets initialise with some lovely random values!
	std::srand(std::time(0));
	for (int i = 0; i < numParticles * 3; i++)
	{
		colours[i] = (std::rand() % 100) / 50.0f;
		vel[i] = (std::rand() % 20 - 10) / 10.0f;
		//positions[i] = 0.0f;
		//startPosition[i] = 0.0f;

		//  Arrays initilised
	}

	for (int i = 0; i < numParticles * 3; i++)
	{

		positions[i] = position.x; // 3 position components per particle
		startPosition[i] = position.x;
		i++;
		positions[i] = position.y;
		startPosition[i] = position.y;
		i++;
		positions[i] = position.z;
		startPosition[i] = position.z;
		i++;
	}
	createBuffers();
	// Initialise VAO and VBO in constructor, after initialising
	// the arrays:
}


//If you are using the RT3D library, this is handled inside the drawMesh() function, with a call to glBindVertexArray. 

ParticleArray::~ParticleArray()
{
	delete[] positions;
	delete[] startPosition;
	delete[] colours;
	delete[] vel;
}

void ParticleArray::createBuffers()
{
	glGenVertexArrays(1, vao);
	glGenBuffers(2, vbo);
	glBindVertexArray(vao[0]); // bind VAO 0 as current object
							   // Position data in attribute index 0, 3 floats per vertex
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // bind VBO for positions
	glBufferData(GL_ARRAY_BUFFER, numParticles * 3 *
		sizeof(GLfloat), positions, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);     // Enable attribute index 0

									  // Colours data in attribute 1, 3 floats per vertex
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // bind VBO for colours
	glBufferData(GL_ARRAY_BUFFER, numParticles * 3 *
		sizeof(GLfloat), colours, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);    // Enable attribute index 1
	glBindVertexArray(0);
}

void ParticleArray::update(void)
{
	unsigned int ui_currentticks = SDL_GetTicks();//
	dt = ui_currentticks - ui_prevticks;
	dt /= 1000.0;
	ui_prevticks = ui_currentticks;

	for (int i = 0; i < numParticles * 3; i++)
	{
		positions[i] = positions[i] + ((double)dt * (vel[i])); // x
		if (glm::distance(startPosition[i], positions[i]) >(std::rand() % 50)) // x
		{
			positions[i] = startPosition[i];
		}
		i++;
		positions[i] = positions[i] + ((double)dt * glm::abs((vel[i])));// y
		if (vel[i] > 0)
		{
			vel[i] -= 1.0f; // decrements the Y component of the particle to make it fall back towards the ground
		}
		if (glm::distance(startPosition[i], positions[i]) > (std::rand() % 100)) // y
		{
			positions[i] = startPosition[i];
		}
		i++;
		positions[i] = positions[i] + ((double)dt * (vel[i]));// z
		if (glm::distance(startPosition[i], positions[i]) > (std::rand() % 50)) // z
		{
			positions[i] = startPosition[i];
		}

		// Resets the position of the particle when it gets a certain distance away from the starting position

	}
}

void ParticleArray::draw(stack<glm::mat4> passedStack, GLuint shaderProgram, mat4 projectionMatrix)
{
	// Sets the object into the stack
	stack<glm::mat4> mvStack = passedStack;
	glUseProgram(shaderProgram);
	glBindTexture(GL_TEXTURE_2D, texture);
	mvStack.push(mvStack.top());
	// pass modelview & projection as uniforms into shader
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(projectionMatrix));
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDepthMask(0); 
	//stop updates to the depth mask when rendering the particles - 
	//This stops them drawing on top of each other.
	glPointSize(25.0);
	// bind the current VAO before drawing each object.
	glBindVertexArray(vao[0]); // bind VAO 0 as current object
							   // particle data may have been updated - so need to resend to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // bind VBO 0
	glBufferData(GL_ARRAY_BUFFER, numParticles * 3 *
		sizeof(GLfloat), positions, GL_DYNAMIC_DRAW);
	// Position data in attribute index 0, 3 floats per vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);     // Enable attribute index 0
									  // Now draw the particles... as easy as this!
	glDrawArrays(GL_POINTS, 0, numParticles);
	glBindVertexArray(0);
	glDisable(GL_BLEND);
	glDepthMask(1);
	mvStack.pop();
}
