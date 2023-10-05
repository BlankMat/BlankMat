#pragma once
#include "glIncludes.h"

enum class GUI
{
	NONE = 0,
	DEBUG_TOOLS = 1,
	HIERARCHY = 2,
	TOOLBAR = 3,
	MATERIAL_BAR = 4,
	TRANSFORM_EDITOR = 5,

	MODEL_EDITOR = 10,
	MATERIAL_EDITOR = 11,
	UV_EDITOR = 12,
};

class IGUIWindow
{
protected:
	GUI type;
	std::string mName;
	glm::vec2 mScreenPos;
	bool mIsEnabled;
public:
	// Draws the GUI to the screen
	virtual void Draw() = 0;

	// Returns whether the GUI is enabled
	bool IsEnabled() { return mIsEnabled; }
	// Enables the GUI
	void Enable() { mIsEnabled = true; }
	// Disables the GUI
	void Disable() { mIsEnabled = false; }
	// Toggles the enabled status of the GUI
	bool ToggleEnabled() { mIsEnabled = !mIsEnabled; return mIsEnabled; }

	// Gets the type of the GUI
	GUI GetType() { return type; }
	// Gets the name of the GUI
	std::string GetName() { return mName; }
	// Sets the name of the GUI
	void SetName(std::string name) { mName = name; }
	// Gets the position of the GUI on the screen
	glm::vec2 GetPos() { return mScreenPos; }
	// Sets the position of the GUI on the screen
	void SetPos(glm::vec2 pos) { mScreenPos = pos; }
};