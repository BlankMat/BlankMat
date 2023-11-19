#pragma once
#include "interfaces/iCommand.h"

template<typename T>
class ChangeValueCommand : public ICommand
{
private:
	/// <summary>
	/// Value before this command was run
	/// </summary>
	T mPrevValue;

	/// <summary>
	/// Value after this command was run
	/// </summary>
	T mNewValue;

	/// <summary>
	/// Reference to the address of the value
	/// </summary>
	T* mValueRef;

	/// <summary>
	/// Optional set callback that needs to run when the value of valueRef is changed
	/// </summary>
	std::function<void()> mSetCallback;
public:
	/// <summary>
	/// Executes this command
	/// </summary>
	void Execute() override
	{
		*mValueRef = mNewValue;
		if (mSetCallback != nullptr)
			mSetCallback();
	}

	/// <summary>
	/// Undoes this command
	/// </summary>
	void Undo() override 
	{
		*mValueRef = mPrevValue;
		if (mSetCallback != nullptr)
			mSetCallback();
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
	std::string GetName() const override
	{
		return "CHANGE_VALUE_FROM_" + TypeToString(mPrevValue) + "_TO_" + TypeToString(mNewValue);
	}

	/// <summary>
	/// Instantiates a command that changes the value of the given reference to the given value once the command is executed
	/// </summary>
	/// <param name="valueRef">Reference to the variable to change</param>
	/// <param name="newValue">Value to change the variable to</param>
	explicit ChangeValueCommand(T* valueRef, const T& newValue, std::function<void()> setCallback = std::function<void()>(nullptr))
		: mValueRef(valueRef), mPrevValue(*valueRef), mNewValue(newValue), mSetCallback(setCallback)
	{
		mCanBeUndone = true;
	}
};