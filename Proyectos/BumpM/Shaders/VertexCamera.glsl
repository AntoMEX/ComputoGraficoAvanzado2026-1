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

void main ()
{          
    //modelview
    mat4 modelview = camera * model;  

    //Posición transformada
    vec4 newPosition = modelview * vPosition;

    //Posición para cálculos en fragment
    fragPos = vec3(model * vPosition);

    //Matriz de normales
    mat3 normalMatrix = transpose(inverse(mat3(model)));

    //Normal transformada
    fragNormal = normalize(normalMatrix * normal);

    gl_Position = projection * newPosition;
}