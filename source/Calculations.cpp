#include "Calculations.h"

calculations::calculations()
{
	//default constructor
}

void calculations::calculateTangents(std::vector<GLfloat>& tangents, std::vector<GLfloat>& verts, std::vector<GLfloat>& normals, std::vector<GLfloat>& tex_coords, std::vector<GLuint>& indices)
{
	// Code taken from http://www.terathon.com/code/tangent.html and modified slightly to use vectors instead of arrays
	// Lengyel, Eric. “Computing Tangent Space Basis Vectors for an Arbitrary Mesh”. Terathon Software 3D Graphics Library, 2001. 

	std::vector<glm::vec3> tan1(verts.size() / 3, glm::vec3(0.0f));
	std::vector<glm::vec3> tan2(verts.size() / 3, glm::vec3(0.0f));
	int triCount = indices.size() / 3;
	for (int c = 0; c < indices.size(); c += 3)
	{
		int i1 = indices[c];
		int i2 = indices[c + 1];
		int i3 = indices[c + 2];

		glm::vec3 v1(verts[i1 * 3], verts[i1 * 3 + 1], verts[i1 * 3 + 2]);
		glm::vec3 v2(verts[i2 * 3], verts[i2 * 3 + 1], verts[i2 * 3 + 2]);
		glm::vec3 v3(verts[i3 * 3], verts[i3 * 3 + 1], verts[i3 * 3 + 2]);

		glm::vec2 w1(tex_coords[i1 * 2], tex_coords[i1 * 2 + 1]);
		glm::vec2 w2(tex_coords[i2 * 2], tex_coords[i2 * 2 + 1]);
		glm::vec2 w3(tex_coords[i3 * 2], tex_coords[i3 * 2 + 1]);

		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;

		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;

		float r = 1.0F / (s1 * t2 - s2 * t1);
		glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r);
		glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r);

		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;

		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
	}

	for (int a = 0; a < verts.size(); a += 3)
	{
		glm::vec3 n(normals[a], normals[a + 1], normals[a + 2]);
		glm::vec3 t = tan1[a / 3];

		glm::vec3 tangent;
		tangent = (t - n * glm::normalize(glm::dot(n, t)));

		// handedness
		GLfloat w = (glm::dot(glm::cross(n, t), tan2[a / 3]) < 0.0f);

		// Set the same tangent for all three vertices of the triangle.
		tangents.push_back(tangent.x);
		tangents.push_back(tangent.y);
		tangents.push_back(tangent.z);
		tangents.push_back(w);

	}
}
