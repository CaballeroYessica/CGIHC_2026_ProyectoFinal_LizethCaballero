#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}


// MODO SEGUIMIENTO
void Camera::establecerObjetivoSeguimiento(glm::vec3* posicionObjetivo, glm::vec3* frenteObjetivo, float distanciaOffset, float altura)
{
	modoSeguimiento = true;
	this->posicionObjetivo = posicionObjetivo;
	this->frenteObjetivo = frenteObjetivo;
	offsetSeguimiento = distanciaOffset;
	alturaSeguimiento = altura;
	guiñadaSeguimiento = 0.0f;
	inclinacionSeguimiento = 20.0f; // Ángulo ligeramente hacia abajo
}

void Camera::establecerModoLibre()
{
	modoSeguimiento = false;
	posicionObjetivo = nullptr;
	frenteObjetivo = nullptr;
}

void Camera::ajustarAngulosSeguimiento(float deltaGuiñada, float deltaInclinacion)
{
	if (modoSeguimiento) {
		guiñadaSeguimiento += deltaGuiñada;
		inclinacionSeguimiento += deltaInclinacion;

		// Limitar la inclinación para evitar volteos
		if (inclinacionSeguimiento > 89.0f) inclinacionSeguimiento = 89.0f;
		if (inclinacionSeguimiento < -89.0f) inclinacionSeguimiento = -89.0f;
	}
}

glm::mat4 Camera::calculateViewMatrix()
{
	if (modoSeguimiento && posicionObjetivo && frenteObjetivo) {
		float guiñadaRad = guiñadaSeguimiento * (3.14159265f / 180.0f);
		float inclinacionRad = inclinacionSeguimiento * (3.14159265f / 180.0f);

		glm::vec3 direccionAtras = (*frenteObjetivo);

		float distanciaHorizontal = offsetSeguimiento * cos(inclinacionRad);
		float distanciaVertical = offsetSeguimiento * sin(inclinacionRad);

		position = glm::vec3(
			posicionObjetivo->x + direccionAtras.x * distanciaHorizontal,
			posicionObjetivo->y + alturaSeguimiento + distanciaVertical,
			posicionObjetivo->z + direccionAtras.z * distanciaHorizontal
		);

		front = glm::normalize(*posicionObjetivo - position);

		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));

		yaw = glm::degrees(atan2(front.z, front.x));
		pitch = glm::degrees(asin(front.y));
	}

	return glm::lookAt(position, position + front, up);
}


//glm::mat4 Camera::calculateViewMatrix()
//{
//	return glm::lookAt(position, position + front, up);
//}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}


glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


Camera::~Camera()
{
}
