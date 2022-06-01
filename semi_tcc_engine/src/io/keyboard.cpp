#include "keyboard.h"

bool keyboard::keys[GLFW_KEY_LAST] = { 0 };
bool keyboard::keysChanged[GLFW_KEY_LAST] = { 0 };

void keyboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_RELEASE) {
		if (!keys[key]) {
			keys[key] = true;
		}
	}
	else {
		keys[key] = false;
		keysChanged[key] = action != GLFW_REPEAT;
	}
}

bool keyboard::key(int key)
{
	return keys[key];
}

bool keyboard::keyChanged(int key)
{
	bool ret = keysChanged[key];
	keysChanged[key] = false;
	return ret;
}

bool keyboard::keyWentUp(int key)
{
	return !keys[key] && keyChanged(key);
}

bool keyboard::keyWentDown(int key)
{
	return keys[key] && keyChanged(key);
}
