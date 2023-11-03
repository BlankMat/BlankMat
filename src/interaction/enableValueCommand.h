#pragma once
#include "utils.h"
#include "interfaces/iCommand.h"

class EnableValueCommand : public ICommand
{
protected:
	bool mPrevValue;
	bool mNewValue;
	bool* mValueRef;
public:
	/// <summary>
	/// Executes this command
	/// </summary>
	void Execute() const override
	{
		*mValueRef = mNewValue;
	}

	/// <summary>
	/// Undoes this command
	/// </summary>
	void Undo() const override
	{
		*mValueRef = mPrevValue;
	}

	/// <summary>
	/// Returns the name of this command
	/// </summary>
	/// <returns></returns>
	const std::string GetName() override
	{
		return "ENABLED_VALUE_FROM_" + BoolToString(mPrevValue) + "_TO_" + BoolToString(mNewValue);
	}

	/// <summary>
	/// Instantiates a command that changes the value of the given reference to the given value once the command is executed
	/// </summary>
	/// <param name="valueRef">Reference to the variable to change</param>
	/// <param name="newValue">Value to change the variable to</param>
	explicit EnableValueCommand(bool* valueRef, const bool& newValue)
	{
		mValueRef = valueRef;
		mPrevValue = *valueRef;
		mNewValue = newValue;
		mCanBeUndone = true;
	}
};