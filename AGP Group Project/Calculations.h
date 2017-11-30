#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <vector>

class calculations {
private:
public:
	calculations();

	void calculateTangents(std::vector<GLfloat> &tangents, std::vector<GLfloat> &verts, std::vector<GLfloat> &normals, std::vector<GLfloat> &tex_coords, std::vector<GLuint> &indices);
};
