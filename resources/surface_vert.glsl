#version 120

uniform mat4 P;
uniform mat4 MV;
uniform mat4 ITMV;
uniform float t;

attribute vec4 aPos; // in object space

varying vec4 p; // in camera space
varying vec3 cNor; // in camera space

void main()
{
	float fx = cos(aPos.x + t) + 2;
	vec4 pos = vec4(aPos.x, fx * cos(aPos.y), fx * sin(aPos.y), aPos.w);
	p = MV * pos;

	float dfx = -sin(aPos.x + t);
	vec3 dpx = vec3(1.0, dfx * cos(aPos.y), dfx * sin(aPos.y));
	vec3 dpt = vec3(0.0, -fx * sin(aPos.y), fx * cos(aPos.y));
	vec3 norms = normalize(cross(dpx, dpt));
	vec4 normal = ITMV * vec4(-norms, 0.0);
	cNor = normalize(normal.xyz);

	gl_Position = P * MV * pos;
}
