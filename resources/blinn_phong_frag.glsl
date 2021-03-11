#version 120

uniform vec3 lightPos;
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
	vec3 l = normalize(lightPos - p.xyz);
	vec3 cd = kd * max(dot(l, n), 0);

	// get specular component
	vec3 e = normalize(-p.xyz);
	vec3 h = normalize(l + e);
	vec3 cs = ks * pow(max(dot(h, n), 0), s);

	// get the color
	vec3 color = ca + cd + cs;
	gl_FragColor = vec4(color, 1.0);
}
