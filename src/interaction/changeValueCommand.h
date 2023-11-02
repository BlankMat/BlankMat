#pragma once
#include "interfaces/iCommand.h"

template<typename T>
class ChangeValueCommand : public ICommand
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
	/// Instantiates a command that changes the value of the given reference to the given value once the command is executed
	/// </summary>
	/// <param name="valueRef">Reference to the variable to change</param>
	/// <param name="newValue">Value to change the variable to</param>
	explicit ChangeValueCommand(T* valueRef, const T& newValue)
	{
		mValueRef = valueRef;
		mPrevValue = *valueRef;
		mNewValue = newValue;
	}
};