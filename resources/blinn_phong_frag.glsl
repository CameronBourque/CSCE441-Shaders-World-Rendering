#version 120

uniform vec3 lightPos[2];
uniform vec3 lightColor[2];
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

varying vec4 p; // in camera space
varying vec3 cNor; // in camera space

void main()
{
	vec3 n = normalize(cNor);

	// get ambient component
	vec3 ca = ka;

	// get diffuse component
	vec3 l[2];
	vec3 cd[2];
	for(int i = 0; i < 2; i++)
	{
		l[i] = normalize(lightPos[i] - p.xyz);
		cd[i] = kd * max(dot(l[i], n), 0);
	}

	// get specular component
	vec3 e = normalize(-p.xyz);
	vec3 h[2];
	vec3 cs[2];
	for(int i = 0; i < 2; i++)
	{
		h[i] = normalize(l[i] + e);
		cs[i] = ks * pow(max(dot(h[i], n), 0), s);
	}

	// get the color
	vec3 color;
	for(int i = 0; i < 2; i++)
	{
		color += lightColor[i] * (ca + cd[i] + cs[i]);
	}
	gl_FragColor = vec4(color, 1.0);
}
