#include "interaction/newinput.h"

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

void Input::CharacterCallback(GLFWwindow* window, unsigned int codepoint)
{

}

void Input::CursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{

}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

}

void Input::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{

}

// Changes the viewport to match the resized window size
void Input::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}