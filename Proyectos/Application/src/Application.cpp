#include "Application.h"
#include <iostream>
#include "ShaderFuncs.h"

void Application::setupGeometry()
{
	int n = 1;

	std::vector<glm::vec4> positions = plane.createPlane(n);

	//Crear VAO
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	ids["plane"] = VAO;

	glBindVertexArray(VAO);

	//crear VBO vertices
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //Ojo esto todavia no ha reservado memoria
	//Pasar arreglo de vertices

	glBufferData(GL_ARRAY_BUFFER,
		sizeof(GLfloat) * positions.size()*4,  //calculo de tamaño en bytes IMPORTANTE: Se le pone 4 para que salga toda la malla
		positions.data(),
		GL_STATIC_DRAW);  //Mandamos la geometria al buffer

	//vertices
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//Colores

	glVertexAttrib4f(1, 0.8f, 0.8f, 0.8f, 1.0f);
}

void Application::setupProgram2()
{
	std::string vertexShader = loadTextFile("shaders/VertexCamera.glsl");
	std::string fragmentShader = loadTextFile("shaders/FragmentCamera.glsl");
	ids["program2"] = InitializeProgram(vertexShader, fragmentShader);
	ids["time2"] = glGetUniformLocation(ids["program2"], "time");
	ids["camera"] = glGetUniformLocation(ids["program2"], "camera");
	ids["projection"] = glGetUniformLocation(ids["program2"], "projection");
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
}

void Application::update()
{
	time += 0.1f;
	eye = glm::vec3( 0.0f, 0.0f, 2.5f);
	camera = glm::lookAt(eye, center, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Application::draw()
{
	//Seleccionar programa (shaders)
	glUseProgram(ids["program2"]);

	//Pasar el resto de los parámetros para el programa
	glUniform1f(ids["time2"], time);
	glUniformMatrix4fv(ids["camera"],1 , GL_FALSE, &camera[0][0]);
	glUniformMatrix4fv(ids["projection"], 1, GL_FALSE, &projection[0][0]);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Seleccionar la geometria
	glBindVertexArray(ids["plane"]);

	//glDraw()
	glDrawArrays(GL_TRIANGLES, 0, plane.vertexSize);
}
