#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoord;



//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;
uniform vec3 PlanetColor;
//uniform sampler2d colorTex;
//uniform Glint rendermode;



vec3 sun_pos = vec3(0.0,0.0,0.0);

out vec3 vertPos;

//out Glint rendermode;
out vec3 pass_Normal;
out vec3 pass_Color;
out vec3 pass_Sun;
out vec3 pass_pos;

out vec2 pass_TexCoord;

void main(void)
{
	gl_Position = (ProjectionMatrix  * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);
	vertPos = (ViewMatrix * ModelMatrix * vec4(in_Position, 1.0)).xyz;
	pass_Normal = (NormalMatrix * vec4(in_Normal, 0.0)).xyz;

	pass_Sun = (ViewMatrix *  vec4(sun_pos, 1.0)).xyz;
	pass_Color = PlanetColor;
	pass_pos = gl_Position.xyz;
	pass_TexCoord = in_TexCoord;
}
