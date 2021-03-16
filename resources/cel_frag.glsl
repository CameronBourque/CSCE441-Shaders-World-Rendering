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

	// find silhoutte edges
	float dotNE = dot(n, e);

	// get the color
	vec3 color;
	if(dotNE < 0.3 && dotNE > -0.3)
	{
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else
	{
		for(int i = 0; i < 2; i++)
		{
			color += lightColor[i] * (ca + cd[i] + cs[i]);
		}

		// quantize red color
		if(color.r < 0.25)
		{
			color.r = 0.0;
		}
		else if(color.r < 0.5)
		{
			color.r = 0.25;
		}
		else if(color.r < 0.75)
		{
			color.r = 0.5;
		}
		else
		{
			color.r = 0.75;
		}

		// quantize green color
		if(color.g < 0.25)
		{
			color.g = 0.0;
		}
		else if(color.g < 0.5)
		{
			color.g = 0.25;
		}
		else if(color.g < 0.75)
		{
			color.g = 0.5;
		}
		else
		{
			color.g = 0.75;
		}

		// quantize blue color
		if(color.b < 0.25)
		{
			color.b = 0.0;
		}
		else if(color.b < 0.5)
		{
			color.b = 0.25;
		}
		else if(color.b < 0.75)
		{
			color.b = 0.5;
		}
		else
		{
			color.b = 0.75;
		}

		// set color
		gl_FragColor = vec4(color, 1.0);
	}
}
