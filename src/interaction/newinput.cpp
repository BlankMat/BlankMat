#include "interaction/newinput.h"
#include <iostream>

// Handles keyboard key interactions
void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Ignore Alt/Ctrl/Shift
	if (IsModKey(key))
		return;

	bool change = false;
	if (action == GLFW_PRESS)
		change = PressKey(GetKeyCode(key), mods);
	else if (action == GLFW_RELEASE)
		change = UnPressKey(GetKeyCode(key));

	if (change)
		std::cout << "Called KeyCallback " << GetKeyCode(key) << ", key " << key << ", scancode " << scancode << ", action " << action << ", mods " << mods << std::endl;
}

// Handles mouse button interactions
void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	bool change = false;
	if (action == GLFW_PRESS)
		change = PressKey(GetMouseCode(button), mods);
	else if (action == GLFW_RELEASE)
		change = UnPressKey(GetMouseCode(button));

	if (change)
		std::cout << "Called MouseButtonCallback " << GetMouseCode(button) << ", button " << button << ", action " << action << ", mods " << mods << std::endl;
}

// Handles mouse movement interactions
void Input::CursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	//std::cout << "Called CursorPositionCallback, xPos " << xPos << ", yPos " << yPos << std::endl;
}

// Handles mouse scroll interactions
void Input::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	//std::cout << "Called ScrollCallback, xOffset " << xOffset << ", yOffset " << yOffset << std::endl;
}

// Initializes the input module with the current hotkeys
Input::Input(Config* hotkeyConfig)
{

}