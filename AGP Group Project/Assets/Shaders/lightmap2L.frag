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
// We take ambient out because ambient doesnt affect the lighting much so we use the diffuse to calculate 
// lighting for the texture.

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
uniform sampler2D textureUnit1;
uniform int textureVisible;
uniform int specularValue;

uniform sampler2D ambient;
uniform sampler2D diffuse;
uniform sampler2D speuclar;

in vec3 surface_Normal_To_Eye_From_Vertex;
in vec3 vertex_In_Eye_Coordinates;
in vec3 surface_Normal_To_Light_From_Vertex[numberOfLights];
in vec2 ex_TexCoord;
in float dist_From_Vertex_To_Light[numberOfLights];
layout(location = 0) out vec4 out_Color;
 
void main(void) {

	light[0] = light0;						   // Takes the uniforms from light0 and 1 and places them in the appropriate slot in the array.
	light[1] = light1;

	vec4 tmp_Colour[numberOfLights];           // Vec4 array to hold the colours that will be added together into litColour
	vec4 litColour[numberOfLights];			   // Litcolour will hold to total colour
	float specificAttenuation[numberOfLights]; // Captures the attenuation specific to the particular light being iterated through in the array.

	vec4 totalLitColours=vec4(0.0f,0.0f,0.0f,0.0f); // Holds to total colour of all lights in the scene.
	
	for (int i = 0; i < numberOfLights; i++)
	{
	// Ambient 
	vec4 ambient = light[i].ambient ;

	// Diffuse 
	float diffuseI = max(dot(normalize(surface_Normal_To_Eye_From_Vertex),normalize(surface_Normal_To_Light_From_Vertex[i])),0);
	vec4 diffuse = light[i].diffuse * diffuseI  * texture(textureUnit1, ex_TexCoord).r;  
	// red component = brightness or intensity of the diffuse
	// The diffuse intensity is given by the r(red) value of textureUnit1 at the relevant coordindate for the pixel.

	// Specular 
	// Calculate R - reflection of light
	vec3 R = normalize(reflect(normalize(-surface_Normal_To_Light_From_Vertex[i]),
		normalize(surface_Normal_To_Eye_From_Vertex)));

	// texture g(green) component is shininess
	float f_texel1_spec_shininess = pow(max(dot(R,vertex_In_Eye_Coordinates),0), texture(textureUnit1, ex_TexCoord).g); 
	// The shininess is given by the g(green) value of textureUnit1 at the relevant coordindate for the pixel.
	// This is how the shader can create a variance in shininess. It's measuring the green value per pixel, not over the whole texture.
	
	vec4 specular =  light[i].specular * (specularValue * f_texel1_spec_shininess) * texture(textureUnit1, ex_TexCoord).b ; 
	// We are now using the texturecoords to decide the material specular value.
	// blue component of the texture is the specular intensity
	// The speuclar intensity is given by the b(blue) value of textureUnit1 at the relevant coordindate for the pixel.

	// Attenuation //
	float attenuation = light[i].attConst + light[i].attLinear * dist_From_Vertex_To_Light[i] + 
	light[i].attQuadratic * dist_From_Vertex_To_Light[i] * dist_From_Vertex_To_Light[i];

	// Lit colour being calculated
	tmp_Colour[i] = (diffuse + specular);
	//attenuation does not affect ambient light
	litColour[i] = ambient + vec4((tmp_Colour[i].rgb / attenuation), 1.0);

	totalLitColours += (litColour[i]/2);
	}
	// Fragment colour //
	// The actual material seen by the user should be the box which is done here.
	vec4 material_tex;
	if(textureVisible == 0)
	{
		material_tex = texture(textureUnit0, ex_TexCoord);
	}
	if(textureVisible == 1)
	{
		material_tex = texture(textureUnit1, ex_TexCoord);
	}
	out_Color = totalLitColours * material_tex;
	// The colour parameters collected by our rgb values are now applied to the box texture to 
	// Create the appropriate lighting effects on the seperate parts of the box.

}