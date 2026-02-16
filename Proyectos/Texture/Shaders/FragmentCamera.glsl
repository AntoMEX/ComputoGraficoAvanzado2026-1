#version 460 core

//in vec4 vertexColor;

in vec2 texCoord;
out vec4 outColor;

uniform float time;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	//outColor = vertexColor;
	//outColor = vec4(1.0,1.0,1.0,1.0);
	outColor = texture(texture0, texCoord); //implementar mix
}