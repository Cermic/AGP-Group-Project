#pragma once

#include <GL/glew.h>
#include "AABB.hpp"
#include "Camera.h"
#include "HUD.h"
#include "Lights.h"
#include "Skybox.h"
#include "SceneObjects.h"
#include "FBO.h"
#include "Utilities.h"
#include "ParticleArray.h"
// Game needs access to any class that will be instantiated by it.

class Game
{
private:
	Camera *camera;
	HUD *hud;
	HUD *keys;
	HUD *nameWaterMark;
	Lights *light0;
	Lights *light1;
	Skybox *skybox;

	SceneObjects *boxes[30];
	SceneObjects *keycards[4];
	//SceneObjects *box1;
	//SceneObjects *box2;

	SceneObjects *lightBox;
	SceneObjects *lightBox2;
	ParticleArray *particles1, *particles2, *particles3, *particles4, *particles5;
	Utilities *util;
	FBO shadowFBO;
	// All of the objects from other classes delcared for use in the scene.

	stack<glm::mat4> mvStack;
	// Stack introduced.
	glm::mat4 modelview;

	int sceneScale;
	int keycardCount = 4;
	// Scene scale declared.

public:
	Game();
	// Game Constructor
	~Game();
	// Game Destructor
	void initialise();
	void render(SDL_Window * window);
	void renderWithShadows(SDL_Window * window); // For incomplete implementation of shadow mapping
	void update();
	void InterSectionReaction(CPM_GLM_AABB_NS::AABB::INTERSECTION_TYPE intersectionType);// Defines a reaction for the camera when it collides with a sceneobject
	void KeyCardCollected(CPM_GLM_AABB_NS::AABB::INTERSECTION_TYPE intersectionType, int keycardNumber); // Defines a reaction when a keycard object collides with the player
	enum MovementState { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN, NOMOVEMENT } moveState; // Enumerator used for checking the last direction the camera moved in
	int getKeycardCount() { return keycardCount; }
	void decrementKeycardCount() { keycardCount--; }
	// init, render and update functions for the Game class, the Game class itself
	// Is essentially a facade to interface with the main.
	
};