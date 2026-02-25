#version 460 core

//in vec4 vertexColor;

in vec2 texCoord;
out vec4 outColor;

uniform float time;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform float mixFactor;

void main()
{
	//outColor = vertexColor;
	//outColor = vec4(1.0,1.0,1.0,1.0);

	vec4 textureA = texture(texture0, texCoord);
	vec4 textureB = texture(texture1, texCoord);

	outColor = mix(textureA, textureB, mixFactor); //implementar mix para la tarea de DoubleTexture
	
}