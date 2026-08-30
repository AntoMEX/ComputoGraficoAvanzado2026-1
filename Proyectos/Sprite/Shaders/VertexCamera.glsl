#version 460 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec2 vTexCoord;

uniform float time;
uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

//Tamaño de spritesheet
uniform float sizeX; 
uniform float sizeY; 

//Indice de sprite
uniform vec2 index; 
//Nota dependiendo de que index (indice) se le de coordenada (0,0) (hasta el 3) sera lo que se le va a sumar a cada 
//coordenada de index mediante los framesize, osea si es (2,3), se le multiplicará a framesizeX * 3 para que ya no 
//sea 0.25 si no 0.75 y al framesizeY * 4 para que ya no sea 0.25 si no 1.0 y se vea el cuadro que en el spritesheet 
//esta en (3,4) (un frame de ataque)


out vec2 texCoord;

void main ()
{  		
	//texCoord = vTexCoord;

	//4*4 de sprites
	float framesizeX = 1.0 / sizeX; //x
	float framesizeY = 1.0 / sizeY; //y
	//el espacio de frame es de 0.25

	//Pasar a otros sprites
	//texCoord.s+= 0.25f; //siguiente en x
	//texCoord.t+= 0.25f; //siguiente en y
	
	vec2 offset = vec2 (index.x * framesizeX, index.y * framesizeY); 

	texCoord = vTexCoord * vec2(framesizeX, framesizeY) + offset;



	vec4 newPosition = projection * camera * model * vPosition;
	
	gl_Position = newPosition;  //equivale a hacer return gl_Position
}