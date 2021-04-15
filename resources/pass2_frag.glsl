#version 120

uniform sampler2D posTexture;
uniform sampler2D norTexture;
uniform sampler2D keTexture;
uniform sampler2D kdTexture;
uniform vec2 windowSize;

// more uniforms for lighting
uniform vec3 lightPos[25];
uniform vec3 lightColor[25];
uniform vec3 ks;
uniform float s;

void main()
{
    vec2 tex;
    tex.x = gl_FragCoord.x/windowSize.x;
    tex.y = gl_FragCoord.y/windowSize.y;

    // fetch shading data
    vec3 p = texture2D(posTexture, tex).rgb;
    vec3 nor = texture2D(norTexture, tex).rgb;
    vec3 ke = texture2D(keTexture, tex).rgb;
    vec3 kd = texture2D(kdTexture, tex).rgb;

	// set some constants
	float A0 = 1.0;
	float A1 = 0.0429;
	float A2 = 0.9857;

	// get the normalized normals
	vec3 n = normalize(nor);

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
	gl_FragColor = vec4(fragColor, 1.0);	// Default output
	//gl_FragColor.rgb = p;		// Position output
	//gl_FragColor.rgb = nor;	// Normal output
	//gl_FragColor.rgb = ke;	// Emissive output
	//gl_FragColor.rgb = kd;	// Diffuse output
}
