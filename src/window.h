#pragma once
#include "glIncludes.h"
#include <iostream>
#include <string>

class Window {
private:
	int width;
	int height;
	std::string name;
	GLFWwindow* window;
public:
	// Opens a OpenGL window with the given name
	// -----------------------------------------
	Window(int _width, int _height, std::string _name);

	// Gets and stores the current window size
	void CalcWindowSize() { glfwGetWindowSize(window, &width, &height); }

	// Returns the GLFW window reference
	GLFWwindow* GetWindow() { return window; }
	// Returns the width of the window, currently
	int GetWidth() { return width; }
	// Returns the height of the window, currently
	int GetHeight() { return height; }
	// Returns the aspect ratio of the screen
	float GetAspect() { return (float)width / (float)height; }
};

void OpenGLEnableWireframe(bool enable);

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
