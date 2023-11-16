#pragma once
#include "glIncludes.h"
#include "iGUIWindow.h"
#include "interaction/input.h"
#include "files/config.h"
#include "files/fileSystem.h"
#include "tools/state.h"
#include <iostream>
#include <unordered_map>
#include <string>

class Window {
private:
	int mWidth = 1920;
	int mHeight = 1080;
	std::string mName = "BlankMat";
	GLFWwindow* mWindow = nullptr;
	ImGuiIO* mIO = nullptr;
	Input* mInput = nullptr;
	bool mIsDockSpaceInitialized = false;

	std::unordered_map<GUI, IGUIWindow*> mGUIList;

	// Sets up GLFW for the app
	bool SetupGLFW();

	// Sets up GLAD for the app
	bool SetupGLAD();

	// Sets up input for the app
	bool SetupInput();
	
	// Sets the icon for the app
	bool SetupIcon(State* state);

	// Sets up ImGui for the app
	bool SetupImGui(State* state, Config* config);

	// Sets up the ImGui Style
	bool SetupImGuiStyle(bool isDarkStyle, float alphaThreshold);

	// Sets up shadows for the app
	bool SetupShadows(State* state);

	// Wrapper to call input KeyCallback
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
	{
		Window* obj = (Window*)glfwGetWindowUserPointer(window);
		obj->mInput->KeyCallback(window, key, scancode, action, mods);
	}

	// Wrapper to call input CursorPostionCallback
	static void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos) 
	{
		Window* obj = (Window*)glfwGetWindowUserPointer(window);
		obj->mInput->CursorPositionCallback(window, xPos, yPos);
	}

	// Wrapper to call input MouseButtonCallback
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
	{
		Window* obj = (Window*)glfwGetWindowUserPointer(window);
		obj->mInput->MouseButtonCallback(window, button, action, mods);
	}

	// Wrapper to call input ScrollCallback
	static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset) 
	{
		Window* obj = (Window*)glfwGetWindowUserPointer(window);
		obj->mInput->ScrollCallback(window, xOffset, yOffset);
	}
public:
	// Opens a OpenGL window with the given name
	// -----------------------------------------
	Window(int width, int height, const std::string& name, Config* config, State* state);

	// Draws all GUIs
	void DrawGUI();

	// Adds the given GUI window
	void AddGUI(IGUIWindow* gui);
	// Gets the GUI with the given type
	IGUIWindow* GetGUI(GUI type);

	// Gets and stores the current window size
	void CalcWindowSize() { glfwGetWindowSize(mWindow, &mWidth, &mHeight); }

	// Returns the GLFW window reference
	GLFWwindow* GetWindow() { return mWindow; }
	// Returns the window's input module
	Input* GetInput() { return mInput; }
	// Returns the width of the window, currently
	int GetWidth() { return mWidth; }
	// Returns the height of the window, currently
	int GetHeight() { return mHeight; }
	// Returns the aspect ratio of the screen
	float GetAspect() { return (float)mWidth / (float)mHeight; }
};
