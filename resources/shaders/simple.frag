#version 150

precision mediump float;

in  vec3 pass_Normal;
in  vec3 vertPos;

in  vec3 pass_Color;
in  vec3 pass_Sun;
in  vec4 gl_FragCoord;
in  vec3 pass_pos;
in  vec2 pass_TexCoord;


uniform sampler2D ColorTex;


out vec4 out_Color;

vec3 lightColor = vec3(0.5,0.5,0.5);
float lightPower = 0.5;
vec3 ambientColor = texture(ColorTex, pass_TexCoord).xyz;
vec3 diffuseColor = texture(ColorTex, pass_TexCoord).xyz;
vec3 specColor = vec3(1.0, 1.0, 1.0);
float shininess = 0.5;

//vec3 Ka = pass_Color;
//vec3 Kd = pass_Color;

vec3 Ks = vec3(1.0f,1.0f,1.0f);

vec3 Ka = texture(ColorTex, pass_TexCoord).xyz * 0.2;
vec3 Kd = texture(ColorTex, pass_TexCoord).xyz * 0.5;

void main() {

	vec3 normal = normalize(pass_Normal);
	vec3 lightDir = pass_Sun - vertPos;
	lightDir = normalize(lightDir);

	float lambertian = max(dot(lightDir,normal), 0.0);
	float specular = 0.0;

	if(lambertian > 0.0) {

		vec3 viewDir = normalize(-vertPos);

		// this is blinn phong
		vec3 halfDir = normalize(lightDir + viewDir);
		float specAngle = max(dot(halfDir, normal), 0.0);
		specular = pow(specAngle, shininess);
	}
	vec3 colorLinear = ambientColor + diffuseColor * lambertian * lightColor * lightPower + specColor * specular * lightColor * lightPower;

	out_Color = vec4(colorLinear, 1.0);




/*  out_Color = vec4((Ka * Ia) + (Kd * Id * dot(normalize(pass_Sun - pass_frag_pos), normalize(pass_Normal)) + (Ks *Is * pow( dot(normalize(pass_Normal), normalize(-pass_frag_pos + (pass_Sun - pass_frag_pos))), 3))), 1.0);*/
}
