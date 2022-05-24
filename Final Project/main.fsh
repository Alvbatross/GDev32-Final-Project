#version 330

in vec3 fragPosition;
in vec3 outColor;
in vec3 fragNormal;
in vec4 lightFragPosition;

// Final color of the fragment, which we are required to output
out vec4 fragColor;

uniform vec3 directionalLightDirection, lightAmbient, lightDiffuse, lightSpecular;
uniform vec3 spotLightDirection, spotLightPosition, sLightAmbient, sLightDiffuse, sLightSpecular;
uniform vec3 objectSpec;
uniform float objectShine;
uniform vec3 cameraPosition;

uniform sampler2D shadowMap;

void main()
{
	vec3 ambient, diffuse, specular;
	vec3 finalColor;

	vec3 norm = normalize(fragNormal);
	vec3 viewDir = normalize(cameraPosition - fragPosition);

	vec3 fragLightNDC = vec3(lightFragPosition.xyz/lightFragPosition.w);
	
	float flNDCx = (fragLightNDC.x + 1) / 2;
	float flNDCy = (fragLightNDC.y + 1) / 2;
	float flNDCz = (fragLightNDC.z + 1) / 2;

	vec4 depthValue = texture(shadowMap, vec2(flNDCx, flNDCy));
	float biasValue = max(0.05 * (1.0 - dot(fragNormal, directionalLightDirection)), 0.005);
	
	vec3 directionalLightDir = normalize(-directionalLightDirection);
	float directionalLightAmbience = 1.0f;
	vec3 directionalLightAmbient = directionalLightAmbience * lightAmbient * vec3(outColor);


	float cutOff = 0.91;
	float outerCutOff = 0.82f;
	vec3 spotLightDir = normalize(spotLightPosition - fragPosition);
	float spotAngle = dot(spotLightDir, normalize(-spotLightDirection));

	vec3 spotLightAmbient = sLightAmbient * vec3(outColor) * 1.0f;

	ambient = spotLightAmbient;

	finalColor = ambient * vec3(outColor);

	//if(depthValue.x >= flNDCz - 0.000005)
	//{
		//float directionalLightDiff = max(dot(norm, directionalLightDir), 0.0f);
		//vec3 directionalLightDiffuse = directionalLightDiff * lightDiffuse * vec3(outColor);

		//vec3 directionalLightReflectDir = reflect(-directionalLightDir, norm);

		//float directionalLightSpec = pow(max(dot(viewDir, directionalLightReflectDir), 0.0f), objectShine);
		//vec3 directionalLightSpecular = directionalLightSpec * lightSpecular * objectSpec;


		float spotLightDiff = max(dot(norm, spotLightDir), 0.0f);
		vec3 spotLightDiffuse = spotLightDiff * sLightDiffuse * vec3(outColor);

		vec3 spotLightReflectDir = reflect(-spotLightDir, norm);
		float spotLightSpec = pow(max(dot(viewDir, spotLightReflectDir), 0.0f), objectShine);
		vec3 spotLightSpecular = spotLightSpec * sLightSpecular * objectSpec;

		float outerAngle = cutOff - outerCutOff;
		float spotLightIntensity = clamp((spotAngle - outerCutOff) / outerAngle, 0.0f, 1.0f);

		spotLightDiffuse *= spotLightIntensity;
		spotLightSpecular *= spotLightIntensity;

		float spotLightDistance = length(spotLightPosition - fragPosition);
		
		diffuse = vec3(spotLightDiffuse);
		specular = vec3(spotLightSpecular);


		finalColor = (finalColor + diffuse + specular);

		// Pass the final color to our fragColor output variable
	//}

	fragColor = vec4(finalColor, 1.0);
}
