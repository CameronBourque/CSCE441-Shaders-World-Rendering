#version 120

uniform vec3 ke;
uniform vec3 kd;

varying vec4 p; // in camera space
varying vec3 cNor; // in camera space

void main()
{
	gl_FragData[0].xyz = p.xyz;
	gl_FragData[1].xyz = cNor;
	gl_FragData[2].xyz = ke;
	gl_FragData[3].xyz = kd;
}
