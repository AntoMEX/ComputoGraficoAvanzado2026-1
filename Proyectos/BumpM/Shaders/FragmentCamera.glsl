#version 460 core

out vec4 outColor;
uniform float time;

in vec3 fragPos;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

in vec2 texCoord;
in mat4 normalMatrix;

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
Material material;
uniform vec3 eye;

//ADS de fragmentshader
vec4 Ambient (struct Light light, struct Material material)
{
    return light.ambient * material.ambient;
}

vec4 Diffuse (struct Light light, struct Material material, vec3 N, vec3 L)
{
    return light.diffuse * material.diffuse * max(dot(L, N), 0.0);
}

vec4 Specular (struct Light light, struct Material material, vec3 R, vec3 V)
{
    return light.specular * material.specular * pow(max(dot(R, V), 0.0), float(material.shininess));
}

void main()
{
    material.diffuse = texture(diffuseMap, texCoord);
    material.ambient = material.diffuse;
    material.specular = material.diffuse;
    material.shininess = 2;

    vec3 normal = texture(normalMap, texCoord).xyz;

    //Normal transformada
    vec3 fragNormal = normalize(normalMatrix * vec4 (normal,0)).xyz;

    vec3 L = normalize(light.position - fragPos);

    vec3 V = normalize(eye - fragPos);

    vec3 R = reflect(-L, normalize(fragNormal));

    //ADS
    vec4 A = Ambient(light, material);
    vec4 D = Diffuse(light, material, fragNormal, L);
    vec4 S = Specular(light, material, R, V);

    outColor = A + D + S;
}