#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include "shader/shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

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

	GLFWwindow* window = glfwCreateWindow(800, 600, "semi_tcc_engine", NULL, NULL);
	if(window == NULL)
	{
		std::cout << "Failed to create GLFW Windows" << std::endl;
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

	glViewport(0, 0 ,  800, 600);
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/*
		shaders
	*/

	shader shader_program("assets/vertex_shader.glsl", "assets/fragment_shader.glsl");
	shader shader_program2("assets/vertex_shader.glsl", "assets/fragment_shader2.glsl");

	// vertex data

	float square_vertices[] = {

		// positions         // colors
		-0.5f,  0.5f, 0.0f,	 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  1.0f, 0.6f, 0.0f

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
	glGenBuffers(1, &EBO);

	// bind VAO
	glBindVertexArray(VAO);

	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), square_vertices, GL_STATIC_DRAW);

	// set attribute pointer
	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// set up EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(square_indices), square_indices, GL_STATIC_DRAW);

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	shader_program.activate();
	shader_program.setMat4("transform", transform);

	glm::mat4 transform2 = glm::mat4(1.0f);
	transform2 = glm::scale(transform2, glm::vec3(0.5f));
	transform2 = glm::rotate(transform2, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	shader_program2.activate();
	shader_program2.setMat4("transform", transform);
	while(!glfwWindowShouldClose(window)){
		//process input
		process_input(window);

		//rendering
		glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		transform = glm::rotate(transform,  glm::radians((float)glfwGetTime() / 100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		transform2 = glm::rotate(transform2, glm::radians((float)glfwGetTime() / -100.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		shader_program.setMat4("transform", transform);
		shader_program2.activate();
		shader_program2.setMat4("transform", transform2);

		// draw call
		glBindVertexArray(VAO);

		// first triangle
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		shader_program.activate();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		shader_program2.activate();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*) (3 * sizeof(unsigned int)));

		// second triangle
		//glUseProgram(shaderPrograms[0]);
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*) (3 * sizeof(unsigned int)));

		// send new buffer to window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}


void process_input(GLFWwindow* window){
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
