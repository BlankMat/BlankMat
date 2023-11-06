#pragma once
#include "interaction/actionStack.h"
#include "interaction/changeValueCommand.h"

/// <summary>
/// Class to store a variable that can be edited in the UI.
/// This class sends a Command to the ActionStack whenever the value of the variable is changed.
/// </summary>
/// <typeparam name="T">Type of variable to store</typeparam> 
template<typename T>
class IUIVariable
{
protected:
	/// <summary>
	/// Name of the variable
	/// </summary>
	std::string mName;

	/// <summary>
	/// Value of the variable
	/// </summary>
	T mValue;

	/// <summary>
	/// Reference to the global action stack
	/// </summary>
	ActionStack* mActionStack;

	/// <summary>
	/// Callback function that runs whenever set is modified
	/// </summary>
	std::function<void()> mSetCallback;
public:
	/// <summary>
	/// Displays the variable as an editable field in the UI. 
	/// Each variable type needs to handle this differently, so this method must be overridden by implementations.
	/// </summary>
	virtual void Display() = 0;

	/// <summary>
	/// Returns whether the value is equal to the given value
	/// </summary>
	/// <param name="value">Value to check</param>
	/// <returns>Whether the values are equals</returns>
	bool Equals(const T& value) const
	{
		return (mValue == value);
	}

	/// <summary>
	/// Returns the current value of this variable
	/// </summary>
	/// <returns>The current value</returns>
	T Get() const
	{
		return mValue;
	}

	/// <summary>
	/// Returns a reference to the value stored in this variable. 
	/// This is unsafe, but is necessary for Commands outside of the class to update the value.
	/// </summary>
	/// <returns>Reference of the current value</returns>
	T* GetRef() const
	{
		return &mValue;
	}

	/// <summary>
	/// Implicitly converts this variable to its internal value
	/// </summary>
	operator T()
	{
		return mValue;
	}

	/// <summary>
	/// Returns a reference to the value stored in this variable
	/// </summary>
	/// <returns>Reference of the value of the variable</returns>
	T& operator*()
	{
		return mValue;
	}

	/// <summary>
	/// Returns a reference to the value stored in this variable
	/// </summary>
	/// <returns>Reference of the value of the variable</returns>
	T* operator->()
	{
		return &mValue;
	}

	/// <summary>
	/// Returns a reference to the value stored in this variable
	/// </summary>
	/// <returns>Reference of the value of the variable</returns>
	T* operator&()
	{
		return &mValue;
	}

	/// <summary>
	/// Returns the name of the variable
	/// </summary>
	/// <returns>Name of the variable</returns>
	std::string Name() const
	{
		return mName;
	}

	/// <summary>
	/// Sets the value of the variable to the new value only if it is different.
	/// Sends a Command to the action stack indicating the changed value.
	/// </summary>
	/// <param name="newValue">New value to set it to</param>
	void Set(const T& newValue)
	{
		// Don't change the value if nothing changed
		if (mValue == newValue)
			return;

		// If the value changed, notify the action stack
		if (mActionStack != nullptr)
			mActionStack->Execute(new ChangeValueCommand<T>(&mValue, newValue, mSetCallback));

		// Trigger the callback function if it exists
		if (mSetCallback != nullptr)
			mSetCallback();

		// Update the value of the variable
		mValue = newValue;
	}

	/// <summary>
	/// Sets the value of the variable to the new value without creating a command
	/// </summary>
	/// <param name="newValue">New value to set it to</param>
	void SetNoUpdate(const T& newValue)
	{
		// Don't change the value if nothing changed
		if (mValue == newValue)
			return;

		mValue = newValue;
	}
};