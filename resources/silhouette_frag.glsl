#version 120

varying vec4 p; // in camera space
varying vec3 cNor; // in camera space

void main()
{
	// get normal vector
	vec3 n = normalize(cNor);

	// get eye vector
	vec3 e = normalize(-p.xyz);

	// get the dot product
	float dotNE = dot(n, e);

	// set the color
	if(dotNE < 0.3 && dotNE > -0.3)
	{
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else
	{
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
}
