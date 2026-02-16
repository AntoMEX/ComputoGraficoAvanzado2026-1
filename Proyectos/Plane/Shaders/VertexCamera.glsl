#version 460 core

layout (location = 0) in vec4 vPosition;
//layout (location = 1) in vec4 vColor;

//out vec4 vertexColor;
uniform float time;
uniform mat4 camera;
uniform mat4 projection;

float Change (float x, float z, float amp, float pha, float frec)
{
	return amp * cos(pha + frec * (x * x + z * z));
}

void main ()
{  		
	//vertexColor = vColor;

	vec4 newPosition = vPosition;
	//newPosition.z = newPosition.z + cos (time);

	float amplitude = 0.3;
	float phase = time;
	float frecuency = 15;

	newPosition.z = Change(newPosition.x, newPosition.y, amplitude, phase, frecuency);
	//newPosition.z = newPosition.z + 0.5 * cos (pow(newPosition.x, 2) + pow(newPosition.y,2) + time); //z = z + 0.5 * cos(x2 + y2 + time)
	
	newPosition = projection * camera * newPosition;
	
	gl_Position = newPosition;  //equivale a hacer return gl_Position	
	
}