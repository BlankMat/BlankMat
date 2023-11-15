#pragma once
#include "glIncludes.h"

class Input
{
public:
	/// <summary>
	/// Disables the mouse cursor, making it not move even if the user moves their mouse
	/// </summary>
	/// <param name="window">Current window</param>
	static void DisableMouseCursor(GLFWwindow* window)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	
	/// <summary>
	/// Hides the mouse cursor, allowing the user to continue moving it in the background
	/// </summary>
	/// <param name="window">Current window</param>
	static void HideMouseCursor(GLFWwindow* window)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	
	/// <summary>
	/// Shows the mouse cursor, resetting it to the default mode
	/// </summary>
	/// <param name="window">Current window</param>
	static void ShowMouseCursor(GLFWwindow* window)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="window"></param>
	/// <param name="key"></param>
	/// <param name="scancode"></param>
	/// <param name="action"></param>
	/// <param name="mods"></param>
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="window"></param>
	/// <param name="codepoint"></param>
	static void CharacterCallback(GLFWwindow* window, unsigned int codepoint);
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="window"></param>
	/// <param name="xPos"></param>
	/// <param name="yPos"></param>
	static void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="window"></param>
	/// <param name="button"></param>
	/// <param name="action"></param>
	/// <param name="mods"></param>
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="window"></param>
	/// <param name="xOffset"></param>
	/// <param name="yOffset"></param>
	static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	
	/// <summary>
	/// Changes the viewport to match the resized window size
	/// </summary>
	/// <param name="window"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};