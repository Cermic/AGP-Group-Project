// phong-tex.vert
// Vertex shader for use with a Phong or other reflection model fragment shader
// Calculates and passes on V, L, N vectors for use in fragment shader, phong2.frag
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

uniform mat4 modelview;
uniform mat4 projection;
uniform lightStruct light;
lightStruct light1 = light;
uniform vec4 lightPosition;

layout (location = 0) in vec3 in_Position;	// These are the vertex attribute pointers, these point the VAO straight to the rt3d header file.
layout (location = 2) in vec3 in_Normal;
layout (location = 3) in vec2 in_TexCoord;

out vec3 surface_Normal_To_Eye_From_Vertex;
out vec3 vertex_In_Eye_Coordinates;
out vec3 surface_Normal_To_Light_From_Vertex;

out vec2 ex_TexCoord;
out float dist_From_Vertex_To_Light;
out float out_Attenuation;

// multiply each vertex position by the MVP matrix
// and find V, L, N vectors for the fragment shader
void main(void) {

	// vertex into eye coordinates
	vec4 vertexPosition = modelview * vec4(in_Position,1.0);

	// Find V - in eye coordinates, eye is at (0,0,0)
	vertex_In_Eye_Coordinates = normalize(-vertexPosition).xyz;

	// surface normal in eye coordinates
	// taking the rotation part of the modelview matrix to generate the normal matrix
	// (if scaling is includes, should use transpose inverse modelview matrix!)
	// this is somewhat wasteful in compute time and should really be part of the cpu program,
	// giving an additional uniform input
	mat3 normalmatrix = transpose(inverse(mat3(modelview)));
	surface_Normal_To_Eye_From_Vertex = normalize(normalmatrix * in_Normal);

	// L - to light source from vertex
	surface_Normal_To_Light_From_Vertex = normalize(lightPosition.xyz - vertexPosition.xyz);

	ex_TexCoord = in_TexCoord;

    gl_Position = projection * vertexPosition;
	
	// Distance from the vertex to the light
	dist_From_Vertex_To_Light = distance(vertexPosition,lightPosition);

	// Attenuation //
	out_Attenuation = light.attConst + light.attLinear * dist_From_Vertex_To_Light + 
	light.attQuadratic * dist_From_Vertex_To_Light * dist_From_Vertex_To_Light;

	//cone restrictions (affects attenuation)
        float lightToSurfaceAngle = degrees(acos(dot(-surface_Normal_To_Light_From_Vertex, normalize(light.coneDirection))));
        if(lightToSurfaceAngle > light.coneAngle)
		{
			light1 = light;
		// This is done simply because you can't edit a uniform, so it's passed into a regular lightStruct here to compensate.
           light1.attConst = 1.0;
        }

}