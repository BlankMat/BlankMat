#pragma once
#include "glIncludes.h"

enum class GUI
{
	NONE = 0,
	DEBUG_TOOLS = 1,
	HIERARCHY = 2,
	INSPECTOR = 3,
	MENU_BAR = 4,
	TOOLBAR = 5,
	MATERIAL_BAR = 6,
	TRANSFORM_EDITOR = 7,
	MODEBAR = 8,

	MODEL_EDITOR = 10,
	MATERIAL_EDITOR = 11,
	TEXTURE_EDITOR = 12,
	UV_EDITOR = 13,

	MATERIAL_VIEWER = 20,
	TEXTURE_VIEWER = 21,
	LIGHT_VIEWER = 22,
	CAMERA_VIEWER = 23,

	ACTION_LIST = 31
};

class IGUIWindow
{
protected:
	GUI mType = GUI::NONE;
	std::string mName = "";
	glm::vec2 mScreenPos = glm::vec2(0, 0);
	bool mIsEnabled = true;

public:
	// Draws the GUI to the screen
	virtual void Draw() = 0;

	// Returns whether the GUI is enabled
	bool IsEnabled()
	{
		return mIsEnabled;
	}

	// Enables the GUI
	void Enable()
	{
		mIsEnabled = true;
	}

	// Disables the GUI
	void Disable()
	{
		mIsEnabled = false;
	}
	
	// Toggles the enabled status of the GUI
	bool ToggleEnabled()
	{
		mIsEnabled = !mIsEnabled;
		return mIsEnabled;
	}

	// Gets the type of the GUI
	GUI GetType()
	{
		return mType;
	}

	// Gets the name of the GUI
	const std::string GetName()
	{
		return mName;
	}

	// Sets the name of the GUI
	void SetName(const std::string& name)
	{
		mName = name;
	}

	// Gets the position of the GUI on the screen
	const glm::vec2 GetPos()
	{
		return mScreenPos;
	}

	// Sets the position of the GUI on the screen
	void SetPos(const glm::vec2& pos)
	{
		mScreenPos = pos;
	}
};