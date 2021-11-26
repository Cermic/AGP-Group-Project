// use textures, but no lighting
#version 330

uniform mat4 modelview;
uniform mat4 projection;

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Color;
layout(location = 3) in vec2 in_TexCoord;

out vec2 ex_TexCoord;
out vec3 ex_Color;

// multiply each vertex position by the MVP matrix
void main(void) {

	// vertex into eye coordinates
	vec4 vertexPosition = modelview * vec4(in_Position,1.0);
    gl_Position = projection * vertexPosition;
	//gl_PointSize = 5.0;

	ex_TexCoord = in_TexCoord;
	//ex_Color = in_Color; //This line does some crazy stuff.
}