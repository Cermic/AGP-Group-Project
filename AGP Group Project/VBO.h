#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

class VBO {
private:
	GLuint VBO1;
public:
	VBO();
	void generateTangentsVBO(GLuint meshObject, std::vector<GLfloat> tangents);

};