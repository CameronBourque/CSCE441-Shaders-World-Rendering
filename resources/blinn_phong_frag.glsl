#version 120

uniform vec3 lightPos[10];
uniform vec3 lightColor[10];
uniform vec3 ke;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

varying vec4 p; // in camera space
varying vec3 cNor; // in camera space

void main()
{
	float A0 = 1.0;
	float A1 = 0.0429;
	float A2 = 0.9857;

	vec3 n = normalize(cNor);

	// get diffuse component
	vec3 l[10];
	vec3 cd[10];
	for(int i = 0; i < 10; i++)
	{
		l[i] = normalize(lightPos[i] - p.xyz);
		cd[i] = kd * max(dot(l[i], n), 0);
	}

	// get specular component
	vec3 e = normalize(-p.xyz);
	vec3 h[10];
	vec3 cs[10];
	for(int i = 0; i < 10; i++)
	{
		h[i] = normalize(l[i] + e);
		cs[i] = ks * pow(max(dot(h[i], n), 0), s);
	}

	// get the color
	vec3 fragColor = ke;
	for(int i = 0; i < 10; i++)
	{
		float r = distance(lightPos[i], p.xyz);
		vec3 color = lightColor[i] * (cd[i] + cs[i]);
		float attenuation = 1.0f / (A0 + (A1 * r) + (A2 * r * r));
		fragColor += color * attenuation;
	}
	gl_FragColor = vec4(fragColor, 1.0);
}
