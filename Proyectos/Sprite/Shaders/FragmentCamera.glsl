#version 460 core

//in vec4 vertexColor;

in vec2 texCoord;
out vec4 outColor;

uniform float time;

uniform sampler2D texture0;

void main()
{

	outColor = texture(texture0, texCoord); //implementar mix para la tarea de DoubleTexture
}