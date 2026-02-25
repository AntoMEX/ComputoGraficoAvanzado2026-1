#version 460 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec2 vTexCoord;

uniform float time;
uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

uniform sampler2D diffuseMap;
uniform float heightScale;

out vec2 texCoord;

void main ()
{  		
	texCoord = vTexCoord;

	//heightmap en el vertex shader
    vec4 h = texture(diffuseMap, vTexCoord);

    //Calcular luminosidad
    float L = 0.2126 * h.r + 0.7152 * h.g + 0.0722 * h.b;

    //Desplazar la posición en Y según L (0..1) y heightScale
    vec4 displaced = vPosition;
    displaced.y += L * heightScale;

	vec4 newPosition = projection * camera * model * displaced;
	
	gl_Position = newPosition;  //equivale a hacer return gl_Position
}