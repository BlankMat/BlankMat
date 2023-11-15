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
	/// Handles keyboard key interactions
	/// </summary>
	/// <param name="window">Current window</param>
	/// <param name="key">The key that was pressed or unpressed</param>
	/// <param name="scancode">Scancode of key</param>
	/// <param name="action">Whether the key was pressed or unpressed</param>
	/// <param name="mods">Modifier</param>
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	
	/// <summary>
	/// Handles keyboard character typing interactions
	/// </summary>
	/// <param name="window">Current window</param>
	/// <param name="codepoint">Code of character typed</param>
	static void CharacterCallback(GLFWwindow* window, unsigned int codepoint);
	
	/// <summary>
	/// Handles mouse movement interactions
	/// </summary>
	/// <param name="window">Current window</param>
	/// <param name="xPos">x position of cursor</param>
	/// <param name="yPos">y position of cursor</param>
	static void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
	
	/// <summary>
	/// Handles mouse button interactions
	/// </summary>
	/// <param name="window">Current window</param>
	/// <param name="button">The mouse button that was pressed or unpressed</param>
	/// <param name="action">Whether the mouse button was pressed or unpressed</param>
	/// <param name="mods">Modifiers</param>
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	
	/// <summary>
	/// Handles mouse scroll interactions
	/// </summary>
	/// <param name="window">Current window</param>
	/// <param name="xOffset">Change in x</param>
	/// <param name="yOffset">Change in y</param>
	static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	
	/// <summary>
	/// Handles changing the viewport size to match the window size
	/// </summary>
	/// <param name="window">Current window</param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};