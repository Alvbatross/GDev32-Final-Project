#version 330

in vec3 fragPosition;
in vec3 outColor;
in vec3 fragNormal;
in vec4 lightFragPosition;

// Final color of the fragment, which we are required to output
out vec4 fragColor;

uniform vec3 directionalLightDirection, lightAmbient, lightDiffuse, lightSpecular;
uniform vec3 objectSpec;
uniform float objectShine;
uniform vec3 cameraPosition;

uniform sampler2D shadowMap;

void main()
{
	vec3 fragLightNDC = vec3(lightFragPosition.xyz/lightFragPosition.w);
	
	float flNDCx = (fragLightNDC.x + 1) / 2;
	float flNDCy = (fragLightNDC.y + 1) / 2;
	float flNDCz = (fragLightNDC.z + 1) / 2;

	vec4 depthValue = texture(shadowMap, vec2(flNDCx, flNDCy));
	float biasValue = max(0.05 * (1.0 - dot(fragNormal, directionalLightDirection)), 0.005);
	
	if(depthValue.x >= flNDCz - 0.000005)
	{
		vec3 norm = normalize(fragNormal);
		vec3 viewDir = normalize(cameraPosition - fragPosition);
		vec3 ambient, diffuse, specular;

		vec3 objectColor = vec3(1.0, 1.0, 1.0);

		vec3 directionalLightDir = normalize(-directionalLightDirection);
		float directionalLightAmbience = 1.0f;
		vec3 directionalLightAmbient = directionalLightAmbience * lightAmbient * vec3(outColor);

		float directionalLightDiff = max(dot(norm, directionalLightDir), 0.0f);
		vec3 directionalLightDiffuse = directionalLightDiff * lightDiffuse * vec3(outColor);

		vec3 directionalLightReflectDir = reflect(-directionalLightDir, norm);

		float directionalLightSpec = pow(max(dot(viewDir, directionalLightReflectDir), 0.0f), objectShine);
		vec3 directionalLightSpecular = directionalLightSpec * lightSpecular * objectSpec;

		ambient = directionalLightAmbient;
		diffuse = vec3(directionalLightDiffuse);
		specular = directionalLightSpecular;

		vec3 finalColor = (ambient * vec3(outColor) + diffuse + specular);
		//if (fragPosition.x < 0.0)
		//{
		//	finalColor = vec3(1.0, 1.0, 1.0);
		//}

		// Pass the final color to our fragColor output variable
		fragColor = vec4(finalColor, 1.0);
	}
	else
	{
		//fragColor = vec4(1.f,0.f,0.f,1.f);
	}
}
