#version 150


in vec2 pass_TexCoord;
uniform bool grey_bool;
uniform sampler2D ColorTex;

out vec4 out_Color;



void main(void)
{
	if(grey_bool == false){
		out_Color = texture(ColorTex, pass_TexCoord);
	}
	else{
		out_Color = vec4(pass_TexCoord,0.0,0.0);
	}

}
