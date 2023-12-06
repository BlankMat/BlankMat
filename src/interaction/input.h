#pragma once
#include "glIncludes.h"
#include "files/config.h"
#include "interaction/state.h"
#include "interfaces/iCommand.h"
#include <unordered_map>
#include <unordered_set>
#include <functional>

enum class MouseInputMode { DEFAULT = 0, ROTATE, ZOOM, MOVE };

class Input
{
private:
	ImGuiIO* mIO = nullptr;
	State* mState = nullptr;
	MouseInputMode mMouseInputMode = MouseInputMode::DEFAULT;

	float mMouseRotSpeed = 0.15f;
	float mMouseZoomSpeed = 0.5f;
	float mMouseMoveSpeed = 0.3f;

	std::function<void(const glm::vec3& delta)> mMouseRotFunc = nullptr;
	std::function<void(const glm::vec3& delta)> mMouseZoomFunc = nullptr;
	std::function<void(const glm::vec3& delta)> mMouseMoveFunc = nullptr;

	std::unordered_map<std::string, int> mKeysPressed;
	std::unordered_map<std::string, std::string> mHotkeys;
	std::unordered_map<std::string, ICommand*> mCommands;
public:
	/// <summary>
	/// Returns whether the key is a modifier key
	/// </summary>
	/// <param name="key">Key code to check</param>
	/// <returns>Is the key a modifier key?</returns>
	static bool IsModKey(int key)
	{
		// Keys in range 340 to 347 are Left/Right Shift/Alt/Control/Super
		return (key >= GLFW_KEY_LEFT_SHIFT && key <= GLFW_KEY_RIGHT_SUPER);
	}

	/// <summary>
	/// Sets the mouse input mode to the given mode
	/// </summary>
	/// <param name="window"></param>
	/// <param name="newMode">Mode to set input to</param>
	void SetMouseInputMode(GLFWwindow* window, MouseInputMode newMode)
	{
		mMouseInputMode = newMode;
		if (newMode != MouseInputMode::DEFAULT)
			DisableMouseCursor(window);
		else
			ShowMouseCursor(window);
	}

	/// <summary>
	/// Sets the callback function that runs whenever the mouse moves in rotation mode
	/// </summary>
	/// <param name="func">Callback function to run</param>
	void SetMouseRotFunction(std::function<void(const glm::vec3& delta)> func)
	{
		mMouseRotFunc = func;
	}

	/// <summary>
	/// Sets the callback function that runs whenever the mouse moves in zoom mode
	/// </summary>
	/// <param name="func">Callback function to run</param>
	void SetMouseZoomFunction(std::function<void(const glm::vec3& delta)> func)
	{
		mMouseZoomFunc = func;
	}

	/// <summary>
	/// Sets the callback function that runs whenever the mouse moves in move mode
	/// </summary>
	/// <param name="func">Callback function to run</param>
	void SetMouseMoveFunction(std::function<void(const glm::vec3& delta)> func)
	{
		mMouseMoveFunc = func;
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

		// Trigger hotkey
		std::string hotkey = GetModCode(mods) + keyCode;
		if (mHotkeys.contains(hotkey))
			RunCommand(mHotkeys[hotkey]);
		return true;
	}

	/// <summary>
	/// Unpresses the given key
	/// </summary>
	/// <param name="keyCode">Key code to unpress</param>
	/// <returns>Whether the key was successfully unpressed</returns>
	bool UnPressKey(const std::string& keyCode)
	{
		// If the key is not pressed, don't unpress it
		if (!IsKeyPressed(keyCode))
			return false;
		int mods = mKeysPressed[keyCode];
		mKeysPressed.erase(keyCode);

		// Trigger reverse hotkey if it exists
		std::string hotkey = GetModCode(mods) + keyCode;
		if (mHotkeys.contains(hotkey))
			RunCommand(mHotkeys[hotkey] + "_RELEASE");
		return true;
	}

	/// <summary>
	/// Returns whether the given key is pressed or not
	/// </summary>
	/// <param name="keyCode">Key code to check</param>
	/// <returns>Whether the key is pressed</returns>
	bool IsKeyPressed(const std::string& keyCode) const
	{
		return mKeysPressed.contains(keyCode);
	}

	/// <summary>
	/// Runs the command with the given name if it exists
	/// </summary>
	/// <param name="command">Command to run</param>
	void RunCommand(const std::string& command)
	{
		std::cout << "Ran command " << command << std::endl;
		if (mCommands.contains(command))
			mState->GetActionStack()->Execute(mCommands[command]);
	}

	/// <summary>
	/// Adds the given command to the command list
	/// </summary>
	/// <param name="name">Name of the command</param>
	/// <param name="command">The command</param>
	void AddCommand(const std::string& name, ICommand* command)
	{
		// If the command already exists, replace it
		if (mCommands.contains(name))
			mCommands[name] = command;
		else
			mCommands.emplace(name, command);
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
		case GLFW_KEY_SPACE:
			return "SPACE";
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
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_1:
			return "MB1";
		case GLFW_MOUSE_BUTTON_2:
			return "MB2";
		case GLFW_MOUSE_BUTTON_3:
			return "MB3";
		case GLFW_MOUSE_BUTTON_4:
			return "MB4";
		case GLFW_MOUSE_BUTTON_5:
			return "MB5";
		default:
			return "NULL";
		}
	}

	/// <summary>
	/// Returns the action code as a string
	/// </summary>
	/// <param name="action">Action</param>
	/// <returns>Name of action</returns>
	static std::string GetActionCode(int action)
	{
		switch (action)
		{
		case GLFW_PRESS:
			return "PRESS";
		case GLFW_RELEASE:
			return "RELEASE";
		case GLFW_REPEAT:
			return "REPEAT";
		default:
			return "NULL";
		}
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
	/// <param name="io">ImGui Input/Output module</param>
	/// <param name="state">Global state</param>
	/// <param name="config">Top level config</param>
	explicit Input(ImGuiIO* io, State* state, Config* config);
};