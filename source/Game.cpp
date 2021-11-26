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

	for (int i = 0; i < 25; i++)
	{
		delete[] boxes[i];
	}

	for (int i = 0; i < 4; i++)
	{
		delete[] keycards[i];
	}

	delete[] lightBox;
	delete[] lightBox2;
	delete[] util;

	delete[] particles1;
	delete[] particles2;
	delete[] particles3;
	delete[] particles4;
	delete[] particles5;

}

// Initialises and loads all objects in the scene
void Game::initialise()
{
	// Utitlities loaded
	util = new Utilities();
	util->loadUtilities();
	// Camera initialised
	camera = new Camera(vec3(0.0f, 4.0f, 50.0f), vec3(0.0f, 1.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), 1.0f, 1.5f);
	// HUD initialised
	hud = new HUD();
	hud->load();
	keys = new HUD();
	keys->load();
	nameWaterMark = new HUD();
	nameWaterMark->load();
	// Lights initialised
	light0 = new Lights(vec4(2.0f, 3.0f, 40.0f, 1.0f), 1.0f);
	light1 = new Lights(vec4(-2.0f, 3.0f, 40.0f, 1.0f), 1.0f, 10.0f);
	//Skybox initialised and loaded.
	skybox = new Skybox();
	skybox->load();

	//Initialise FBO for shadows
	shadowFBO.generateShadowFBO();

	particles1 = new ParticleArray(vec3( 15.0f, 8.0f, 0.0f), 300,"../assets/materials/fire.bmp"); // Particles constructed
	particles2 = new ParticleArray(vec3(-15.0f, 8.0f, 0.0f), 300, "../assets/materials/fire.bmp"); // Particles constructed
	particles3 = new ParticleArray(vec3(  0.0f, 8.0f, -15.0f), 300, "../assets/materials/fire.bmp"); // Particles constructed
	particles4 = new ParticleArray(vec3(  19.0f, 8.0f, 30.0f), 300, "../assets/materials/fire.bmp"); // Particles constructed
	particles5 = new ParticleArray(vec3( -19.0f, 8.0f, 30.0f), 300, "../assets/materials/fire.bmp"); // Particles constructed

	//initialise openGL to start depth testing and blend
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Sceneobjects loaded
	boxes[0] = new SceneObjects(vec3(-3.0f, 5.0f, 35.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f, "../assets/materials/MetalBox.bmp", "../assets/materials/MetalBoxLightMapAdvanced.bmp", "../assets/materials/metalbox-normal.bmp");
	boxes[1] = new SceneObjects(vec3( 3.0f, 5.0f, 35.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f, "../assets/materials/MetalBox.bmp", "../assets/materials/MetalBoxLightMapAdvanced.bmp");
	//floor
	boxes[2] = new SceneObjects(vec3(0.0f, 0.0f, 0.0f), vec3(40.0f, 1.0f, 60.0f), 0.0f, "../assets/materials/bricktexture.bmp");
	//ceiling
	boxes[3] = new SceneObjects(vec3(0.0f, 22.0f, 0.0f), vec3(40.0f, 1.0f, 60.0f), 0.0f, "../assets/materials/bricktexture.bmp");
	//first room
	boxes[4] = new SceneObjects(vec3(  0.0f, 11.0f, 59.0f), vec3(20.0f, 10.0f,  1.0f), 0.0f, "../assets/materials/bricktexture.bmp", "../assets/materials/bricknormal.bmp");
	boxes[5] = new SceneObjects(vec3(-21.0f, 11.0f, 38.0f), vec3( 1.0f, 10.0f, 20.0f), 0.0f, "../assets/materials/bricktexture.bmp", "../assets/materials/bricknormal.bmp");
	boxes[6] = new SceneObjects(vec3( 21.0f, 11.0f, 38.0f), vec3( 1.0f, 10.0f, 20.0f), 180.0f, "../assets/materials/bricktexture.bmp", "../assets/materials/bricknormal.bmp");
	boxes[7] = new SceneObjects(vec3(-22.0f, 11.0f, 17.0f), vec3(16.0f, 10.0f,  1.0f), 0.0f, "../assets/materials/bricktexture.bmp", "../assets/materials/bricknormal.bmp");
	boxes[8] = new SceneObjects(vec3( 22.0f, 11.0f, 17.0f), vec3(16.0f, 10.0f,  1.0f), 0.0f, "../assets/materials/bricktexture.bmp", "../assets/materials/bricknormal.bmp");
	boxes[9] = new SceneObjects(vec3(  0.0f, 15.0f, 17.0f), vec3( 6.0f,  6.0f,  1.0f), 0.0f, "../assets/materials/bricktexture.bmp", "../assets/materials/bricknormal.bmp");
	//second room
	boxes[10] = new SceneObjects(vec3(-39.0f, 11.0f,   0.0f), vec3( 1.0f, 10.0f, 16.0f), 0.0f, "../assets/materials/bricktexture.bmp", "../assets/materials/bricknormal.bmp");
	boxes[11] = new SceneObjects(vec3( 39.0f, 11.0f,   0.0f), vec3( 1.0f, 10.0f, 16.0f), 180.0f, "../assets/materials/bricktexture.bmp", "../assets/materials/bricknormal.bmp");
	boxes[12] = new SceneObjects(vec3(-20.0f, 11.0f, -17.0f), vec3(20.0f, 10.0f,  1.0f), 0.0f, "../assets/materials/bricktexture.bmp", "../assets/materials/bricknormal.bmp");
	boxes[13] = new SceneObjects(vec3( 20.0f, 11.0f, -17.0f), vec3(20.0f, 10.0f,  1.0f), 0.0f, "../assets/materials/bricktexture.bmp","../assets/materials/bricknormal.bmp");

	boxes[14] = new SceneObjects(vec3(-20.0f, 11.0f,   0.0f), vec3( 4.0f, 10.0f,  4.0f), 0.0f, "../assets/materials/pillar.bmp");
	boxes[15] = new SceneObjects(vec3( 20.0f, 11.0f,   0.0f), vec3( 4.0f, 10.0f,  4.0f), 180.0f, "../assets/materials/pillar.bmp");
	//door
	boxes[16] = new SceneObjects(vec3( 0.0f,  5.0f,   17.0f), vec3( 6.0f, 4.0f,   0.9f), 1.0f, "../assets/materials/door.bmp");
	//keycards
	keycards[0] = new SceneObjects(vec3(0.0f, 4.0f, 25.0f), vec3(0.01f, 0.2f, 0.4f), 1.0f, "../assets/materials/keycard.bmp");
	keycards[1] = new SceneObjects(vec3(-27.0f, 4.0f, 0.0f), vec3(0.01f, 0.2f, 0.4f), 1.0f, "../assets/materials/keycard.bmp");
	keycards[2] = new SceneObjects(vec3( 27.0f, 4.0f, 0.0f), vec3(0.01f, 0.2f, 0.4f), 1.0f, "../assets/materials/keycard.bmp");
	keycards[3] = new SceneObjects(vec3(-3.0f, 4.0f, -2.5f), vec3(0.01f, 0.2f, 0.4f), 1.0f, "../assets/materials/keycard.bmp");
	//brazier
	boxes[17] = new SceneObjects(vec3(15.0f, 6.8f, 0.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f, "../assets/materials/brazier.bmp");
	boxes[18] = new SceneObjects(vec3(-15.0f, 6.8f, 0.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f, "../assets/materials/brazier.bmp");
	boxes[19] = new SceneObjects(vec3(0.0f, 6.8f, -15.0f), vec3(1.0f, 1.0f, 1.0f),  1.0f, "../assets/materials/brazier.bmp");
	boxes[20] = new SceneObjects(vec3(19.0f, 6.8f, 30.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f, "../assets/materials/brazier.bmp");
	boxes[21] = new SceneObjects(vec3(-19.0f, 6.8f, 30.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f, "../assets/materials/brazier.bmp");

	//comparison boxes room 1
	//middle bottom
	boxes[22] = new SceneObjects(vec3(0.0f, 5.0f, 35.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f, "../assets/materials/bricktexture.bmp","../assets/materials/bricklightmap.bmp", "../assets/materials/bricknormal.bmp");
	//left top
	boxes[23] = new SceneObjects(vec3(-3.0f, 8.0f, 35.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f, "../assets/materials/bricktexture.bmp");
	//middle top
	boxes[24] = new SceneObjects(vec3(0.0f, 8.0f, 35.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f, "../assets/materials/bricktexture.bmp");
	boxes[25] = new SceneObjects(vec3(3.0f, 8.0f, 35.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f, "../assets/materials/bricktexture.bmp");

	lightBox = new SceneObjects(light0->getLightPos(), vec3(0.25f, 0.25f, 0.25f), "../assets/materials/MetalBox.bmp");
	lightBox2 = new SceneObjects(light1->getLightPos(), vec3(0.25f, 0.25f, 0.25f), "../assets/materials/door.bmp");
}


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
	mat4 view = glm::lookAt(camera->getEyePos(), camera->getAt(), camera->getUp());
	mvStack.top() = mvStack.top() = glm::lookAt(camera->getEyePos(), camera->getAt(), camera->getUp());
	
	// Skybox drawn
	skybox->draw(mvStack, util->getCubeMapProgram(), util->getProjection());
	// The scene objects decide which shader and light they will use in the scene so it does not matter where the light
	// Is declared in the render portion of the program.
	// Draws the scene lights that apply to all objects with Phong shader as default for the lightmapped box
	light0->draw(mvStack, util->getPhongTextureProgram(), util->getProjection(), light0->getAttenuationConstant()); // Right hand side light
	light1->draw(mvStack, util->getPhongTextureProgram(), util->getProjection(), light1->getAttenuationConstant()); // Left hand side light
	// Draws a box which is loaded with lightmapping, two textures can be affected by rotation, attenuation and specular shininess changes.
	// boxes[0] is the box on the RIGHT

	//draws a box with normal texture, normal mapping and light mapping																								
	boxes[0]->drawWithVariableTextures(mvStack, util->getCombinedTextureProgram(), util->getProjection(), 3, boxes[0]->getTextureVisible(), boxes[0]->getSpecularValue(), boxes[0]->getRotation(), light0->getLight(), light1->getLight(), camera->getEyePos(), view);
	//draws a box with only normal texture and normal mapping
	//draws a box with
	boxes[1]->drawWithTwoTexturesAndTwoLights(mvStack, util->getLightMap2LProgram(), util->getProjection(), true, boxes[1]->getTextureVisible(), boxes[1]->getSpecularValue(), boxes[1]->getRotation(), light0->getLight(), light1->getLight(), boxes[1]->getLightOn());

	boxes[2]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[2]->getTextureVisible(), boxes[2]->getSpecularValue(), boxes[2]->getRotation(), light0->getLight(), light1->getLight(), boxes[2]->getLightOn());
	boxes[3]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[3]->getTextureVisible(), boxes[3]->getSpecularValue(), boxes[3]->getRotation(), light0->getLight(), light1->getLight(), boxes[3]->getLightOn());
	boxes[4]->drawWithVariableTextures(mvStack, util->getCombinedTextureProgram(), util->getProjection(), 2, boxes[4]->getTextureVisible(), boxes[4]->getSpecularValue(), boxes[4]->getRotation(), light0->getLight(), light1->getLight(), camera->getEyePos(), view);
	boxes[5]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[5]->getTextureVisible(), boxes[5]->getSpecularValue(), boxes[5]->getRotation(), light0->getLight(), light1->getLight(), boxes[5]->getLightOn());
	boxes[6]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[6]->getTextureVisible(), boxes[6]->getSpecularValue(), boxes[6]->getRotation(), light0->getLight(), light1->getLight(), boxes[6]->getLightOn());
	boxes[7]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[7]->getTextureVisible(), boxes[7]->getSpecularValue(), boxes[7]->getRotation(), light0->getLight(), light1->getLight(), boxes[7]->getLightOn());
	boxes[8]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[8]->getTextureVisible(), boxes[8]->getSpecularValue(), boxes[8]->getRotation(), light0->getLight(), light1->getLight(), boxes[8]->getLightOn());
	boxes[9]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[9]->getTextureVisible(), boxes[9]->getSpecularValue(), boxes[9]->getRotation(), light0->getLight(), light1->getLight(), boxes[9]->getLightOn());
	boxes[10]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[10]->getTextureVisible(), boxes[10]->getSpecularValue(), boxes[10]->getRotation(), light0->getLight(), light1->getLight(), boxes[10]->getLightOn());
	boxes[11]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[11]->getTextureVisible(), boxes[11]->getSpecularValue(), boxes[11]->getRotation(), light0->getLight(), light1->getLight(), boxes[11]->getLightOn());
	boxes[12]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[12]->getTextureVisible(), boxes[12]->getSpecularValue(), boxes[12]->getRotation(), light0->getLight(), light1->getLight(), boxes[12]->getLightOn());
	boxes[13]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[13]->getTextureVisible(), boxes[13]->getSpecularValue(), boxes[13]->getRotation(), light0->getLight(), light1->getLight(), boxes[13]->getLightOn());
	boxes[14]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[14]->getTextureVisible(), boxes[14]->getSpecularValue(), boxes[14]->getRotation(), light0->getLight(), light1->getLight(), boxes[14]->getLightOn());
	boxes[15]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[15]->getTextureVisible(), boxes[15]->getSpecularValue(), boxes[15]->getRotation(), light0->getLight(), light1->getLight(), boxes[15]->getLightOn());
	boxes[16]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[16]->getTextureVisible(), boxes[16]->getSpecularValue(), boxes[16]->getRotation(), light0->getLight(), light1->getLight(), boxes[16]->getLightOn());
	boxes[17]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[17]->getTextureVisible(), boxes[17]->getSpecularValue(), boxes[17]->getRotation(), light0->getLight(), light1->getLight(), boxes[17]->getLightOn());
	boxes[18]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[18]->getTextureVisible(), boxes[18]->getSpecularValue(), boxes[18]->getRotation(), light0->getLight(), light1->getLight(), boxes[18]->getLightOn());
	boxes[19]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[19]->getTextureVisible(), boxes[19]->getSpecularValue(), boxes[19]->getRotation(), light0->getLight(), light1->getLight(), boxes[19]->getLightOn());
	boxes[20]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[20]->getTextureVisible(), boxes[20]->getSpecularValue(), boxes[20]->getRotation(), light0->getLight(), light1->getLight(), boxes[20]->getLightOn());
	boxes[21]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[21]->getTextureVisible(), boxes[21]->getSpecularValue(), boxes[21]->getRotation(), light0->getLight(), light1->getLight(), boxes[21]->getLightOn());
	boxes[22]->drawWithVariableTextures       (mvStack, util->getCombinedTextureProgram(), util->getProjection(), 3, boxes[22]->getTextureVisible(), boxes[22]->getSpecularValue(), boxes[22]->getRotation(), light0->getLight(), light1->getLight(), camera->getEyePos(), view);
	boxes[23]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[23]->getTextureVisible(), boxes[23]->getSpecularValue(), boxes[23]->getRotation(), light0->getLight(), light1->getLight(), boxes[23]->getLightOn());
	boxes[24]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[24]->getTextureVisible(), boxes[24]->getSpecularValue(), boxes[24]->getRotation(), light0->getLight(), light1->getLight(), boxes[24]->getLightOn());
	boxes[25]->drawWithTwoTexturesAndTwoLights(mvStack, util->getPhong2LTextureProgram(), util->getProjection(), true, boxes[24]->getTextureVisible(), boxes[24]->getSpecularValue(), boxes[24]->getRotation(), light0->getLight(), light1->getLight(), boxes[25]->getLightOn());

	for (int i = 0; i <= 3; i++)
	{
		keycards[i]->draw(mvStack, util->getPhongTextureProgram(), util->getProjection(), keycards[i]->getRotation(), light0->getLight());
	}

	// Draws a box around the light that simply follows it.
	lightBox->draw(mvStack, util->getPhongTextureProgram(), util->getProjection(), 1.0f, light0->getLight()); // Small box on the right
	lightBox2->draw(mvStack, util->getPhongTextureProgram(), util->getProjection(), 1.0f, light0->getLight()); // Small box on the left

	nameWaterMark->draw(mvStack, util->getShaderProgram(), 9, vec3(0.7, 0.9, 0.0), vec3(0.25, 0.1, 0.0), { 255,255,255 });
	// Particles drawn
	particles1->draw(mvStack, util->getParticleProgram(), util->getProjection());
	particles2->draw(mvStack, util->getParticleProgram(), util->getProjection());
	particles3->draw(mvStack, util->getParticleProgram(), util->getProjection());
	particles4->draw(mvStack, util->getParticleProgram(), util->getProjection());
	particles5->draw(mvStack, util->getParticleProgram(), util->getProjection());

	// HUD drawn last so it renders on top.
	if (keycardCount == 0)
	{
		hud->draw(mvStack, keycardCount, util->getShaderProgram(), " YOU WIN!!! EXIT NOW BYE :");
	}
	else
	{
		hud->draw(mvStack, keycardCount, util->getShaderProgram(), " Keycards remaining: ");
	}
	keys->draw(mvStack, util->getShaderProgram());

	mvStack.pop();
	glDepthMask(GL_TRUE);
	SDL_GL_SwapWindow(window); // swap buffers
}

// Stops the player from continuing to move in a direction if they were moving along it when they collide with an obstacle.
void Game::InterSectionReaction(CPM_GLM_AABB_NS::AABB::INTERSECTION_TYPE intersectionType)
{
	// Detects the passed in intersection type and reacts based on the movement state.
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

	}
	// When the camera bounding box is outside an obstacle bounding box movement in the previously limited direction is allowed again.
	if (intersectionType == CPM_GLM_AABB_NS::AABB::OUTSIDE)
	{
		moveState == NOMOVEMENT;
	}
}

void Game::KeyCardCollected(CPM_GLM_AABB_NS::AABB::INTERSECTION_TYPE intersectionType, int keycardNumber)
{
	// if an intersection is detected between a keycard and the cmaera the door is allowed to move
	// if doorMove is true it checks if it has reached a certain point - if it has it sets doorMove to false to stop the door moving. 
	// if it hasnt reached this point it increments upwards in the Y axis. 
	// keycardCount is decremented.
	if (intersectionType == CPM_GLM_AABB_NS::AABB::INSIDE || intersectionType == CPM_GLM_AABB_NS::AABB::INTERSECT)
	{
		boxes[16]->setDoorMovement(true);
		keycards[keycardNumber]->setIsDrawn(false);
		keycards[keycardNumber]->setPosition(vec3(100.0f, 100.0f, 100.0f));
		keycardCount--;
	}

	if (boxes[16]->getDoorMovement()==true)
	{
		if (boxes[16]->getPosition().y > 15.0f)
		{
			boxes[16]->setDoorMovement(false);
		}
		else
		{
			boxes[16]->setPosition(boxes[16]->getPosition() + vec3(0.0f, 0.01f, 0.0f));
		}
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

	for (int i = 0; i < 4; i++)
	{
		keycards[i]->setRotation(keycards[i]->getRotation() + 2.5f);
	}

	particles1->update();
	particles2->update();
	particles3->update();
	particles4->update();
	particles5->update();

	// Particles updated

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
		boxes[0]->setTextures("../assets/materials/MetalBox.bmp", "../assets/materials/MetalBoxLightMapAdvanced.bmp");
		boxes[1]->setTextures("../assets/materials/MetalBox.bmp", "../assets/materials/MetalBoxLightMapAdvanced.bmp");
	}
	// Loads MetalBox2 texture pair
	if (keys[SDL_SCANCODE_6])
	{
		boxes[0]->setTextures("../assets/materials/MetalBox2.bmp", "../assets/materials/MetalBox2LightMap.bmp");
		boxes[1]->setTextures("../assets/materials/MetalBox2.bmp", "../assets/materials/MetalBox2LightMap.bmp");
	}
	// Shows the full lightmapping effect on the box
	if (keys[SDL_SCANCODE_7])
	{
		boxes[0]->setTextureVisible(0);
		boxes[1]->setTextureVisible(0);
	}
	// Shows only the box's lightmap texture
	if (keys[SDL_SCANCODE_8])
	{
		boxes[0]->setTextureVisible(1);
		boxes[1]->setTextureVisible(1);
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
	//	camera->setEyePos(camera->getEyePos() + vec3(0.0f, 0.1f, 0.0f));
	//	camera->setBoundingBox(CPM_GLM_AABB_NS::AABB(camera->getEyePos(), camera->getBoundingRadius()));
		//Updates the camera and the boundingBox
	}
	// Moves camera down
	if (keys[SDL_SCANCODE_DOWN])
	{
		moveState = DOWN;
	//	camera->setEyePos(camera->getEyePos() - vec3(0.0f, 0.1f, 0.0f));
	//	camera->setBoundingBox(CPM_GLM_AABB_NS::AABB(camera->getEyePos(), camera->getBoundingRadius()));
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
	
	for (int i = 0; i <= 22; i++) // Keep this number set to the amount of objects that have been constructed, not the array size.
	{
		boxes[i]->update(boxes[i]->getPosition(), boxes[i]->getRotation());
		InterSectionReaction(camera->getBoundingBox().intersect(boxes[i]->getBoundingBox()));
	}
	for (int i = 0; i <= 3; i++) // Keep this number set to the amount of objects that have been constructed, not the array size.
	{
		KeyCardCollected(camera->getBoundingBox().intersect(keycards[i]->getBoundingBox()), i);
		keycards[i]->update(keycards[i]->getPosition(), keycards[i]->getRotation());
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
			light0->setConeDirection(light0->getConeDirection() + vec3(0.0f, 0.0f, 0.1f));
		}
		if (keys[SDL_SCANCODE_KP_9])
		{
			light0->setLightPos(light0->getLightPos() - vec4(0.0f, 0.0f, 0.1f, 0.0f));
			light0->setConeDirection(light0->getConeDirection() - vec3(0.0f, 0.0f, 0.1f));
		}
		if (keys[SDL_SCANCODE_KP_8])
		{
			light0->setLightPos(light0->getLightPos() + vec4(0.0f, 0.1f, 0.0f, 0.0f));
			light0->setConeDirection(light0->getConeDirection() + vec3(0.0f, 0.1f, 0.0f));
		}
		if (keys[SDL_SCANCODE_KP_2])
		{
			light0->setLightPos(light0->getLightPos() - vec4(0.0f, 0.1f, 0.0f, 0.0f));
			light0->setConeDirection(light0->getConeDirection() - vec3(0.0f, 0.1f, 0.0f));
		}
		if (keys[SDL_SCANCODE_KP_4])
		{
			light0->setLightPos(light0->getLightPos() - vec4(0.1f, 0.0f, 0.0f, 0.0f));
			light0->setConeDirection(light0->getConeDirection() - vec3(0.1f, 0.0f, 0.0f));
		}
		if (keys[SDL_SCANCODE_KP_6])
		{
			light0->setLightPos(light0->getLightPos() + vec4(0.1f, 0.0f, 0.0f, 0.0f));
			light0->setConeDirection(light0->getConeDirection() + vec3(0.1f, 0.0f, 0.0f));
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

	if (keys[SDL_SCANCODE_O])
	{
		for (int i = 0; i <= 25; i++)
		{
			boxes[i]->setLightOn(1);
		}
	}
	if (keys[SDL_SCANCODE_P])
	{
		for (int i = 0; i <= 25; i++)
		{
			boxes[i]->setLightOn(0);
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
		for (int i = 0; i <= 25; i++)
		{
			// Increases specular shininess
			if (keys[SDL_SCANCODE_KP_PLUS]) boxes[i]->setSpecularValue(boxes[i]->getSpecularValue() + 1);
			// Decreases specular shininess
			if (keys[SDL_SCANCODE_KP_MINUS]) boxes[i]->setSpecularValue(boxes[i]->getSpecularValue() - 1);
		}
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

//void Game::renderWithShadows(SDL_Window * window) {// Example for incomplete implementation of shadow mapping as example of rendering method, currently unworking so will not build if commenting is removed
//	int pass = 1;
//	glEnable(GL_CULL_FACE);
//	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glm::mat4 projection(1.0);// Sets up a fresh projection at the start of the scene
//	rt3d::setUniformMatrix4fv(util->getLightMapProgram(), "projection", glm::value_ptr(util->getProjection()));
//
//	GLfloat scale(1.0f); // just to allow easy scaling of complete scene
//	glm::mat4 modelview(1.0); // set base position for scene
//	mvStack.push(modelview); // modelview is pushed onto the stack to begin
//
//							 // Camera set
//	camera->setAt(util->moveForward(camera->getEyePos(), camera->getRotation(), 1.0f));
//	// lookat is updated
//	mvStack.top() = mvStack.top() = glm::lookAt(camera->getEyePos(), camera->getAt(), camera->getUp());
//	// Base scene is now setup
//
//	// 1. render depth of scene to texture (from light's perspective)
//	if (pass == 1) {
//		shadowFBO.firstPass(light0->getLightPos(), util->getDepthProgram());
//
//		//boxes[0]->drawWithVariableTextures(mvStack, util->getCombinedTextureProgram(), util->getProjection(), 3, 0, 0, 0.0f, light0->getLight(), camera->getEyePos());
//		//boxes[1]->drawWithVariableTextures(mvStack, util->getCombinedTextureProgram(), util->getProjection(), 3, 0, 0, 0.0f, light0->getLight(), camera->getEyePos());
//		pass = 2;
//	}
//
//	// 2. render scene as normal using the generated depth/shadow map  
//	else {
//		shadowFBO.secondPass(util->getShadowProgram());
//
//		// Skybox drawn
//		skybox->draw(mvStack, util->getCubeMapProgram(), util->getProjection());
//		// The scene objects decide which shader and light they will use in the scene so it does not matter where the light
//		// Is declared in the render portion of the program.
//		// Draws the scene lights that apply to all objects with Phong shader as default for the lightmapped box
//		light0->draw(mvStack, util->getShadowProgram(), util->getProjection(), light0->getAttenuationConstant()); // Right hand side light
//		light1->draw(mvStack, util->getShadowProgram(), util->getProjection(), light1->getAttenuationConstant()); // Left hand side light
//		//boxes[0]->draw(mvStack, util->getShadowProgram(), util->getProjection(), true, boxes[0]->getTextureVisible(), boxes[0]->getSpecularValue(), boxes[0]->getRotation() + 0.1f, light0->getLight(), light1->getLight(),1); // This is the lightmapped box
//		boxes[1]->drawWithTwoLights(mvStack, util->getShadowProgram(), util->getProjection(), boxes[1]->getRotation() + 0.1f, light0->getLight(), light1->getLight(),1);
//		// Draws a box around the light that simply follows it.
//		lightBox->draw(mvStack, util->getShadowProgram(), util->getProjection(), 1.0f, light0->getLight()); // Small box on the right
//		lightBox2->draw(mvStack, util->getShadowProgram(), util->getProjection(), 1.0f, light0->getLight()); // Small box on the left
//																												   // HUD drawn last so it renders on top.
//		hud->draw(mvStack, keycardCount, util->getShaderProgram());
//		//keys->draw(mvStack, util->getShaderProgram());
//		//nameWaterMark->draw(mvStack, util->getShaderProgram(), 9, vec3(0.8, 0.9, 0.0), vec3(0.2, 0.1, 0.0), { 255,255,255 });
//		mvStack.pop();
//		glDepthMask(GL_TRUE);
//		SDL_GL_SwapWindow(window); // swap buffers
//		pass = 1;
//
//		//Shadow program in its final form would include everything from the other shaders, including light mapping, normal mapping and multiple light sources
//	}
//}
// Draws all of the scene objects
