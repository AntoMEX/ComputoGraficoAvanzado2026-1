#include "Application.h"
#include <iostream>
#include "ShaderFuncs.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Application::setupGeometry()
{
	oPlane.createPlane(1);

	glGenVertexArrays(1, &oPlane.vao);
	glBindVertexArray(oPlane.vao);
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, oPlane.getVertexSizeInBytes() +
		oPlane.getTextureCoordsSizeInBytes(), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, oPlane.getVertexSizeInBytes(), oPlane.plane);

	glBufferSubData(GL_ARRAY_BUFFER, oPlane.getVertexSizeInBytes(),
		oPlane.getTextureCoordsSizeInBytes(), oPlane.textureCoords);

	oPlane.cleanMemory();

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(oPlane.getVertexSizeInBytes()));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnable(GL_DEPTH_TEST);
}

void Application::setupProgram2()
{
	std::string vertexShader = loadTextFile("shaders/VertexCamera.glsl");
	std::string fragmentShader = loadTextFile("shaders/FragmentCamera.glsl");
	ids["program2"] = InitializeProgram(vertexShader, fragmentShader);
	ids["time2"] = glGetUniformLocation(ids["program2"], "time");
	ids["model"] = glGetUniformLocation(ids["program2"], "model");
	ids["camera"] = glGetUniformLocation(ids["program2"], "camera");
	ids["projection"] = glGetUniformLocation(ids["program2"], "projection");
	ids["texture0"] = glGetUniformLocation(ids["program2"], "texture0");
	ids["texture1"] = glGetUniformLocation(ids["program2"], "texture1");
}

GLuint Application::setupTexture(const std::string& path)
{
	int width, height, channels;
	unsigned char* img = stbi_load(path.c_str(), &width, &height, &channels, 4);

	if (img == nullptr)
		return -1;
	//Mandamos al GPU
	GLuint textID = -1;
	glGenTextures(1, &textID);
	glBindTexture(GL_TEXTURE_2D, textID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

	stbi_image_free(img); //Liberamos la imagen cargada porque ya no la necesitamos
	//Como se va comportar
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //MIN_FILTER en lo más cercano (minificación) y GL_LINEAR se ve mejor, sin "bordes de sierra"
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //MAG_FILTER en lo más lejano (maxificación)

	//Trilinear
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Si se sale del tamaño de la imágen, GL_REPEAT hace que se repitan las texturas
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return textID;
}

//void Application::setTexture(GLuint texture)
//{
//
//}

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
	ids["Dragon"] = setupTexture("Textures/Dragon.png");
	ids["Dino"] = setupTexture("Textures/Dino.jpg");

	//projection = glm::perspective(45.0f, 1024.0f / 768.0f, 0.1f, 100.0f);
}

void Application::update()
{
	time += 0.009f;
	eye = glm::vec3(0.0f, 1.5f + cos(time), 1.5f + cos(time));
	center = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	model = glm::identity<glm::mat4>();
	camera = glm::lookAt(eye, center, up);
	projection = glm::perspective(glm::radians(45.0f), (1024.0f / 768.0f), 0.1f, 200.0f);

}

void Application::draw()
{
	//Seleccionar programa (shaders)
	glUseProgram(ids["program2"]);

	//Pasar el resto de los parámetros para el programa
	glUniform1f(ids["time2"], time);
	glUniformMatrix4fv(ids["model"], 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(ids["camera"],1 , GL_FALSE, &camera[0][0]);
	glUniformMatrix4fv(ids["projection"], 1, GL_FALSE, &projection[0][0]);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	//Seleccionar la geometria
	glBindVertexArray(oPlane.vao);

	//Seleccionar texturas
	glBindTexture(GL_TEXTURE_2D, ids["Dragon"]);
	glBindTexture(GL_TEXTURE_2D, ids["Dino"]);
	glUniform1i(ids["texture0"], 0);
	glUniform1i(ids["texture1"], 1);

	glActiveTexture(GL_TEXTURE0);

	//glDraw()
	glDrawArrays(GL_TRIANGLES, 0, oPlane.getNumVertex());
}
//imagen de dinosaurio: https://stock.adobe.com/fr/images/huge-green-dinosaur-monolophosaurus-hunting-8-bit-pixel-art-wild-animal-isolated-on-white-background-archeology-paleontology-science-symbol-ancient-reptile-monster-jurassic-period-creature-logo/226620387
// https://www.bing.com/images/search?q=dinosaurios%20502x502&qs=n&form=QBIR&sp=-1&lq=0&pq=dinosaurios%20502x502&sc=0-19&cvid=DB9488383C3F4838A73D59A899177394&first=1
