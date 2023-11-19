#include "interaction/input.h"
#include <iostream>

// Handles keyboard key interactions
void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// If ImGui wants to capture the keyboard, don't handle it natively
	if (mIO->WantTextInput)
		return;

	// Ignore Alt/Ctrl/Shift
	if (IsModKey(key))
		return;

	bool change = false;
	if (action == GLFW_PRESS)
		change = PressKey(GetKeyCode(key), mods);
	else if (action == GLFW_RELEASE)
		change = UnPressKey(GetKeyCode(key));

	if (change)
		std::cout << "Called KeyCallback " << GetActionCode(action) << " " << GetModCode(mods) << GetKeyCode(key) << std::endl;
}

// Handles mouse button interactions
void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	// If ImGui wants to capture the mouse, don't handle it natively
	if (mIO->WantCaptureMouse)
		return;

	bool change = false;
	if (action == GLFW_PRESS)
		change = PressKey(GetMouseCode(button), mods);
	else if (action == GLFW_RELEASE)
		change = UnPressKey(GetMouseCode(button));

	if (change)
		std::cout << "Called MouseButtonCallback " << GetActionCode(action) << " " << GetModCode(mods) << GetMouseCode(button) << std::endl;
}

// Handles mouse movement interactions
void Input::CursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	mState->mouseDeltaX = xPos - mState->mousePrevX;
	mState->mouseDeltaY = yPos - mState->mousePrevY;
	mState->mousePrevX = xPos;
	mState->mousePrevY = yPos;
}

// Handles mouse scroll interactions
void Input::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	//std::cout << "Called ScrollCallback, xOffset " << xOffset << ", yOffset " << yOffset << std::endl;
}

// Initializes the input module with the current hotkeys
Input::Input(ImGuiIO* io, State* state, Config* config)
	: mIO(io), mState(state)
{
	// Copy the hotkeys from the config
	auto& hotkeys = config->GetConfig("hotkeys")->GetStrings();
	for (auto iter = hotkeys.begin(); iter != hotkeys.end(); ++iter)
		mHotkeys.emplace(iter->first, iter->second);

	mMouseRotSpeed = config->GetFloat("speeds.mouseRot");
	mMouseZoomSpeed = config->GetFloat("speeds.mouseZoom");
	mMouseMoveSpeed = config->GetFloat("speeds.mouseMove");
}