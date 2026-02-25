#version 460 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec2 vTexCoord;

uniform float time;
uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

out vec2 texCoord;

void main ()
{  		
	texCoord = vTexCoord;

	vec4 newPosition = projection * camera * model * vPosition;
	
	gl_Position = newPosition;  //equivale a hacer return gl_Position	
	
}