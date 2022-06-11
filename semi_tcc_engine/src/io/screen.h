#ifndef SCREEN_H
#define SCREEN_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class screen
{
private:
	GLFWwindow* window;
public:
	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGHT;

	screen();

	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	

	bool init(std::string screen_name);

	void setParameters();

	//main loop

	void update();
	void newFrame();

	bool shouldClose();
	void setShouldClose(bool shouldClose);
};

#endif // !SCREEN_H
