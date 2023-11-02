#pragma once
#include "interfaces/iCommand.h"

template<typename T>
class AddValueCommand : public ICommand
{
protected:
	T mPrevValue;
	T mNewValue;
	T* mValueRef;
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
	/// Instantiates a command that adds the given value to the value of the given reference once the command is executed
	/// </summary>
	/// <param name="valueRef">Reference to the variable to change</param>
	/// <param name="delta">Value to add to the variable</param>
	explicit AddValueCommand(T* valueRef, const T& delta)
	{
		mValueRef = valueRef;
		mPrevValue = *valueRef;
		mNewValue = *valueRef + delta;
	}
};