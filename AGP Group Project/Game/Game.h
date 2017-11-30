#pragma once

#include <GL/glew.h>
#include "./Utilities/AABB.hpp"
#include "Camera.h"
#include "HUD.h"
#include "Lights.h"
#include "Skybox.h"
#include "SceneObjects.h"
#include "../FBO.h"
#include "../Game/Utilities/Utilities.h"
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
	//Lights *lights[2];
	Skybox *skybox;

	SceneObjects *boxes[20];
	//SceneObjects *box1;
	//SceneObjects *box2;

	SceneObjects *lightBox;
	SceneObjects *lightBox2;
	ParticleArray *particles;
	Utilities *util;
	FBO shadowFBO;
	// All of the objects from other classes delcared for use in the scene.

	stack<glm::mat4> mvStack;
	// Stack introduced.
	glm::mat4 modelview;

	int sceneScale;
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
	void InterSectionReaction(CPM_GLM_AABB_NS::AABB::INTERSECTION_TYPE intersectionType);
	enum MovementState { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN, NOMOVEMENT } moveState; // Enumerator used for checking the last direction the camera moved in
	// init, render and update functions for the Game class, the Game class itself
	// Is essentially a facade to interface with the main.
	
};