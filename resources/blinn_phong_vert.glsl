#version 120

uniform mat4 P;
uniform mat4 MV;
uniform mat4 ITMV;

attribute vec4 aPos; // in object space
attribute vec3 aNor; // in object space

varying vec4 p; // in camera space
varying vec3 cNor; // in camera space

void main()
{
	p = MV * aPos;
	vec4 normal = ITMV * vec4(aNor, 0.0);
	cNor = normalize(normal.xyz);

	gl_Position = P * MV * aPos;
}
