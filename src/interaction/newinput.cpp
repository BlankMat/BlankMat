#include "interaction/newinput.h"

// Handles keyboard key interactions
void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

// Handles keyboard character typing interactions
void Input::CharacterCallback(GLFWwindow* window, unsigned int codepoint)
{

}

// Handles mouse movement interactions
void Input::CursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{

}

// Handles mouse button interactions
void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

}

// Handles mouse scroll interactions
void Input::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{

}

// Handles changing the viewport size to match the window size
void Input::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}