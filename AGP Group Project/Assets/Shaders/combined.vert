// phong-tex2L.vert
// Vertex shader for use with a Phong or other reflection model fragment shader
// Calculates and passes on V, L, N vectors for use in fragment shader, phong2L.frag
#version 330


const int numberOfLights = 2;
vec4 lightPosition[numberOfLights];

uniform mat4 modelview;
uniform mat4 projection;
uniform vec4 light0Position;
uniform vec4 light1Position;
uniform vec3 cameraPos;		
uniform mat4 model;
uniform mat4 view;
//normal mapping


layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Color;
layout(location = 2) in vec3 in_Normal;
layout(location = 3) in vec2 in_TexCoord;
layout(location = 5) in vec4 in_Tangent;										//normal mapping


out vec3 surface_Normal_To_Eye_From_Vertex;
out vec3 vertex_In_Eye_Coordinates;
out vec3 surface_Normal_To_Light_From_Vertex[numberOfLights];

out vec2 ex_TexCoord;

out float dist_From_Vertex_To_Light[numberOfLights];

out mat3 out_TBN;


// multiply each vertex position by the MVP matrix
// and find V, L, N vectors for the fragment shader
void main(void) {

	lightPosition[0] = light0Position;
	lightPosition[1] = light1Position;

	//mat3 normalmatrix = (transpose(inverse(mat3(model))));

	for(int i = 0; i < numberOfLights; i++)
	{
		// vertex into eye coordinates
		vec3 vertexPosition = vec3(model * vec4(in_Position,1.0f));

		// Find V, eye is at (0,0,0)
		vertex_In_Eye_Coordinates = normalize(-vertexPosition).xyz;

		// N - surface normal in eye coordinates
		// taking the rotation part of the modelview matrix to generate the normal matrix
		// (if scaling is includes, should use transpose inverse modelview matrix!)
		// this is somewhat wasteful in compute time and should really be part of the cpu program,
		// giving an additional uniform input

		
		surface_Normal_To_Eye_From_Vertex = normalize(vec3 (model * in_Tangent));

		// L - to light source from vertex
		surface_Normal_To_Light_From_Vertex[i] = normalize(lightPosition[i].xyz - vertexPosition.xyz);

		ex_TexCoord = in_TexCoord;

		vec3 position = vec3 (modelview * vec4(in_Position,1.0f));
		gl_Position = projection * vec4(position, 1.0f);
	
		// Distance from the vertex to the light
		dist_From_Vertex_To_Light[i] = distance(vec4(position, 1.0f), lightPosition[i]);
	}

		vec3 tan = normalize(vec3(model * in_Tangent));
		vec3 N = normalize(vec3(model * vec4 (in_Normal, 0.0f)));
		tan = normalize (tan - dot(tan, N)*N);
		vec3 bitan = cross(N, tan);			//w used in case the handnesness is coded in w coord.

		out_TBN = transpose(mat3(tan,bitan,N));

}