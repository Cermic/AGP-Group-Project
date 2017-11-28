#include "Game.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Game::Game()
{
	// Empty Constructor
}

//Destructor to clean up the objects - Game is taking responsibility for scene object destruction
Game::~Game()
{
	delete[] camera;
	delete[] hud;
	delete[] keys;
	delete[] nameWaterMark;
	delete[] light0;
	delete[] light1;
	delete[] skybox;
	delete[] boxes[0];
	delete[] boxes[1];
	delete[] lightBox;
	delete[] lightBox2;
	delete[] util;
}

// Initialises and loads all objects in the scene
void Game::initialise()
{
	// Utitlities loaded
	util = new Utilities();
	util->loadUtilities();
	// Camera initialised
	camera = new Camera(vec3(-6.5f, 5.0f, 3.0f), vec3(0.0f, 1.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f);
	// HUD initialised
	hud = new HUD();
	hud->load();
	keys = new HUD();
	keys->load();
	nameWaterMark = new HUD();
	nameWaterMark->load();
	// Lights initialised
	light0 = new Lights(vec4(-6.0f, 5.5f, 0.5f, 1.0f), 1.0f);
	light1 = new Lights(vec4(-9.0f, 5.5f, 0.5f, 1.0f), 1.0f);
	//Skybox initialised and loaded.
	skybox = new Skybox();
	skybox->load();

	//initialise openGL to start depth testing and blend
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Sceneobjects loaded
	boxes[0] = new SceneObjects(vec3(-5.0f, 5.0f, -2.5f), vec3(1.0f, 1.0f, 1.0f), 1.0f, "Assets/Textures/MetalBox.bmp", "Assets/Textures/MetalBoxLightMapAdvanced.bmp");
	boxes[1] = new SceneObjects(vec3(-8.0f, 5.0f, -2.5f), vec3(1.0f, 1.0f, 1.0f), 1.0f, "Assets/Textures/MetalBox.bmp");

	lightBox = new SceneObjects(light0->getLightPos(), vec3(0.25f, 0.25f, 0.25f), "Assets/Textures/MetalBox.bmp");
	lightBox2 = new SceneObjects(light1->getLightPos(), vec3(0.25f, 0.25f, 0.25f), "Assets/Textures/MetalBox.bmp");
}

// Draws all of the scene objects
void Game::render(SDL_Window * window)
{
	glEnable(GL_CULL_FACE);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection(1.0);// Sets up a fresh projection at the start of the scene
	rt3d::setUniformMatrix4fv(util->getLightMapProgram(), "projection", glm::value_ptr(util->getProjection()));

	GLfloat scale(1.0f); // just to allow easy scaling of complete scene
	glm::mat4 modelview(1.0); // set base position for scene
	mvStack.push(modelview); // modelview is pushed onto the stack to begin

	// Camera set
	camera->setAt(util->moveForward(camera->getEyePos(), camera->getRotation(), 1.0f));
	// lookat is updated
	mvStack.top() = mvStack.top() = glm::lookAt(camera->getEyePos(), camera->getAt(), camera->getUp());
	// Base scene is now setup
	// Skybox drawn
	skybox->draw(mvStack, util->getCubeMapProgram(), util->getProjection());
	// The scene objects decide which shader and light they will use in the scene so it does not matter where the light
	// Is declared in the render portion of the program.
	// Draws the scene lights that apply to all objects with Phong shader as default for the lightmapped box
	light0->draw(mvStack, util->getPhongTextureProgram(), util->getProjection(), light0->getAttenuationConstant()); // Right hand side light
	light1->draw(mvStack, util->getPhongTextureProgram(), util->getProjection(), light1->getAttenuationConstant()); // Left hand side light
	// Draws a box which is loaded with lightmapping, two textures can be affected by rotation, attenuation and specular shininess changes.
	// boxes[0] is the box on the RIGHT
	boxes[0]->drawWithTwoTexturesAndTwoLights(mvStack, util->getLightMap2LProgram(), util->getProjection(), true, boxes[0]->getTextureVisible(), boxes[0]->getSpecularValue(), boxes[0]->getRotation() + 0.1f, light0->getLight(), light1->getLight()); // This is the lightmapped box
	// Draws a box which is loaded with regular phong lighting, one texture and can be affected by rotation and attenuation changes.																								
	// boxes[1] is the box on the LEFT
	//boxes[1]->draw(mvStack, util->getPhongTextureProgram(), util->getProjection(), boxes[1]->getRotation() + 0.1f, light1->getLight());
	boxes[1]->drawWithTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), boxes[1]->getRotation() + 0.1f, light0->getLight(), light1->getLight());
	// Draws a box around the light that simply follows it.
	lightBox->draw(mvStack, util->getPhongTextureProgram(), util->getProjection(), 1.0f, light0->getLight()); // Small box on the right
	lightBox2->draw(mvStack, util->getPhongTextureProgram(), util->getProjection(), 1.0f, light0->getLight()); // Small box on the left
	// HUD drawn last so it renders on top.
	hud->draw(mvStack, camera->getEyePos(), util->getShaderProgram());
	//keys->draw(mvStack, util->getShaderProgram());
	//nameWaterMark->draw(mvStack, util->getShaderProgram(), 9, vec3(0.8, 0.9, 0.0), vec3(0.2, 0.1, 0.0), { 255,255,255 });
	mvStack.pop();
	glDepthMask(GL_TRUE);
	SDL_GL_SwapWindow(window); // swap buffers
}

void Game::InterSectionReaction(CPM_GLM_AABB_NS::AABB::INTERSECTION_TYPE intersectionType)
{
	if (intersectionType == CPM_GLM_AABB_NS::AABB::INSIDE || intersectionType == CPM_GLM_AABB_NS::AABB::INTERSECT)
	{
		if (moveState == FORWARD)
		{
			camera->setEyePos(util->moveForward(camera->getEyePos(), camera->getRotation(), -0.1f));
			moveState = NOMOVEMENT;
		}
		if (moveState == BACKWARD)
		{
			camera->setEyePos(util->moveForward(camera->getEyePos(), camera->getRotation(), +0.1f));
			moveState = NOMOVEMENT;
		}
		if (moveState == LEFT)
		{
			camera->setEyePos(util->moveRight(camera->getEyePos(), camera->getRotation(), 0.1f));
			moveState = NOMOVEMENT;
		}
		if (moveState == RIGHT)
		{
			camera->setEyePos(util->moveRight(camera->getEyePos(), camera->getRotation(), -0.1f));
			moveState = NOMOVEMENT;
		}
		if (moveState == UP)
		{
			camera->setEyePos(camera->getEyePos() + vec3(0.0f, -0.1f, 0.0f));
			moveState = NOMOVEMENT;
		}
		if (moveState == DOWN)
		{
			camera->setEyePos(camera->getEyePos() + vec3(0.0f, 0.1f, 0.0f));
			moveState = NOMOVEMENT;
		}
		cout << "Intersection" << endl;
	}
	if (intersectionType == CPM_GLM_AABB_NS::AABB::OUTSIDE)
	{
		moveState == NOMOVEMENT;
		cout << "No intersection" << endl;
	}
}

// Updates all of the scene objects
void Game::update()
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	// Updates light attenuation.
	light0->update(light0->getAttenuationConstant());
	light1->update(light1->getAttenuationConstant());
	// Updates lightbox position based on the light
	lightBox->update(light0->getLightPos(), 1.0f);
	lightBox2->update(light1->getLightPos(), 1.0f);

	// boxes position and rotation updated.
	if (keys[SDL_SCANCODE_3])
	{
		boxes[0]->setRotation(boxes[0]->getRotation() - 1.0f);
		boxes[1]->setRotation(boxes[1]->getRotation() + 1.0f);
	}
	// boxes position and rotation updated inversly.
	if (keys[SDL_SCANCODE_4])
	{
		boxes[0]->setRotation(boxes[0]->getRotation() + 1.0f);
		boxes[1]->setRotation(boxes[1]->getRotation() - 1.0f);
	}
	// Loads MetalBox texture pair
	if (keys[SDL_SCANCODE_5])
	{
		boxes[0]->setTextures("Assets/Textures/MetalBox.bmp", "Assets/Textures/MetalBoxLightMapAdvanced.bmp");
	}
	// Loads MetalBox2 texture pair
	if (keys[SDL_SCANCODE_6])
	{
		boxes[0]->setTextures("Assets/Textures/MetalBox2.bmp", "Assets/Textures/MetalBox2LightMap.bmp");
	}
	// Shows the full lightmapping effect on the box
	if (keys[SDL_SCANCODE_7])
	{
		boxes[0]->setTextureVisible(0);
	}
	// Shows only the box's lightmap texture
	if (keys[SDL_SCANCODE_8])
	{
		boxes[0]->setTextureVisible(1);
	}

	/////////////////////////////////////////////////////////
	// Camera movement and collision detection / Reaction ///
	/////////////////////////////////////////////////////////

	// Moves camera forward
	if (keys[SDL_SCANCODE_W])
	{
		moveState = FORWARD;
		camera->setEyePos(util->moveForward(camera->getEyePos(), camera->getRotation(), 0.1f));
		camera->setBoundingBox(CPM_GLM_AABB_NS::AABB(camera->getEyePos(), camera->getBoundingRadius()));
		//Updates the camera and the boundingBox
	}
	// Moves camera backward
	if (keys[SDL_SCANCODE_S])
	{
		moveState = BACKWARD;
		camera->setEyePos(util->moveForward(camera->getEyePos(), camera->getRotation(), -0.1f));
		camera->setBoundingBox(CPM_GLM_AABB_NS::AABB(camera->getEyePos(), camera->getBoundingRadius()));
		//Updates the camera and the boundingBox
	}
	// Moves camera left
	if (keys[SDL_SCANCODE_A])
	{
		moveState = LEFT;
		camera->setEyePos(util->moveRight(camera->getEyePos(), camera->getRotation(), -0.1f));
		camera->setBoundingBox(CPM_GLM_AABB_NS::AABB(camera->getEyePos(), camera->getBoundingRadius()));
		//Updates the camera and the boundingBox
	}
	// Moves camera right
	if (keys[SDL_SCANCODE_D])
	{
		moveState = RIGHT;
		camera->setEyePos(util->moveRight(camera->getEyePos(), camera->getRotation(), 0.1f));
		camera->setBoundingBox(CPM_GLM_AABB_NS::AABB(camera->getEyePos(), camera->getBoundingRadius()));
		//Updates the camera and the boundingBox
	}
	// Moves camera up
	if (keys[SDL_SCANCODE_UP])
	{
		moveState = UP;
		camera->setEyePos(camera->getEyePos() + vec3(0.0f, 0.1f, 0.0f));
		camera->setBoundingBox(CPM_GLM_AABB_NS::AABB(camera->getEyePos(), camera->getBoundingRadius()));
		//Updates the camera and the boundingBox
	}
	// Moves camera down
	if (keys[SDL_SCANCODE_DOWN])
	{
		moveState = DOWN;
		camera->setEyePos(camera->getEyePos() - vec3(0.0f, 0.1f, 0.0f));
		camera->setBoundingBox(CPM_GLM_AABB_NS::AABB(camera->getEyePos(), camera->getBoundingRadius()));
		//Updates the camera and the boundingBox
	}
	// Rotates camera left
	if (keys[SDL_SCANCODE_LEFT])
	{
		camera->setRotation(camera->getRotation() - 1.0f);
		camera->setBoundingBox(CPM_GLM_AABB_NS::AABB(camera->getEyePos(), camera->getBoundingRadius()));
		//Updates the camera and the boundingBox
	}
	// Rotates right
	if (keys[SDL_SCANCODE_RIGHT])
	{
		camera->setRotation(camera->getRotation() + 1.0f);
		camera->setBoundingBox(CPM_GLM_AABB_NS::AABB(camera->getEyePos(), camera->getBoundingRadius()));
		//Updates the camera and the boundingBox
	}
	
	for (int i = 0; i < 2; i++) // Keep this number set to the amount of objects that have been constructed, not the array size.
	{
		InterSectionReaction(camera->getBoundingBox().intersect(boxes[i]->getBoundingBox()));
	}
	// Collision detection check.
	/////////////////////////////////////////////////////////
	// Camera movement and collision detection / Reaction ///
	/////////////////////////////////////////////////////////

	// Resets camera
	if (keys[SDL_SCANCODE_R])
	{
		camera->setEyePos(vec3(-6.5f, 5.0f, 3.0f));
		//camera->setBoundingBoxPos(camera->getEyePos());
		camera->setBoundingBox(CPM_GLM_AABB_NS::AABB(camera->getEyePos(), camera->getBoundingRadius()));
		//Updates the camera and the boundingBox
	}
	// Camera movement checks

	// Moves light backward
	if (keys[SDL_SCANCODE_LCTRL])
	{
		if (keys[SDL_SCANCODE_KP_3])
		{
			light0->setLightPos(light0->getLightPos() + vec4(0.0f, 0.0f, 0.1f, 0.0f));
		}
		if (keys[SDL_SCANCODE_KP_9])
		{
			light0->setLightPos(light0->getLightPos() - vec4(0.0f, 0.0f, 0.1f, 0.0f));
		}
		if (keys[SDL_SCANCODE_KP_8])
		{
			light0->setLightPos(light0->getLightPos() + vec4(0.0f, 0.1f, 0.0f, 0.0f));
		}
		if (keys[SDL_SCANCODE_KP_2])
		{
			light0->setLightPos(light0->getLightPos() - vec4(0.0f, 0.1f, 0.0f, 0.0f));
		}
		if (keys[SDL_SCANCODE_KP_4])
		{
			light0->setLightPos(light0->getLightPos() - vec4(0.1f, 0.0f, 0.0f, 0.0f));
		}
		if (keys[SDL_SCANCODE_KP_6])
		{
			light0->setLightPos(light0->getLightPos() + vec4(0.1f, 0.0f, 0.0f, 0.0f));
		}
	}
	else
	{

		if (keys[SDL_SCANCODE_KP_3])
		{
			light1->setLightPos(light1->getLightPos() + vec4(0.0f, 0.0f, 0.1f, 0.0f));
		}
		if (keys[SDL_SCANCODE_KP_9])
		{
			light1->setLightPos(light1->getLightPos() - vec4(0.0f, 0.0f, 0.1f, 0.0f));
		}
		if (keys[SDL_SCANCODE_KP_8])
		{
			light1->setLightPos(light1->getLightPos() + vec4(0.0f, 0.1f, 0.0f, 0.0f));
		}
		if (keys[SDL_SCANCODE_KP_2])
		{
			light1->setLightPos(light1->getLightPos() - vec4(0.0f, 0.1f, 0.0f, 0.0f));
		}
		if (keys[SDL_SCANCODE_KP_4])
		{
			light1->setLightPos(light1->getLightPos() - vec4(0.1f, 0.0f, 0.0f, 0.0f));
		}
		if (keys[SDL_SCANCODE_KP_6])
		{
			light1->setLightPos(light1->getLightPos() + vec4(0.1f, 0.0f, 0.0f, 0.0f));
		}

	}
	// Draws scene in poly only mode - no textures
	if (keys[SDL_SCANCODE_1]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
	}
	// Draws scene normally
	if (keys[SDL_SCANCODE_2]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);
	}

	// Allows specular changes
	if (keys[SDL_SCANCODE_LSHIFT])
	{
		// Increases specular shininess
		if (keys[SDL_SCANCODE_KP_PLUS]) boxes[0]->setSpecularValue(boxes[0]->getSpecularValue() + 1);
		// Decreases specular shininess
		if (keys[SDL_SCANCODE_KP_MINUS]) boxes[0]->setSpecularValue(boxes[0]->getSpecularValue() - 1);
	}
	else
		// Allows attenuation changes
	{
		//Attenutation User Control:
		if (light0->getAttenuationConstant() < 1.0f)
		{
			if (keys[SDL_SCANCODE_KP_PLUS]) light0->setAttenuationConstant(light0->getAttenuationConstant() + 0.1f);
			// Increases attenuation - thus decreasing overall light through restricting it from traveling further unattenuated.
		}
		if (light0->getAttenuationConstant() > 0.0f)
		{
			if (keys[SDL_SCANCODE_KP_MINUS]) light0->setAttenuationConstant(light0->getAttenuationConstant() - 0.1f);
			// Decreases attenuation - thus increasing overall light through allowing it to travel further unattenuated.

		}
		if (light1->getAttenuationConstant() < 1.0f)
		{
			if (keys[SDL_SCANCODE_KP_PLUS]) light1->setAttenuationConstant(light1->getAttenuationConstant() + 0.1f);
			// Increases attenuation - thus decreasing overall light through restricting it from traveling further unattenuated.
		}
		if (light1->getAttenuationConstant() > 0.0f)
		{
			if (keys[SDL_SCANCODE_KP_MINUS]) light1->setAttenuationConstant(light1->getAttenuationConstant() - 0.1f);
			// Decreases attenuation - thus increasing overall light through allowing it to travel further unattenuated.

		}
	}

}


