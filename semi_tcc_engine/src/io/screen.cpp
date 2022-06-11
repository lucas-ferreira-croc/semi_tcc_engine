#include "screen.h"

#include "keyboard.h"
#include "mouse.h"

unsigned int screen::SCR_WIDTH = 800;
unsigned int screen::SCR_HEIGHT = 600;

screen::screen()
	: window(nullptr)
{
}


void screen::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}


bool screen::init(std::string name)
{
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, name.c_str(), NULL, NULL);
	if (!window) {
		return false;
	}

	glfwMakeContextCurrent(window);
	return true;
}

void screen::setParameters()
{
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glfwSetFramebufferSizeCallback(window, screen::framebufferSizeCallback);

	glfwSetKeyCallback(window, keyboard::keyCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(window, mouse::cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, mouse::mouseWheelCallback);

	glEnable(GL_DEPTH_TEST);
}

void screen::update()
{
	glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void screen::newFrame()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool screen::shouldClose()
{
	return glfwWindowShouldClose(window);
}

void screen::setShouldClose(bool shouldClose)
{
	glfwSetWindowShouldClose(window, shouldClose);
}
