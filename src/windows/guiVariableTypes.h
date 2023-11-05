#pragma once
#include "glIncludes.h"
#include "interfaces/iUIVariable.h"
#include "windows/guiWindowUtils.h"

/// <summary>
/// A boolean that can be directly displayed and edited in the UI.
/// </summary>
class UIBool : public IUIVariable<bool>
{
public:
	/// <summary>
	/// Displays the variable as an editable field in the UI. 
	/// </summary>
	void Display() override
	{
		Set(GUIWindowUtils::Checkbox(mName, mValue));
	}

	/// <summary>
	/// Creates a new variable that can be displayed and updated directly in the UI
	/// </summary>
	/// <param name="value">Value to initialize the variable with</param>
	/// <param name="actionStack">Reference to the global ActionStack</param>
	explicit UIBool(const std::string& name = "", bool value = true, ActionStack* actionStack = nullptr)
	{
		mName = name;
		mValue = value;
		mActionStack = actionStack;
	}
};

/// <summary>
/// An integer that can be directly displayed and edited in the UI.
/// </summary>
class UIInt : public IUIVariable<int>
{
public:
	/// <summary>
	/// Displays the variable as an editable field in the UI. 
	/// </summary>
	void Display() override
	{
		Set(GUIWindowUtils::InputInt(mName, mValue));
	}

	/// <summary>
	/// Adds the given value to this value
	/// </summary>
	/// <param name="other">Value to add</param>
	/// <returns>This + Other</returns>
	int operator+(int other) const
	{
		return mValue + other;
	}

	/// <summary>
	/// Adds the given value to this value
	/// </summary>
	/// <param name="other">Value to add</param>
	/// <returns>This + Other</returns>
	int operator+(const UIInt& other) const
	{
		return mValue + other.mValue;
	}

	/// <summary>
	/// Creates a new variable that can be displayed and updated directly in the UI
	/// </summary>
	/// <param name="value">Value to initialize the variable with</param>
	/// <param name="actionStack">Reference to the global ActionStack</param>
	explicit UIInt(const std::string& name = "", int value = 0, ActionStack* actionStack = nullptr)
	{
		mName = name;
		mValue = value;
		mActionStack = actionStack;
	}
};

/// <summary>
/// A float that can be directly displayed and edited in the UI.
/// </summary>
class UIFloat : public IUIVariable<float>
{
public:
	/// <summary>
	/// Displays the variable as an editable field in the UI. 
	/// </summary>
	void Display() override
	{
		Set(GUIWindowUtils::InputFloat(mName, mValue));
	}

	/// <summary>
	/// Adds the given value to this value
	/// </summary>
	/// <param name="other">Value to add</param>
	/// <returns>This + Other</returns>
	float operator+(float other) const
	{
		return mValue + other;
	}

	/// <summary>
	/// Adds the given value to this value
	/// </summary>
	/// <param name="other">Value to add</param>
	/// <returns>This + Other</returns>
	float operator+(const UIFloat& other) const
	{
		return mValue + other.mValue;
	}

	/// <summary>
	/// Creates a new variable that can be displayed and updated directly in the UI
	/// </summary>
	/// <param name="value">Value to initialize the variable with</param>
	/// <param name="actionStack">Reference to the global ActionStack</param>
	explicit UIFloat(const std::string& name = "", float value = 0.0f, ActionStack* actionStack = nullptr)
	{
		mName = name;
		mValue = value;
		mActionStack = actionStack;
	}
};

/// <summary>
/// A vector3 that can be directly displayed and edited in the UI.
/// </summary>
class UIVec3 : public IUIVariable<glm::vec3>
{
public:
	/// <summary>
	/// Displays the variable as an editable field in the UI. 
	/// </summary>
	void Display() override
	{
		Set(GUIWindowUtils::InputVec3(mName, mValue));
	}

	/// <summary>
	/// Adds the given value to this value
	/// </summary>
	/// <param name="other">Value to add</param>
	/// <returns>This + Other</returns>
	glm::vec3 operator+(const glm::vec3& other) const
	{
		return mValue + other;
	}

	/// <summary>
	/// Adds the given value to this value
	/// </summary>
	/// <param name="other">Value to add</param>
	/// <returns>This + Other</returns>
	glm::vec3 operator+(const UIVec3& other) const
	{
		return mValue + other.mValue;
	}

	/// <summary>
	/// Subtracts the given value from this value
	/// </summary>
	/// <param name="other">Value to subtract</param>
	/// <returns>This - Other</returns>
	glm::vec3 operator-(const glm::vec3& other) const
	{
		return mValue - other;
	}

	/// <summary>
	/// Subtracts the given value from this value
	/// </summary>
	/// <param name="other">Value to subtract</param>
	/// <returns>This - Other</returns>
	glm::vec3 operator-(const UIVec3& other) const
	{
		return mValue - other.mValue;
	}

	/// <summary>
	/// Multiplies the given value with this value
	/// </summary>
	/// <param name="other">Value to multiply</param>
	/// <returns>This * Other</returns>
	glm::vec3 operator*(float other) const
	{
		return other * mValue;
	}

	/// <summary>
	/// Multiplies the given value with this value
	/// </summary>
	/// <param name="other">Value to multiply</param>
	/// <returns>This * Other</returns>
	glm::vec3 operator*(const glm::vec3& other) const
	{
		return mValue * other;
	}

	/// <summary>
	/// Multiplies the given value with this value
	/// </summary>
	/// <param name="other">Value to multiply</param>
	/// <returns>This * Other</returns>
	glm::vec3 operator*(const UIVec3& other) const
	{
		return mValue * other.mValue;
	}

	/// <summary>
	/// Creates a new variable that can be displayed and updated directly in the UI
	/// </summary>
	/// <param name="value">Value to initialize the variable with</param>
	/// <param name="actionStack">Reference to the global ActionStack</param>
	explicit UIVec3(const std::string& name = "", const glm::vec3& value = glm::vec3(0.0f), ActionStack* actionStack = nullptr)
	{
		mName = name;
		mValue = value;
		mActionStack = actionStack;
	}
};

/// <summary>
/// A color (vector3) that can be directly displayed and edited in the UI.
/// </summary>
class UIColor : public IUIVariable<glm::vec3>
{
private:
	bool mPickColor;
public:
	/// <summary>
	/// Displays the variable as an editable field in the UI. 
	/// </summary>
	void Display() override
	{
		Set(GUIWindowUtils::ColorPicker(mName, mValue));
	}

	/// <summary>
	/// Creates a new variable that can be displayed and updated directly in the UI
	/// </summary>
	/// <param name="value">Value to initialize the variable with</param>
	/// <param name="actionStack">Reference to the global ActionStack</param>
	explicit UIColor(const std::string& name = "", const glm::vec3& value = glm::vec3(0.0f), ActionStack* actionStack = nullptr)
	{
		mName = name;
		mValue = value;
		mActionStack = actionStack;
	}
};

/// <summary>
/// A string that can be directly displayed and edited in the UI.
/// </summary>
class UIString : public IUIVariable<std::string>
{
public:
	/// <summary>
	/// Displays the variable as an editable field in the UI. 
	/// </summary>
	void Display() override
	{
		Set(GUIWindowUtils::InputText(mName, mValue));
	}

	/// <summary>
	/// Adds the given value to this value
	/// </summary>
	/// <param name="other">Value to add</param>
	/// <returns>This + Other</returns>
	std::string operator+(const std::string& other) const
	{
		return mValue + other;
	}

	/// <summary>
	/// Adds the given value to this value
	/// </summary>
	/// <param name="other">Value to add</param>
	/// <returns>This + Other</returns>
	std::string operator+(const UIString& other) const
	{
		return mValue + other.mValue;
	}

	/// <summary>
	/// Creates a new variable that can be displayed and updated directly in the UI
	/// </summary>
	/// <param name="value">Value to initialize the variable with</param>
	/// <param name="actionStack">Reference to the global ActionStack</param>
	explicit UIString(const std::string& name = "", const std::string& value = "", ActionStack* actionStack = nullptr)
	{
		mName = name;
		mValue = value;
		mActionStack = actionStack;
	}
};