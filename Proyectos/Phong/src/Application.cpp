#include "Application.h"
#include <iostream>
#include "ShaderFuncs.h"
#include "glm/gtc/type_ptr.hpp"

void Application::setupGeometry()
{
	int n = 10;

	std::vector<glm::vec4> positions = plane.createPlane(n);

	//Crear VAO
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	ids["plane"] = VAO;

	glBindVertexArray(VAO);

	//crear VBO vertices
	glGenBuffers(1, &VBO); //Puede haber varios vbo en un vao

	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //Ojo esto todavia no ha reservado memoria
	//Pasar arreglo de vertices

	glBufferData(GL_ARRAY_BUFFER,
		sizeof(GLfloat) * positions.size() * 4,  //calculo de tamaño en bytes IMPORTANTE: Se le pone 4 para que salga toda la malla
		positions.data(),
		GL_STATIC_DRAW);  //Mandamos la geometria al buffer

	//vertices
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//Colores

	glVertexAttrib4f(1, 1.0f, 1.0f, 1.0f, 1.0f);
}

void Application::setupProgram2()
{
	std::string vertexShader = loadTextFile("shaders/VertexCamera.glsl");
	std::string fragmentShader = loadTextFile("shaders/FragmentCamera.glsl");
	ids["program2"] = InitializeProgram(vertexShader, fragmentShader);
	ids["time2"] = glGetUniformLocation(ids["program2"], "time");
	ids["camera"] = glGetUniformLocation(ids["program2"], "camera");
	ids["projection"] = glGetUniformLocation(ids["program2"], "projection");

	ids["material.ambient"] = glGetUniformLocation(ids["program2"], "material.ambient");
	ids["material.diffuse"] = glGetUniformLocation(ids["program2"], "material.diffuse");
	ids["material.specular"] = glGetUniformLocation(ids["program2"], "material.specular");
	ids["material.shininess"] = glGetUniformLocation(ids["program2"], "material.shininess");

	ids["light.position"] = glGetUniformLocation(ids["program2"], "light.position");
	ids["light.ambient"] = glGetUniformLocation(ids["program2"], "light.ambient");
	ids["light.diffuse"] = glGetUniformLocation(ids["program2"], "light.diffuse");
	ids["light.specular"] = glGetUniformLocation(ids["program2"], "light.specular");

	ids["model"] = glGetUniformLocation(ids["program2"], "model");
	ids["eye"] = glGetUniformLocation(ids["program2"], "eye");
}

void Application::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//teclas para mover	
}

void Application::setup()
{
	setupGeometry();
	setupProgram2();
	projection = glm::perspective(45.0f, 1024.0f / 768.0f, 0.1f, 100.0f);

	light.position = glm::vec3(0.0f, 0.0f, 2.0f);
	light.ambient = glm::vec4(1.0f, 0.1f, 0.1f, 1.0f);
	light.diffuse = glm::vec4(0.1f, 1.0f, 0.1f, 1.0f);
	light.specular = glm::vec4(0.1f, 0.1f, 1.0f, 1.0f);

	material.ambient = glm::vec4(0.1f, 0.1f, 1.0f, 1.0f);
	material.diffuse = glm::vec4(0.1f, 0.1f, 1.0f, 1.0f);
	material.specular = glm::vec4(0.1f, 0.1f, 1.0f, 1.0f);
	material.shininess = 1;

	model = glm::mat4(1.0f);
}

void Application::update()
{
	time += 0.01f;
	eye = glm::vec3(0.0f, -2.0f, 2.0f);
	camera = glm::lookAt(eye, center, glm::vec3(0.0f, 1.0f, 0.0f));
	light.position = glm::vec3(cos(time), sin(time), 1.0f);
}

void Application::draw()
{
	//Seleccionar programa (shaders)
	glUseProgram(ids["program2"]);

	//Pasar el resto de los parámetros para el programa
	glUniform1f(ids["time2"], time);
	glUniformMatrix4fv(ids["camera"], 1, GL_FALSE, &camera[0][0]);
	glUniformMatrix4fv(ids["projection"], 1, GL_FALSE, &projection[0][0]);

	glUniform3fv(ids["light.position"], 1, glm::value_ptr(light.position));
	glUniform4fv(ids["light.ambient"], 1, glm::value_ptr(light.ambient));
	glUniform4fv(ids["light.diffuse"], 1, glm::value_ptr(light.diffuse));
	glUniform4fv(ids["light.specular"], 1, glm::value_ptr(light.specular));

	glUniform1i(ids["material.shininess"], material.shininess);
	glUniform4fv(ids["material.ambient"], 1, glm::value_ptr(material.ambient));
	glUniform4fv(ids["material.diffuse"], 1, glm::value_ptr(material.diffuse));
	glUniform4fv(ids["material.specular"], 1, glm::value_ptr(material.specular));

	glUniformMatrix4fv(ids["model"], 1, GL_FALSE, glm::value_ptr(model));

	glUniform3fv(ids["eye"], 1, glm::value_ptr(eye));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	//Seleccionar la geometria
	glBindVertexArray(ids["plane"]);

	//glDraw()
	glDrawArrays(GL_TRIANGLES, 0, plane.vertexSize);
}
