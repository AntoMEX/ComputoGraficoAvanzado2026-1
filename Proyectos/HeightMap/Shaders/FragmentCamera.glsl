#version 460 core

//in vec4 vertexColor;

in vec2 texCoord;
out vec4 outColor;

uniform float time;

uniform sampler2D heightMap;
//uniform sampler2D texture1;

uniform float mixFactor;

void main()
{	
	outColor = texture(heightMap, texCoord);
}