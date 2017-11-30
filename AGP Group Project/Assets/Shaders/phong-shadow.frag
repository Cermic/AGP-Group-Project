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
uniform materialStruct material;
uniform sampler2D textureUnit0;
uniform sampler2D shadowMap;

in vec3 ex_N;
in vec3 ex_V;
in vec3 ex_L;
in vec2 ex_TexCoord;
in vec4 FragPosLightSpace;
in float ex_D;
layout(location = 0) out vec4 out_Color;
 
 float ShadowCalculation(vec4 fragPosLightSpace)
{
     // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main(void) {


	// Ambient intensity
	vec4 ambientI = light.ambient * material.ambient;

	// Diffuse intensity
	vec4 diffuseI = light.diffuse * material.diffuse;
	diffuseI = diffuseI * max(dot(normalize(ex_N),normalize(ex_L)),0);

	// Specular intensity
	// Calculate R - reflection of light
	vec3 R = normalize(reflect(normalize(-ex_L),normalize(ex_N)));

	vec4 specularI = light.specular * material.specular;
	specularI = specularI * pow(max(dot(R,ex_V),0), material.shininess);

	float attenuation = light.attConst + light.attLinear * ex_D + light.attQuadratic * ex_D*ex_D;

	vec4 tmp_Color = (diffuseI + specularI);
	
	//Calculate shadow
	float shadow = ShadowCalculation(FragPosLightSpace);

	//attenuation does not affect ambient light
	vec4 litColour = ambientI + ( (1.0 - shadow) * vec4( tmp_Color.rgb/* / attenuation*/, 1.0));

	// Fragment colour
	//out_Color = vec4(shadow, shadow, shadow, 1.0);
	out_Color =(litColour) * texture(textureUnit0, ex_TexCoord);
}
	