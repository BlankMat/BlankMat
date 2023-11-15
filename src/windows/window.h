#pragma once
#include "glIncludes.h"
#include "iGUIWindow.h"
#include "interaction/newinput.h"
#include "files/config.h"
#include "files/fileSystem.h"
#include "tools/state.h"
#include <iostream>
#include <unordered_map>
#include <string>

class Window {
private:
	int mWidth;
	int mHeight;
	std::string mName;
	GLFWwindow* mWindow;
	ImGuiIO* mIO;

	std::unordered_map<GUI, IGUIWindow*> mGUIList;

	// Flag to track if the docking space is initialized
	bool DockSpaceInitialized;

	// Sets up GLFW for the app
	bool SetupGLFW();

	// Sets up GLAD for the app
	bool SetupGLAD();

	// Sets up input for the app
	bool SetupInput();
	
	// Sets the icon for the app
	bool SetupIcon(State* state);

	// Sets up ImGui for the app
	bool SetupImGui(Config* config);

	// Sets up the ImGui Style
	bool SetupImGuiStyle(bool isDarkStyle, float alphaThreshold);

	// Sets up shadows for the app
	bool SetupShadows(State* state);
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
	// Returns the width of the window, currently
	int GetWidth() { return mWidth; }
	// Returns the height of the window, currently
	int GetHeight() { return mHeight; }
	// Returns the aspect ratio of the screen
	float GetAspect() { return (float)mWidth / (float)mHeight; }
};
