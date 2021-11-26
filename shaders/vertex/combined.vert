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
out float out_Attenuation[numberOfLights];
out mat3 out_TBN;

// multiply each vertex position by the MVP matrix
// and find V, L, N vectors for the fragment shader
void main(void) {

	light[0] = light0;						   // Takes the uniforms from light0 and 1 and places them in the appropriate slot in the array.
	light[1] = light1;
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
	//	gl_Position = projection * vertexPosition; // uses modelview to work 
	// work out what the view is when it's passed in to stop weid shit happening. 
	
	
		// Distance from the vertex to the light
		dist_From_Vertex_To_Light[i] = distance(vec4(position, 1.0f), lightPosition[i]);

		// Attenuation Calculation
		out_Attenuation[i] = light[i].attConst + light[i].attLinear * dist_From_Vertex_To_Light[i] + 
		light[i].attQuadratic * dist_From_Vertex_To_Light[i] * dist_From_Vertex_To_Light[i];

		//cone restrictions (affects attenuation)
        float lightToSurfaceAngle = degrees(acos(dot(-surface_Normal_To_Light_From_Vertex[i], normalize(light[i].coneDirection))));
        if(lightToSurfaceAngle > light[i].coneAngle)
		{
           light[i].attConst = 1.0;
        }
	}

		// This section of the shader calculates the tangent, normals and bitangents to be used in the TBN matrix
		vec3 tan = normalize(vec3(model * in_Tangent));
		vec3 N = normalize(vec3(model * vec4 (in_Normal, 0.0f)));
		tan = normalize (tan - dot(tan, N)*N);
		vec3 bitan = cross(N, tan);			//w used in case the handnesness is coded in w coord.
		
		//TBN matrix is required for calculating changes in normals during normal mapping
		out_TBN = transpose(mat3(tan,bitan,N));

}