#version 150

in  vec3 pass_Normal;
in  vec3 pass_Color;
in  vec3 pass_Sun;
in  vec4 gl_FragCoord;
out vec4 out_Color;

float Ia = 0.2f;
float Id = 0.5f;
float Is = 1.0f;

vec3 Ka = pass_Color;
vec3 Kd = pass_Color;
vec3 Ks = vec3(1.0f,1.0f,1.0f);

void main() {
//  out_Color = vec4(abs(normalize(pass_Normal)), 1.0);
  out_Color = vec4(Ka * Ia + Kd * Id * dot(normalize(gl_FragCoord.xyz - pass_Sun), normalize(pass_Normal)) , 1.0);
}
