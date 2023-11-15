#include "interaction/newinput.h"
#include <iostream>

// Returns whether alt is being pressed
bool Input::AltPress(int mod)
{
	return (mod & GLFW_MOD_ALT);
}

// Returns whether shift is being pressed
bool Input::ShiftPress(int mod)
{
	return (mod & GLFW_MOD_SHIFT);
}

// Returns whether ctrl is being pressed
bool Input::CtrlPress(int mod)
{
	return (mod & GLFW_MOD_CONTROL);
}

// Handles keyboard key interactions
void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << "Called KeyCallback, key " << key << ", scancode " << scancode << ", action " << action << ", mods " << mods << std::endl;
	if (key == GLFW_KEY_W)
	{
		//
	}
}

// Handles mouse movement interactions
void Input::CursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	std::cout << "Called CursorPositionCallback, xPos " << xPos << ", yPos " << yPos << std::endl;
}

// Handles mouse button interactions
void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	std::cout << "Called MouseButtonCallback, button " << button << ", action " << action << ", mods " << mods << std::endl;
	if (action == GLFW_PRESS)
	{

	}
	else if (action == GLFW_RELEASE)
	{

	}
}

// Handles mouse scroll interactions
void Input::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	std::cout << "Called ScrollCallback, xOffset " << xOffset << ", yOffset " << yOffset << std::endl;
}

// Handles changing the viewport size to match the window size
void Input::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}