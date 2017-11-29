// Phong fragment shader phong-tex.frag matched with phong-tex.vert
#version 330

// Some drivers require the following
precision highp float;

// This is just the definition of the lightStruct so that lightStructs that are passed in know what they are composed of.
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

uniform lightStruct light;
uniform materialStruct material;
uniform sampler2D textureUnit0;

in vec3 surface_Normal_To_Eye_From_Vertex;
in vec3 vertex_In_Eye_Coordinates;
in vec3 surface_Normal_To_Light_From_Vertex;
in vec2 ex_TexCoord;
in float dist_From_Vertex_To_Light;

in float out_Attenuation;

layout(location = 0) out vec4 out_Color;
 
void main(void) {

	// Ambient intensity
	vec4 ambientI = light.ambient * material.ambient;

	// Diffuse intensity
	vec4 diffuseI = light.diffuse * material.diffuse;				
	diffuseI = diffuseI * max(dot(normalize(surface_Normal_To_Eye_From_Vertex),
	normalize(surface_Normal_To_Light_From_Vertex)),0);

	// Specular intensity
	// Calculate R - reflection of light
	vec3 R = normalize(reflect(normalize(-surface_Normal_To_Light_From_Vertex),
	normalize(surface_Normal_To_Eye_From_Vertex)));
	vec4 specularI = light.specular * material.specular;
	specularI = specularI * pow(max(dot(R,vertex_In_Eye_Coordinates),0), material.shininess);

	vec4 tmp_Color = (diffuseI + specularI);
	//attenuation does not affect ambient light
	vec4 litColour = ambientI + vec4((tmp_Color.rgb / out_Attenuation), 1.0);

	// Fragment colour
	out_Color = (litColour) * texture(textureUnit0, ex_TexCoord);
}