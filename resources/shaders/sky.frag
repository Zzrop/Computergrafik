#version 150

precision mediump float;

in vec2 pass_TexCoord;
uniform sampler2D ColorTex1;

out vec4 out_Color;



void main(void)
{
//out_Color = texture(ColorTex1, pass_TexCoord);
out_Color = vec4(1.0,1.0,1.0,0.0);
}