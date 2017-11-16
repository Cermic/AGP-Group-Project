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

uniform lightStruct light;
uniform lightStruct light2;
uniform materialStruct material;
uniform sampler2D textureUnit0;

in vec3 ex_N;
in vec3 ex_V;
in vec3 ex_L;
in vec2 ex_TexCoord;
in float ex_D;
layout(location = 0) out vec4 out_Color;
 
void main(void) {


	// Ambient intensity
	vec4 ambientI = light2.ambient * material.ambient;			// For second light
	ambientI = light.ambient * material.ambient;				// For first light

	// Diffuse intensity
	vec4 diffuseI = light2.diffuse * material.diffuse;				// For second light
	diffuseI = diffuseI * max(dot(normalize(ex_N),normalize(ex_L)),0);

	diffuseI = light.diffuse * material.diffuse;					// For first light
	diffuseI = diffuseI * max(dot(normalize(ex_N),normalize(ex_L)),0);

	// Specular intensity
	// Calculate R - reflection of light
	vec3 R = normalize(reflect(normalize(-ex_L),normalize(ex_N)));
	vec4 specularI = light2.specular * material.specular;				// For second light
	specularI = specularI * pow(max(dot(R,ex_V),0), material.shininess);

	specularI = light.specular * material.specular;						// For first light
	specularI = specularI * pow(max(dot(R,ex_V),0), material.shininess);
	

	float attenuation = ((light.attLinear + light2.attLinear) + (light.attConst + light2.attConst)) * ex_D + (light.attQuadratic + light2.attQuadratic) * ex_D * ex_D;

	vec4 tmp_Color = (diffuseI + specularI);
	//attenuation does not affect ambient light
	vec4 litColour = ambientI+vec4(tmp_Color.rgb /attenuation, 1.0);

	// Fragment colour
	out_Color = (litColour) * texture(textureUnit0, ex_TexCoord);
}