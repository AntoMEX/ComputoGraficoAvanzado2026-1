#version 460 core

layout (location = 0) in vec4 vPosition;
//layout (location = 1) in vec4 vColor;

//out vec4 vertexColor;
uniform float time;
uniform mat4 camera;
uniform mat4 projection;
uniform mat4 model;  

struct Light
{
    vec3 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct Material 
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    int shininess;
};

uniform Light light;
uniform Material material;

vec3 normal = vec3(0.0,0.0,1.0);

uniform vec3 eye;

//Datos para pasar al fragment shader
out vec3 fragPos;   //Posición del vértice en espacio mundo
out vec3 fragNormal; //Normal transformada

float Change (float x, float z, float amp, float pha, float frec)
{
	return amp * cos(pha + frec * (x * x + z * z));
}

void main ()
{          
    //modelview
    mat4 modelview = camera * model;  

    //Posición transformada
    vec4 newPosition = modelview * vPosition;

    float amplitude = 0.3;
	float phase = time;
	float frecuency = 15;

    newPosition.z = Change(newPosition.x, newPosition.y, amplitude, phase, frecuency);

    //Posición para cálculos en fragment
    fragPos = vec3(model * vPosition);

    //Matriz de normales
    mat3 normalMatrix = transpose(inverse(mat3(model)));

    //Normal transformada
    fragNormal = normalize(normalMatrix * normal);

    gl_Position = projection * modelview * newPosition;
}




//Receta para el calculo de normales y demas para Gouraud Shading (iluminación por vertice)
//
////Obtenemos la geometria que queremos (ondas)
//mat4 modelview = view * model; //Precalculamos modelview
//vec4 newPosition = vPosition; //copia de vPosition (vertice original para poderlo manipular)
//newPosition.y = Func(vPosition.x, vPosition.z, ampliitud, fase, frecuencia); //Altura del vertice (funcion de las ondas)
//
////Calculamos la normal para la sabana con las ondas
//vec3 Vtx = vec3(1, DyDx (newPosition.x, newPosition.z), 0); //vector tangente en x
//vec3 Vtz = vec3(0, DyDz (newPosition.x, newPosition.z), 1); //vector tangente en y
//
//vec3 normal = cross(Vtz, Vtx); //Calculamos la normal en las ondas sin transformaciones
//
////Ahora aplicamos transformaciones
//mat4 matForNormal = transpose(inverse(modelview)); //Calculamos la matriz de transformacion para las normales
//vec3 newNormal = (matForNormal * vec4(normal //Calculamos la normal transdormada
//newPosition = modelview * newPosition; //Aplicamos las transformaciones a las ondas
//
////Calculamos ADS
//outcolor = ADS(newPosition, newNormal, .......); //Calculamos con todo ya transformado
//
//Emitimos vertice
//gl_Position = projection * newPosition; //newPosition ya incluye el modelview



//newPosition debe ser una variable out para llevarla al fragment shader
//newNormal igual