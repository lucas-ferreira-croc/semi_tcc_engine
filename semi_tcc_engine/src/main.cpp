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

#include "shader/shader.h"

#include "io/keyboard.h"
#include "io/mouse.h"
#include "io/joystick.h"
#include "io/camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window, double deltaTime);

float mixRatio = 0.5f;
float FOV = 45.0f;
glm::mat4 transform = glm::mat4(1.0f);

camera cameras[2] = {
	camera(glm::vec3(0.0f, 0.0f, 3.0f)),
	camera(glm::vec3(5.0f, 5.0f, 5.0f))
};
	
int active_camera = 1;
double deltaTime = 0.0f;
double lastFrame = 0.0f;

joystick mainJ(0);


unsigned int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;
float x , y , z;

int main() {
	int success;
	char infoLog[512];

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef  __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__ 

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "semi_tcc_engine", NULL, NULL);
	if(window == NULL)
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0 , SCREEN_WIDTH, SCREEN_HEIGHT);
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, keyboard::keyCallback);
	
	glfwSetCursorPosCallback(window, mouse::cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, mouse::mouseWheelCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glEnable(GL_DEPTH_TEST);

	/*
		shaders
	*/

	shader shader_program("assets/vertex_shader.glsl", "assets/fragment_shader.glsl");
	shader shader_program2("assets/vertex_shader.glsl", "assets/fragment_shader2.glsl");

	// vertex data

	float cube_vertices[] = {

		// position           // texture coord 
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	float square_vertices[] = {

		// positions         // colors			// texture coord
		-0.5f,  0.5f, 0.0f,	 1.0f, 0.0f, 1.0f,  0.0f, 1.0f,       // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,       // bottom left
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,       // top right
		 0.5f, -0.5f, 0.0f,  1.0f, 0.6f, 0.0f,  1.0f, 0.0f        // top left

	};

	float vertices[] = {

		-0.7f,  0.7f, 0.0f, 
		-0.7f, -0.7f, 0.0f,
		-0.2f, -0.7f, 0.0f,

		 0.7f,  0.7f, 0.0f,
		 0.7f, -0.7f, 0.0f,
		 0.2f, -0.7f, 0.0f
	};

	unsigned int square_indices[] = {
		0, 1, 2, // first triangle
		2, 3, 1 // second triangle
	};

	unsigned int indices[] = {
		0, 1, 2, // first triangle
		3, 4, 5 // second triangle
	};

	// VAO, VBO, EBO
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// bind VAO
	glBindVertexArray(VAO);

	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	// set up EBO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(square_indices), square_indices, GL_STATIC_DRAW);

	// set attribute pointer
	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	// texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// textures
	unsigned int textureOne, textureTwo;
	glGenTextures(1, &textureOne);
	glBindTexture(GL_TEXTURE_2D, textureOne);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// loag image

	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("assets/akiraposter.jpg", &width, &height, &channels, 0);

	if (data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glGenTextures(1, &textureTwo);
	glBindTexture(GL_TEXTURE_2D, textureTwo);
	data = stbi_load("assets/croc_studios.png", &width, &height, &channels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	shader_program.activate();
	shader_program.setInt("textureOne", 0);
	shader_program.setInt("textureTwo", 1);

	mainJ.update();
	if (mainJ.isPresent()) {
		std::cout << mainJ.getName() << " is present" << std::endl;
	}
	else {
		std::cout << "joystick not present" << std::endl;
	}

	x = 0.0f;
	y = 0.0f;
	z = 3.0f;

	while(!glfwWindowShouldClose(window)){
		
		double currentTime = glfwGetTime();
		deltaTime = currentTime  - lastFrame;
		lastFrame = currentTime;

		//process input
		process_input(window, deltaTime);

		//rendering
		glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureOne);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureTwo);

		// draw call
		glBindVertexArray(VAO);

		// create transformation matrix
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		
		model = glm::rotate(model, glm::radians(-55.0f) * (float)glfwGetTime(), glm::vec3(0.5f));
		//view = glm::translate(view, glm::vec3(-x, -y, -z));
		view = cameras[active_camera].getViewMatrix();
		projection = glm::perspective(glm::radians(cameras[active_camera].zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.f);

		// first triangle
		shader_program.activate();
		shader_program.setFloat("mixRatio", mixRatio);
		shader_program.setMat4("model", model);
		shader_program.setMat4("view", view);
		shader_program.setMat4("projection", projection);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*) (3 * sizeof(unsigned int)));


		// send new buffer to window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
}


void process_input(GLFWwindow* window, double deltaTime){
	if (keyboard::key(GLFW_KEY_ESCAPE) || mainJ.buttonState(GLFW_JOYSTICK_DPAD_RIGHT))
		glfwSetWindowShouldClose(window, true);

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
