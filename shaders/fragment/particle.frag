#version 330
// Some drivers require the following
precision highp float;

uniform sampler2D textureUnit0;

in vec3 ex_Color;
in vec2 ex_TexCoord;

layout(location = 0) out vec4 out_Color;

void main(void)
{

	out_Color = texture(textureUnit0, gl_PointCoord); // use the built-in gl_PointCoord instead of texture coordinated
	
	if(0.9 < out_Color.r && 0.9 < out_Color.g && 0.9 < out_Color.b) // detect close to white (based on texture)
		{discard;} // discard the fragment
}