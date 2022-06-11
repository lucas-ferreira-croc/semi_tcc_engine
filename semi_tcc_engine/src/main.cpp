#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "graphics/shader.h"
#include "graphics/mesh.h"
#include "graphics/models/cube.hpp"

#include "io/screen.h"
#include "io/keyboard.h"
#include "io/mouse.h"
#include "io/joystick.h"
#include "io/camera.h"

void process_input(double deltaTime);

float mixRatio = 0.5f;
float FOV = 45.0f;
glm::mat4 transform = glm::mat4(1.0f);

camera cameras[2] = {
	camera(glm::vec3(0.0f, 0.0f, 3.0f)),
	camera(glm::vec3(5.0f, 5.0f, 5.0f))
};
	
int active_camera = 0;
double deltaTime = 0.0f;
double lastFrame = 0.0f;

joystick mainJ(0);
screen scr;

unsigned int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef  __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__ 

	if (!scr.init("semi_tcc_engine")) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	scr.setParameters();

	glViewport(0, 0 , SCREEN_WIDTH, SCREEN_HEIGHT);
	
	/*
		shaders
	*/

	shader shader_program("assets/object.vs", "assets/object.fs");

	// vertex data
	cube cube(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.85f));
	cube.init();

	mainJ.update();
	if (mainJ.isPresent()) {
		std::cout << mainJ.getName() << " is present" << std::endl;
	}
	else {
		std::cout << "joystick not present" << std::endl;
	}

	while(!scr.shouldClose()){
		
		double currentTime = glfwGetTime();
		deltaTime = currentTime  - lastFrame;
		lastFrame = currentTime;

		//process input
		process_input(deltaTime);

		//rendering
		scr.update();
		// create transformation matrix
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		
		//view = glm::translate(view, glm::vec3(-x, -y, -z));
		view = cameras[active_camera].getViewMatrix();
		projection = glm::perspective(glm::radians(cameras[active_camera].getZoom()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.f);

		// first triangle
		shader_program.activate();
		shader_program.setFloat("mixRatio", mixRatio);
		shader_program.setMat4("view", view);
		shader_program.setMat4("projection", projection);

		cube.render(shader_program);

		// send new buffer to window
		scr.newFrame();
	}

	glfwTerminate();
	return 0;
}

void process_input(double deltaTime){
	if (keyboard::key(GLFW_KEY_ESCAPE) || mainJ.buttonState(GLFW_JOYSTICK_DPAD_RIGHT))
		scr.setShouldClose(true);

	// change mixRatio

	if(keyboard::key(GLFW_KEY_UP))
	{
		if (mixRatio >= 1.0f)
			mixRatio = 1.0f;
		else
			mixRatio += 0.05f;
	}

	if (keyboard::key(GLFW_KEY_DOWN))
	{
		if (mixRatio <= 0.0f)
			mixRatio = 0.0f;
		else
			mixRatio -= 0.05f;
	}
	
	// move Camera

	if (keyboard::key(GLFW_KEY_W)) {
		cameras[active_camera].updateCameraPos(cameraDirection::FOWARD, deltaTime);
	}
	
	if (keyboard::key(GLFW_KEY_S)) {
		cameras[active_camera].updateCameraPos(cameraDirection::BACKWARD, deltaTime);
	}
	
	if (keyboard::key(GLFW_KEY_D)) {
		cameras[active_camera].updateCameraPos(cameraDirection::RIGHT, deltaTime);
	}
	
	if (keyboard::key(GLFW_KEY_A)) {
		cameras[active_camera].updateCameraPos(cameraDirection::LEFT, deltaTime);
	}

	if (keyboard::key(GLFW_KEY_SPACE)) {
		cameras[active_camera].updateCameraPos(cameraDirection::UP, deltaTime);

	}

	if (keyboard::key(GLFW_KEY_LEFT_SHIFT)) {
		cameras[active_camera].updateCameraPos(cameraDirection::DOWN, deltaTime);
	}

	if (keyboard::keyWentUp(GLFW_KEY_TAB)) {
		active_camera += (active_camera == 0) ? 1 : -1;
	}


	double dx = mouse::getDX();
	double dy = mouse::getDY();

	if (dx != 0 || dy != 0) {
		cameras[active_camera].updateCameraDirection(dx, dy);
	}

	double scrollDy = mouse::getScrollDY();

	if (scrollDy != 0) {
		cameras[active_camera].updateCameraZoom(scrollDy);
	}
}
