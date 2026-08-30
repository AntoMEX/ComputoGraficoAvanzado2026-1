#pragma once
#include <vector>
#include <map>
#include "glad/glad.h"
#include <string>
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Plane.h"

class Application
{
	
private:
	
	std::map<std::string, GLuint> ids;

	void setupGeometry();
	void setupProgram2();
	GLuint setupTexture(const std::string&path);
	float time{ 0.0f };
	glm::mat4 camera;
	glm::mat4 projection;
	glm::mat4 model;
	glm::vec3 eye{0.0f, 0.0f, 2.0f};
	glm::vec3 center{0.1f, 0.1f, 0.5f};

	float sizeX = 4.0f;
	float sizeY = 4.0f;
	glm::vec2 index;

	Plane oPlane;

	//Maquina de estados
	enum SpriteState { STATE_IDLE = 0, STATE_MOVE = 1, STATE_ATTACK = 2, STATE_DEFEND = 3 };
	
	SpriteState currentState = STATE_IDLE;  //Estado activo
	SpriteState requestedState = STATE_IDLE; //Estado solicitado por la tecla

	bool keyHeld = false; //Mientras la tecla esté presionada es True y se queda en loop
	bool returnToIdle = false; //Si se suelta la tecla es True y vuelve a idle al terminar ciclo

	float animTime = 0.0f; //Tiempo local de animación
	int prevFrame = -1; //Para detectar final de ciclo
	float animFPS = 6.0f; //Frames por segundo de la animación

public:

	GLFWwindow* window;
	void setup();
	void update();
	void draw();
	void keyCallback(int key, int scancode, int action, int mods);

};