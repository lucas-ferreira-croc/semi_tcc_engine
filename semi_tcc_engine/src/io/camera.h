#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class cameraDirection
{
	NONE = 0,
	FOWARD,
	BACKWARD,
	RIGHT,
	LEFT,
	UP,
	DOWN
};

class camera 
{
private:
	void updateCameraVectors();

	float yaw;
	float pitch;
	float speed;
	float zoom;

public:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;

	glm::vec3 worldUp;


	camera(glm::vec3 position);

	void updateCameraDirection(double dx, double dy);
	void updateCameraPos(cameraDirection camDir, double dt);
	void updateCameraZoom(double dy);

	glm::mat4 getViewMatrix();
	float getYaw();
	float getPitch();
	float getSpeed();
	float getZoom();
};

#endif // !CAMERA_H
