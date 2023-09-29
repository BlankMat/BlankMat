#pragma once
#include "glIncludes.h"
#include <iostream>
#include <string>

class Window {
private:
	int mWidth;
	int mHeight;
	std::string mName;
	GLFWwindow* window;
public:
	// Opens a OpenGL window with the given name
	// -----------------------------------------
	Window(int width, int height, std::string name);

	// Gets and stores the current window size
	void CalcWindowSize() { glfwGetWindowSize(window, &mWidth, &mHeight); }

	// Returns the GLFW window reference
	GLFWwindow* GetWindow() { return window; }
	// Returns the width of the window, currently
	int GetWidth() { return mWidth; }
	// Returns the height of the window, currently
	int GetHeight() { return mHeight; }
	// Returns the aspect ratio of the screen
	float GetAspect() { return (float)mWidth / (float)mHeight; }
};

void OpenGLEnableWireframe(bool enable);

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
