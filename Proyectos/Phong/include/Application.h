#pragma once
#include <vector>
#include <map>
#include "glad/glad.h"
#include <string>
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Plane.h"

typedef struct 
{
	glm::vec3 position;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
} Light;

typedef struct 
{
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	int shininess;
} Material;

class Application
{
private:
	
	std::map<std::string, GLuint> ids;

	void setupGeometry();
	void setupProgram2();
	float time{ 0.0f };
	glm::mat4 camera;
	glm::mat4 projection;
	glm::vec3 eye{0.0f, 0.0f, 2.0f};
	glm::vec3 center{0.1f, 0.1f, 0.5f};

	glm::mat4 model;

	Plane plane;

	Light light;
	Material material;

public:

	GLFWwindow* window;
	void setup();
	void update();
	void draw();
	void keyCallback(int key, int scancode, int action, int mods);
};