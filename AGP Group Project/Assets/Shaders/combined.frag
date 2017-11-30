// Phong fragment shader phong-tex.frag matched with phong-tex.vert
#version 330

// Some drivers require the following
precision highp float;

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

struct materialStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

const int numberOfLights = 2;
lightStruct light[numberOfLights];
uniform lightStruct light0;
uniform lightStruct light1;

uniform materialStruct material;
uniform sampler2D textureUnit0;		// texture map (actual image)
//uniform sampler2D texture2;	// specular map
uniform sampler2D texture3;		// normal map


in vec3 surface_Normal_To_Eye_From_Vertex;
in vec3 vertex_In_Eye_Coordinates;
in vec3 surface_Normal_To_Light_From_Vertex[numberOfLights];
in vec2 ex_TexCoord;
in float dist_From_Vertex_To_Light[numberOfLights];

in float out_Attenuation[numberOfLights];

layout(location = 0) out vec4 out_Color;

in mat3 out_TBN;
 
void main(void) {

	vec3 N = out_TBN * (texture( texture3, ex_TexCoord ).rgb * 2.0 - vec3(1,1,1));//Move from tangent space to world space


	light[0] = light0;						   // Takes the uniforms from light0 and 1 and places them in the appropriate slot in the array.
	light[1] = light1;

	vec4 tmp_Colour[numberOfLights];           // Vec4 array to hold the colours that will be added together into litColour
	vec4 litColour[numberOfLights];			   // Litcolour will hold to total colour
	
	vec4 totalLitColours=vec4(0.0f,0.0f,0.0f,0.0f); // Holds to total colour of all lights in the scene.

	for (int i = 0; i < numberOfLights; i++)
	{
		// Ambient intensity
		vec4 ambientI = light[i].ambient * material.ambient;

		// Diffuse intensity
		vec4 diffuseI = light[i].diffuse * material.diffuse;				
		diffuseI = diffuseI * max(dot(normalize(N),normalize(surface_Normal_To_Light_From_Vertex[i])),0);

		// Specular intensity
		// Calculate R - reflection of light
		vec3 R = normalize(reflect(normalize(-surface_Normal_To_Light_From_Vertex[i]),normalize(N)));
		vec4 specularI = light[i].specular * material.specular;
		specularI = specularI * pow(max(dot(R,vertex_In_Eye_Coordinates),0), material.shininess);

		// Lit colout being calculated
		tmp_Colour[i] = (diffuseI + specularI);
		//attenuation does not affect ambient light
		litColour[i] = ambientI + vec4((tmp_Colour[i].rgb / out_Attenuation[i]), 1.0);

		totalLitColours += (litColour[i]/2);

	}
	
	// Fragment colour
	out_Color = (totalLitColours) * texture(textureUnit0, ex_TexCoord);
	//out_Color = vec4(N,1.0f);
}