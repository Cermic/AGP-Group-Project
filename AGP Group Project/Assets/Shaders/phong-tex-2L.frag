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
uniform sampler2D textureUnit0;

in vec3 ex_N;
in vec3 ex_V;
in vec3 ex_L;
in vec2 ex_TexCoord;
in float ex_D;
layout(location = 0) out vec4 out_Color;
 
void main(void) {

	light[0] = light0;						   // Takes the uniforms from light0 and 1 and places them in the appropriate slot in the array.
	light[1] = light1;

	vec4 tmp_Colour[numberOfLights];           // Vec4 array to hold the colours that will be added together into litColour
	vec4 litColour[numberOfLights];			   // Litcolour will hold to total colour
	float specificAttenuation[numberOfLights]; // Captures the attenuation specific to the particular light being iterated through in the array.

	vec4 totalLitColours; // Holds to total colour of all lights in the scene.
	float totalAttenuation; // stores the total light attenuation in the scene.

	for (int i = 0; i < numberOfLights; i++)
	{
		// Ambient intensity
		vec4 ambientI = light[i].ambient * material.ambient;

		// Diffuse intensity
		vec4 diffuseI = light[i].diffuse * material.diffuse;				
		diffuseI = diffuseI * max(dot(normalize(ex_N),normalize(ex_L)),0);

		// Specular intensity
		// Calculate R - reflection of light
		vec3 R = normalize(reflect(normalize(-ex_L),normalize(ex_N)));
		vec4 specularI = light[i].specular * material.specular;
		specularI = specularI * pow(max(dot(R,ex_V),0), material.shininess);

		// Attenuation Calculation
		//specificAttenuation[i] = light[i].attConst + light[i].attLinear * ex_D + light[i].attQuadratic * ex_D * ex_D;
		float attenuation = light[i].attConst + light[i].attLinear * ex_D + light[i].attQuadratic * ex_D * ex_D;

		// Lit colout being calculated
		tmp_Colour[i] = (diffuseI + specularI);
		//attenuation does not affect ambient light
		litColour[i] = ambientI + vec4((tmp_Colour[i].rgb / attenuation), 1.0);

		totalLitColours += (litColour[i]/2);
	}
	
	// Fragment colour
	out_Color = (totalLitColours) * texture(textureUnit0, ex_TexCoord);
}