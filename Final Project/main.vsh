#version 330

// Vertex attributes as inputs
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 vertexNormal;

out vec2 outUV;
out vec3 outColor;
out vec3 fragPosition;
out vec3 fragNormal;
out vec4 lightFragPosition;


uniform mat4 camera;
uniform mat4 perspective;
uniform mat4 modelMatrix;
uniform mat4 lightProjection, lightView;

void main()
{

	vec4 finalPosition = modelMatrix * vec4(vertexPosition, 1.0);
	
	fragPosition = vec3(finalPosition);
	
	fragNormal = mat3(transpose(inverse(modelMatrix)))* vertexNormal;
	gl_Position = perspective * camera * finalPosition;
	lightFragPosition = lightProjection * lightView * finalPosition;

	// We pass the color of the current vertex to our output variable
	outUV = vertexUV;
	outColor = vertexColor;
}
