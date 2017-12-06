#version 150

in  vec3 pass_Normal;
in  vec3 pass_Color;
in  vec3 pass_Sun;
in  vec4 gl_FragCoord;
in vec3 pass_pos;
//in sampler2d colorTex;
in vec2 pass_TexCoord;
out vec4 out_Color;

float Ia = 0.2f;
float Id = 0.5f;
float Is = 0.5f;

//vec3 Ka = pass_Color;
//vec3 Kd = pass_Color;

vec3 Ks = vec3(1.0f,1.0f,1.0f);

uniform sampler2D ColorTex;

vec3 Ka = texture(ColorTex, pass_TexCoord).xyz;
vec3 Kd = texture(ColorTex, pass_TexCoord).xyz;

void main() {
//  out_Color = vec4(abs(normalize(pass_Normal)), 1.0);
  out_Color = vec4((Ka * Ia) + (Kd * Id * dot(normalize(pass_Sun - pass_pos), normalize(pass_Normal)) + (Ks *Is * pow( dot(normalize(pass_Normal), normalize(-pass_pos + (pass_Sun - pass_pos))), 3))), 1.0);
}
