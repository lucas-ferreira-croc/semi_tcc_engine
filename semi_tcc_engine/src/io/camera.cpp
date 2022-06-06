#include "camera.h"

camera::camera(glm::vec3 position)
	:cameraPos(position),
	worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	yaw(-90.0f),
	pitch(0.0f),
	speed(2.5f),
	zoom(45.0f),
	cameraFront(glm::vec3(0.0f, 0.0f, -1.0f))
{
	updateCameraVectors();
}

void camera::updateCameraVectors()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void camera::updateCameraDirection(double dx, double dy)
{
	yaw += dx;
	pitch += dy;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	updateCameraVectors();
}

void camera::updateCameraPos(cameraDirection camDir, double dt)
{
	float velocity = (float)dt * speed;
	
	switch(camDir)
	{
		case  cameraDirection::FOWARD:
			cameraPos += cameraFront * velocity;
			break;
		case cameraDirection::BACKWARD:
			cameraPos -= cameraFront * velocity;
			break;

		case cameraDirection::RIGHT:
			cameraPos += cameraRight * velocity;
			break;

		case cameraDirection::LEFT:
			cameraPos -= cameraRight * velocity;
			break;

		case cameraDirection::UP:
			cameraPos += cameraUp * velocity;
			break;

		case cameraDirection::DOWN:
			cameraPos -= cameraUp * velocity;
			break;
	}

}

void camera::updateCameraZoom(double dy)
{
	if (zoom >= 1.0f && zoom <= 45.0f) {
		zoom -= dy;
	}
	else if (zoom < 1.0f){
		zoom = 1.0f;
	}
	else {
		zoom = 45.0f;
	}
}

glm::mat4 camera::getViewMatrix()
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
