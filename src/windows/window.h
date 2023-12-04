#pragma once
#include "glIncludes.h"
#include "interfaces/iGUIWindow.h"
#include "interaction/input.h"
#include "files/config.h"
#include "files/fileSystem.h"
#include "files/sceneIO.h"
#include "tools/state.h"
#include "rendering/scene.h"

#include "windows/guiMenuBarWindow.h"
#include "windows/guiHierarchyWindow.h"
#include "windows/guiInspectorWindow.h"
#include "windows/guiToolbarWindow.h"
#include "windows/guiToolModeWindow.h"

#include "windows/guiCameraViewer.h"
#include "windows/guiLightViewer.h"
#include "windows/guiTextureViewer.h"
#include "windows/guiTextureEditor.h"
#include "windows/guiMaterialViewer.h"
#include "windows/guiMaterialEditor.h"

#include "windows/guiDebugToolsWindow.h"
#include "windows/guiActionList.h"

#include <iostream>
#include <unordered_map>
#include <string>

class Window {
private:
	int mWidth = 1920;
	int mHeight = 1080;
	bool mIsDockSpaceInitialized = false;
	std::string mName = "BlankMat";

	GLFWwindow* mWindow = nullptr;
	ImGuiIO* mIO = nullptr;
	Config* mConfig = nullptr;
	Input* mInput = nullptr;
	SceneIO* mSceneIO = nullptr;
	Scene* mScene = nullptr;
	State* mState = nullptr;

	std::unordered_map<GUI, IGUIWindow*> mGUIList;

	// Sets up GLFW for the app
	bool SetupGLFW();

	// Sets up GLAD for the app
	bool SetupGLAD();

	// Sets up input for the app
	bool SetupInput();
	
	// Sets the icon for the app
	bool SetupIcon();

	// Sets up ImGui for the app
	bool SetupImGui(Config* config);

	// Sets up the ImGui Style
	bool SetupImGuiStyle(bool isDarkStyle, float alphaThreshold);

	// Sets up shadows for the app
	bool SetupShadows();

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
	Window(int width, int height, const std::string& name, Config* config);

	// Draws all GUIs
	void DrawGUI(State* state);

	// Adds the given GUI window
	void AddGUI(IGUIWindow* gui);

	// Gets the GUI with the given type
	IGUIWindow* GetGUI(GUI type);

	// Returns whether the given type of GUI is open
	bool IsGUIOpen(GUI type);

	// Opens the given GUI if it is not open yet
	void OpenGUI(GUI gui);

	// Opens all defined GUIs
	void LoadGUIs();

	// Updates the window title to match the current state of the application
	void UpdateWindowTitle(const std::string& fileName, bool isSaved);

	// Resets the viewport to be the correct size
	void ResetViewport()
	{
		CalcWindowSize();
		glViewport(0, 0, mWidth, mHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// Gets and stores the current window size
	void CalcWindowSize() { glfwGetWindowSize(mWindow, &mWidth, &mHeight); }

	// Attempts to quit the window
	void Quit() { glfwSetWindowShouldClose(mWindow, GLFW_TRUE); }

	// Returns the GLFW window reference
	inline GLFWwindow* GetWindow() const { return mWindow; }
	// Returns the window's input module
	inline Input* GetInput() const { return mInput; }
	// Returns the window's scene file operations module
	inline SceneIO* GetSceneIO() const { return mSceneIO; }
	// Returns the window's scene
	inline Scene* GetScene() const { return mScene; }
	// Returns the window's global state
	inline State* GetState() const { return mState; }
	// Returns the width of the window, currently
	inline int GetWidth() const { return mWidth; }
	// Returns the height of the window, currently
	inline int GetHeight() const { return mHeight; }
	// Returns the aspect ratio of the screen
	inline float GetAspect() const { return (mHeight > 0) ? (float)mWidth / (float)mHeight : 1.0f; }
};
