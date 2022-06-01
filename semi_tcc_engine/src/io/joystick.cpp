#include "joystick.h"

joystick::joystick(int id)
	:id(getId(id))
{
	update();
}

void joystick::update()
{
	present = glfwJoystickPresent(id);

	if (present) {
		name = glfwGetJoystickName(id);
		axes = glfwGetJoystickAxes(id, &axesCount);
		buttons = glfwGetJoystickButtons(id, &buttonCount);
	}
}

float joystick::axesState(int axis)
{
	return present ? axes[axis] : 0.0f;
}

unsigned char joystick::buttonState(int button)
{
	return present ? buttons[button] : GLFW_RELEASE;
}

int joystick::getAxesCount()
{
	return axesCount;
}

int joystick::getButtonCount()
{
	return buttonCount;
}

bool joystick::isPresent()
{
	return present;
}

const char* joystick::getName()
{
	return name;
}

int joystick::getId(int id)
{
	return GLFW_JOYSTICK_1 + id;
}
