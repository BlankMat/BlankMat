#pragma once
#include "glIncludes.h"
#include "files/config.h"
#include <unordered_map>
#include <unordered_set>

class Input
{
private:
	std::unordered_map<std::string, int> mKeysPressed;
	std::unordered_map<std::string, std::string> mHotkeys;
	std::unordered_map<std::string, std::string> mCommands;
public:
	/// <summary>
	/// Returns whether the key is a modifier key
	/// </summary>
	/// <param name="key">Key code to check</param>
	/// <returns>Is the key a modifier key?</returns>
	static bool IsModKey(int key)
	{
		return (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT
			|| key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT
			|| key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL
			|| key == GLFW_KEY_LEFT_SUPER || key == GLFW_KEY_RIGHT_SUPER
		);
	}

	/// <summary>
	/// Presses the given key with the given mod bits
	/// </summary>
	/// <param name="keyCode">Key code to press</param>
	/// <param name="mods">Modifier bits</param>
	/// <returns>Whether the key was successfully pressed</returns>
	bool PressKey(const std::string& keyCode, int mods)
	{
		// If the key is already pressed, don't override mod bits
		if (IsKeyPressed(keyCode))
			return false;
		mKeysPressed.emplace(keyCode, mods);
		return true;
	}

	/// <summary>
	/// Unpresses the given key
	/// </summary>
	/// <param name="keyCode">Key code to unpress</param>
	/// <returns>Whether the key was successfully unpressed</returns>
	bool UnPressKey(const std::string& keyCode)
	{
		if (!IsKeyPressed(keyCode))
			return false;
		mKeysPressed.erase(keyCode);
		return true;
	}

	/// <summary>
	/// Returns whether the given key is pressed or not
	/// </summary>
	/// <param name="keyCode">Key code to check</param>
	/// <returns>Whether the key is pressed</returns>
	bool IsKeyPressed(const std::string& keyCode) const
	{
		return mKeysPressed.find(keyCode) != mKeysPressed.end();
	}

	/// <summary>
	/// Returns the modifier keys of the pressed key
	/// </summary>
	/// <param name="keyCode">Key code of the key pressed</param>
	/// <returns>The modifier bits of the pressed key</returns>
	int GeyKeyMods(const std::string& keyCode) const
	{
		if (IsKeyPressed(keyCode))
			return mKeysPressed.at(keyCode);
		else
			return 0;
	}

	/// <summary>
	/// Returns the mod code as a string
	/// </summary>
	/// <param name="mod">Alt/Ctrl/Shift modifiers</param>
	/// <returns>Name of mod</returns>
	static std::string GetModCode(int mod)
	{
		std::string result = "";
		if (CtrlPress(mod))
			result += "CTRL+";
		if (AltPress(mod))
			result += "ALT+";
		if (ShiftPress(mod))
			result += "SHIFT+";
		return result;
	}

	/// <summary>
	/// Returns the key code as a string
	/// </summary>
	/// <param name="key">Key code</param>
	/// <returns>Name of key</returns>
	static std::string GetKeyCode(int key)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			return "ESC";
		case GLFW_KEY_BACKSPACE:
			return "BKSP";
		case GLFW_KEY_DELETE:
			return "DEL";
		case GLFW_KEY_ENTER:
			return "ENTER";
		case GLFW_KEY_TAB:
			return "TAB";
		case GLFW_KEY_UP:
			return "UP";
		case GLFW_KEY_DOWN:
			return "DOWN";
		case GLFW_KEY_RIGHT:
			return "RIGHT";
		case GLFW_KEY_LEFT:
			return "LEFT";
		default:
			if (key >= 0 && key <= 255)
				return std::string(1, (char)key);
			else
				return "NULL";
		}
	}

	/// <summary>
	/// Returns the mouse button code as a string
	/// </summary>
	/// <param name="button">Mouse button</param>
	/// <returns>Name of mouse button</returns>
	static std::string GetMouseCode(int button)
	{
		if (button == GLFW_MOUSE_BUTTON_1)
			return "MB1";
		else if (button == GLFW_MOUSE_BUTTON_2)
			return "MB2";
		else if (button == GLFW_MOUSE_BUTTON_3)
			return "MB3";
		else if (button == GLFW_MOUSE_BUTTON_4)
			return "MB4";
		else if (button == GLFW_MOUSE_BUTTON_5)
			return "MB5";
		else
			return "";
	}

	/// <summary>
	/// Returns whether alt is being pressed
	/// </summary>
	/// <param name="mod">Alt/Ctrl/Shift modifiers</param>
	/// <returns>Whether the modifier is pressing alt</returns>
	static bool AltPress(int mod)
	{
		return (mod & GLFW_MOD_ALT);
	}

	/// <summary>
	/// Returns whether shift is being pressed
	/// </summary>
	/// <param name="mod">Alt/Ctrl/Shift modifiers</param>
	/// <returns>Whether the modifier is pressing alt</returns>
	static bool ShiftPress(int mod)
	{
		return (mod & GLFW_MOD_SHIFT);
	}

	/// <summary>
	/// Returns whether ctrl is being pressed
	/// </summary>
	/// <param name="mod">Alt/Ctrl/Shift modifiers</param>
	/// <returns>Whether the modifier is pressing alt</returns>
	static bool CtrlPress(int mod)
	{
		return (mod & GLFW_MOD_CONTROL);
	}

	/// <summary>
	/// Handles changing the viewport size to match the window size
	/// </summary>
	/// <param name="window">Current window</param>
	/// <param name="width">Width of new window</param>
	/// <param name="height">Height of new window</param>
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

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
	/// <param name="mods">Alt/Ctrl/Shift modifiers</param>
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	
	/// <summary>
	/// Handles mouse button interactions
	/// </summary>
	/// <param name="window">Current window</param>
	/// <param name="button">The mouse button that was pressed or unpressed</param>
	/// <param name="action">Whether the mouse button was pressed or unpressed</param>
	/// <param name="mods">Alt/Ctrl/Shift modifiers</param>
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	/// <summary>
	/// Handles mouse movement interactions
	/// </summary>
	/// <param name="window">Current window</param>
	/// <param name="xPos">x position of cursor</param>
	/// <param name="yPos">y position of cursor</param>
	void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
	
	/// <summary>
	/// Handles mouse scroll interactions
	/// </summary>
	/// <param name="window">Current window</param>
	/// <param name="xOffset">Change in x</param>
	/// <param name="yOffset">Change in y</param>
	void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

	/// <summary>
	/// Initializes the input module with the current hotkeys
	/// </summary>
	/// <param name="hotkeyConfig">Hotkey configuration</param>
	explicit Input(Config* hotkeyConfig);
};