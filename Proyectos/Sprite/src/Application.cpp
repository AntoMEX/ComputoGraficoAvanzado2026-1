#include "Application.h"
#include <iostream>
#include "ShaderFuncs.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glm/gtc/type_ptr.hpp"

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

	ids["sizeX"] = glGetUniformLocation(ids["program2"], "sizeX");
	ids["sizeY"] = glGetUniformLocation(ids["program2"], "sizeY");
	ids["index"] = glGetUniformLocation(ids["program2"], "index");
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
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //Máximo de calidad
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Si se sale del tamaño de la imágen (ver textureCoords en CreatePlane del Plane.cpp), GL_REPEAT hace que se repitan las texturas
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return textID;
}

void Application::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Teclas para mover el personaje en spritesheet
	if (action == GLFW_PRESS)
	{
		//Seleccionar estado según tecla y arrancar desde frame 0
		if (key == GLFW_KEY_D) //Movimiento
		{
			requestedState = STATE_MOVE;
		}
		else if (key == GLFW_KEY_SPACE) //Ataque
		{
			requestedState = STATE_ATTACK;
		}
		else if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) //Defensa
		{
			requestedState = STATE_DEFEND;
		}
		else
		{
			return;
		}

		//Activar estado
		currentState = requestedState;
		keyHeld = true;
		returnToIdle = false;
		animTime = 0.0f; //Empezar desde el inicio
		prevFrame = -1;
	}
	else if (action == GLFW_RELEASE)
	{
		//Si se suelta cualquiera de las teclas de acción, vuelve al idle
		if (key == GLFW_KEY_D || key == GLFW_KEY_SPACE || key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT)
		{
			keyHeld = false;
			//si esta en un estado de acción, vuelve a idle al terminar el ciclo
			if (currentState != STATE_IDLE)
				returnToIdle = true;
		}
	}
}

void Application::setup()
{
	setupGeometry();
	setupProgram2();
	ids["Caballero"] = setupTexture("Textures/Caballero.png");

	//projection = glm::perspective(45.0f, 1024.0f / 768.0f, 0.1f, 100.0f);
}

void Application::update()
{
	time += 0.009f;

	//Actualizar tiempo local
	animTime += 0.009f;

	//Calcular frame actual
	int framesPerRow = (int)sizeX;
	int frame = (int)(animTime * animFPS) % framesPerRow;

	//Detectar final de ciclo
	bool cycleCompleted = false;
	if (prevFrame != -1)
	{
		if (prevFrame == framesPerRow - 1 && frame == 0)
			cycleCompleted = true;
	}
	prevFrame = frame;

	//Decidir fila osea estado actual
	int row = 0;
	if (currentState == STATE_IDLE) row = 0;
	else if (currentState == STATE_MOVE) row = 1;
	else if (currentState == STATE_ATTACK) row = 2;
	else if (currentState == STATE_DEFEND) row = 3;

	//Asignar index (columna = frame, fila = row)
	index = glm::vec2((float)frame, (float)row);

	//Si la tecla está siendo sostenida seguir en el mismo estado haciendo loop o si la tecla fue soltada termina el ciclo y vuelve a idle
	if (!keyHeld && returnToIdle)
	{
		if (cycleCompleted)
		{
			//Terminar ciclo y volver a idle
			currentState = STATE_IDLE;
			requestedState = STATE_IDLE;
			returnToIdle = false;
			animTime = 0.0f;
			prevFrame = -1;
		}
	}

	eye = glm::vec3(0.0f, 2.5f, 0.0f);
	center = glm::vec3(0.01f, 0.01f, 0.01f);
	glm::vec3 up = glm::vec3(0.0f, 0.0f, -1.0f);
	model = glm::identity<glm::mat4>();
	camera = glm::lookAt(eye, center, up);
	projection = glm::perspective(glm::radians(45.0f), (1024.0f / 768.0f), 0.1f, 200.0f);

	//index = glm::vec2(1.0f, 1.0f);
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

	glUniform1f(ids["sizeX"], sizeX);
	glUniform1f(ids["sizeY"], sizeY);
	glUniform2f(ids["index"], index.x, index.y);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	//Seleccionar la geometria
	glBindVertexArray(oPlane.vao);

	//Seleccionar texturas
	glBindTexture(GL_TEXTURE_2D, ids["Caballero"]);
	glUniform1i(ids["texture0"], 0);
	glActiveTexture(GL_TEXTURE0);

	//glDraw()
	glDrawArrays(GL_TRIANGLES, 0, oPlane.getNumVertex());
}
//página de spritesheets: https://artpictures.club/autumn-2023.html
//spritesheet de caballero recuperada de https://ar.inspiredpencil.com/pictures-2023/sprite-sheet-png