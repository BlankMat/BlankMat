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
	void Execute() override
	{
		*mValueRef = mNewValue;
	}

	/// <summary>
	/// Undoes this command
	/// </summary>
	void Undo() override 
	{
		*mValueRef = mPrevValue;
	}

	/// <summary>
	/// Combine this command with another command if they modify the same variable.
	/// WARNING: If combined, the other command is deleted and will be nullptr
	/// </summary>
	/// <param name="other">Other command to combine with</param>
	/// <returns>Whether the commands were combined</returns>
	bool Combine(ICommand*& other) override
	{
		auto* otherCast = dynamic_cast<ChangeValueCommand*>(other);
		// If the commands modify different variables, don't combine them
		if (!otherCast || this->mValueRef != otherCast->mValueRef)
			return false;

		// Update this command's value to the new value
		mNewValue = otherCast->mNewValue;
		*mValueRef = mNewValue;
		
		// Delete the other, since the action stack will not be doing that
		delete other;
		return true;
	}

	/// <summary>
	/// Returns the name of this command
	/// </summary>
	/// <returns></returns>
	const std::string GetName() const override
	{
		return "CHANGE_VALUE_FROM_" + TypeToString(mPrevValue) + "_TO_" + TypeToString(mNewValue);
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
		mCanBeUndone = true;
	}
};