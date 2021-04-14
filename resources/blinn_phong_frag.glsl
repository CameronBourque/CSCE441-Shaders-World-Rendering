#version 120

uniform vec3 lightPos[25];
uniform vec3 lightColor[25];
uniform vec3 ke;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

varying vec4 p; // in camera space
varying vec3 cNor; // in camera space

void main()
{
	// set some constants
	float A0 = 1.0;
	float A1 = 0.0429;
	float A2 = 0.9857;

	// get the normalized normals
	vec3 n = normalize(cNor);

	// start with ke for the color
	vec3 fragColor = ke;

	// get the color
	for(int i = 0; i < 25; i++)
	{
		float r = distance(lightPos[i], p.xyz);

		// calculate the diffuse
		vec3 l = normalize(lightPos[i] - p.xyz);
		vec3 diffuse = kd * max(dot(l, n), 0);

		// calculate the specular
		vec3 e = normalize(-p.xyz);
		vec3 h = normalize(l + e);
		vec3 specular = ks * pow(max(dot(h, n), 0), s);

		vec3 color = lightColor[i] * (diffuse + specular);
		float attenuation = 1.0f / (A0 + (A1 * r) + (A2 * r * r));
		fragColor += color * attenuation;
	}
	gl_FragColor = vec4(fragColor, 1.0);
}
