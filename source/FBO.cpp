#include "FBO.h"


FBO::FBO()
{
	//Default constructor
}

void FBO::generateShadowFBO()
{
	// create depth texture
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::firstPass(vec3 lightPos, GLuint shader) {
	//pass = 0;
	glUseProgram(shader);
	vec3 lightPosition = vec3(lightPos.x, lightPos.y, lightPos.z);
	float near_plane = 1.0f, far_plane = 7.5f;
	mat4 lightProjection = ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	mat4 lightView = lookAt(lightPosition, vec3(0.0f), vec3(0.0, 1.0, 0.0));
	mat4 lightSpaceMatrix = lightProjection * lightView;

	// render scene from light's point of view
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);// shadow maps often have a different resolution compared to what we render the scene in, we need to change the viewport parameters to accommodate for the size of the shadow map. If we forget to update the viewport parameters, the resulting depth map will be either incomplete or too small.
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	rt3d::setUniformMatrix4fv(shader, "lightSpaceMatrix", value_ptr(lightSpaceMatrix));
}


void FBO::secondPass(GLuint shader) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//pass = 1;
	glViewport(0, 0, 1280, 720); //Hardcoded values as screen size is hard coded into setupRC
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader);

}
