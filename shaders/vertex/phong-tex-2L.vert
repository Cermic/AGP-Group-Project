// phong-tex2L.vert
// Vertex shader for use with a Phong or other reflection model fragment shader
// Calculates and passes on V, L, N vectors for use in fragment shader, phong2L.frag
#version 330

struct lightStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	
	float attConst;
	float attLinear;
	float attQuadratic;

	float coneAngle;
	vec3 coneDirection;
};

const int numberOfLights = 2;
vec4 lightPosition[numberOfLights];
lightStruct light[numberOfLights];

uniform mat4 modelview;
uniform mat4 projection;
uniform lightStruct light0;
uniform lightStruct light1;
uniform vec4 light0Position;
uniform vec4 light1Position;

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Color;
layout(location = 2) in vec3 in_Normal;
layout(location = 3) in vec2 in_TexCoord;


out vec3 surface_Normal_To_Eye_From_Vertex;
out vec3 vertex_In_Eye_Coordinates;
out vec3 surface_Normal_To_Light_From_Vertex[numberOfLights];

out vec2 ex_TexCoord;
out float dist_From_Vertex_To_Light[numberOfLights];
out float out_Attenuation[numberOfLights];

// multiply each vertex position by the MVP matrix
// and find V, L, N vectors for the fragment shader
void main(void) {

	light[0] = light0;						   // Takes the uniforms from light0 and 1 and places them in the appropriate slot in the array.
	light[1] = light1;
	lightPosition[0] = light0Position;
	lightPosition[1] = light1Position;

	for(int i = 0; i < numberOfLights; i++)
	{
		// vertex into eye coordinates
		vec4 vertexPosition = modelview * vec4(in_Position,1.0);

		// Find V, eye is at (0,0,0)
		vertex_In_Eye_Coordinates = normalize(-vertexPosition).xyz;

		// N - surface normal in eye coordinates
		// taking the rotation part of the modelview matrix to generate the normal matrix
		// (if scaling is includes, should use transpose inverse modelview matrix!)
		// this is somewhat wasteful in compute time and should really be part of the cpu program,
		// giving an additional uniform input
		mat3 normalmatrix = transpose(inverse(mat3(modelview)));
		surface_Normal_To_Eye_From_Vertex = normalize(normalmatrix * in_Normal);

		// L - to light source from vertex
		surface_Normal_To_Light_From_Vertex[i] = normalize(lightPosition[i].xyz - vertexPosition.xyz);

		ex_TexCoord = in_TexCoord;

		gl_Position = projection * vertexPosition;
	
		// Distance from the vertex to the light
		dist_From_Vertex_To_Light[i] = distance(vertexPosition, lightPosition[i]);

		// Attenuation Calculation
		out_Attenuation[i] = light[i].attConst + light[i].attLinear * dist_From_Vertex_To_Light[i] + 
		light[i].attQuadratic * dist_From_Vertex_To_Light[i] * dist_From_Vertex_To_Light[i];

	}
}