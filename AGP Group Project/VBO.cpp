#include "VBO.h"

VBO::VBO()
{
	//default constructor
}

void VBO::generateTangentsVBO(GLuint meshObject, std::vector<GLfloat> tangents)
{
	glBindVertexArray(meshObject);
	glGenBuffers(1, &VBO1);

	// VBO for tangent data
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(GLfloat), tangents.data(), GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)5, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(5);
	glBindVertexArray(0);
}
