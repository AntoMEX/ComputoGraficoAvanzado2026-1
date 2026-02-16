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

vec3 normal = vec3(0.0,0.0,1.0); //N = Normal del vertice, por ahora normalizada por mi en z



uniform vec3 eye; //E = Eye viene de C++ (Application) por medio de uniform

//ADS
vec4 Ambient (struct Light light, struct Material material)
{
	return light.ambient * material.ambient; //A = La * Ma
}

vec4 Diffuse (struct Light light, struct Material material, vec3 normal, vec3 lightV)
{
	
	return light.diffuse * material.diffuse * dot(lightV, normal); //D = Ld * Md * producto punto (light position y normal)
}

vec4 Specular (struct Light light, struct Material material, vec3 eye, vec3 reflLight, vec3 viewV, vec3 lightV) 
{
	vec4 specular = vec4( 0.0, 0.0, 0.0, 0.0 );

	if( dot(lightV, viewV) > 0.0)
	{
		specular = pow(max(0.0, dot(viewV,reflLight)),
		material.shininess)*material.specular*light.specular; 
	}

	return specular; //S = Ls * Ms * producto punto (R y E) al exponente SH
}

out vec4 NewADS; //Para salir del vertex camera hacia el fragment camera es un out

void main ()
{  		
	/*
	vertexColor = vColor;
	
	vec4 newPosition = vPosition;

	newPosition = projection * camera * newPosition;
	
	NewADS = Ambient(light, material) + Diffuse(light, material, normal) + Specular(light, material, reflLight, eye);
	*/

	mat4 modelview = camera * model; 

	vec4 newPosition = modelview * vPosition; //nueva posición

	vec3 lightV = normalize( light.position - newPosition.xyz);

	vec3 reflLight = reflect(-lightV, normal); //R = producto punto de light position y normal

	vec3 viewV  = normalize(- newPosition.xyz );

	mat4 normalMatrix = transpose(inverse(modelview)); //matriz de las normales

	vec4 tempnormal = vec4 (normal,0.0);

	tempnormal = normalMatrix * tempnormal;

	vec3 N = normalize(tempnormal.xyz); //normal de la matriz

	reflLight = reflect(-lightV, N); 

	vec4 A = Ambient(light, material); //Calculo de ADS nuevo
	vec4 D = Diffuse(light, material, N, lightV);
	vec4 S = Specular(light, material, eye, reflLight, viewV, lightV);

	NewADS = A+D+S;

	gl_Position = projection * newPosition;  //equivale a hacer return gl_Position	
	
}

/*
1 Multiplicar vposition por las transformaciones del modelview y guardar en newPosition. 
2 obtener la matriz para las normales. 
3 calcular la nueva normal con esa matriz. 
4 calcular el ADS usando la nueva normal y la posición con el modelview osea el newPosition. 
5 hacer outcolor = NewADS y el gl_position = projection * newPosition.
*/