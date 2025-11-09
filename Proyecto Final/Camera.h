#pragma once

#include <glew.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include <glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	// modo seguimiento
	void establecerObjetivoSeguimiento(glm::vec3* posicionObjetivo, glm::vec3* frenteObjetivo, float distanciaOffset = 5.0f, float altura = 2.0f);
	void establecerModoLibre();
	bool estaSiguiendo() const { return modoSeguimiento; }
	void ajustarAngulosSeguimiento(float deltaGuiñada, float deltaInclinacion);


	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;


	// Variables para modo seguimiento
	bool modoSeguimiento = false;
	glm::vec3* posicionObjetivo = nullptr;
	glm::vec3* frenteObjetivo = nullptr;
	float offsetSeguimiento = 5.0f;
	float alturaSeguimiento = 2.0f;
	float guiñadaSeguimiento = 0.0f;
	float inclinacionSeguimiento = 20.0f;
	const float toRadians = 3.14159265f / 180.0f;



	void update();
};

