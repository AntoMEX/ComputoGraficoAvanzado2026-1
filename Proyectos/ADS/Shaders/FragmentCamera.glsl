#version 460 core

//in vec4 vertexColor;
out vec4 outColor;
uniform float time;

in vec4 NewADS; //ADS que viene desde el vertex camera

void main()
{
	//outColor = vertexColor;
	//outColor = vec4(1.0,1.0,1.0,1.0); //Color blanco

	outColor = NewADS;
}