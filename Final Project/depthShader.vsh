#version 330

layout(location = 0) in vec3 vertexPosition;

uniform mat4 lightView;
uniform mat4 lightProjection;
uniform mat4 modelMatrix;

void main ()
{
    vec4 finalPosition = modelMatrix * vec4(vertexPosition, 1.0f);

    gl_Position = lightProjection * lightView * finalPosition;

}