#pragma once
#include "interfaces/iCommand.h"

class RunFunctionCommand : public ICommand
{
protected:
	typedef void (*FunctionType)();
	FunctionType mFunction;
public:
	/// <summary>
	/// Executes this command
	/// </summary>
	void Execute() override
	{

	}

	/// <summary>
	/// Undoes this command
	/// </summary>
	void Undo() override
	{

	}

	/// <summary>
	/// Combine this command with another command if they modify the same variable.
	/// WARNING: If combined, the other command is deleted and will be nullptr
	/// </summary>
	/// <param name="other">Other command to combine with</param>
	/// <returns>Whether the commands were combined</returns>
	bool Combine(ICommand*& other) override
	{
		return false;
	}

	/// <summary>
	/// Returns the name of this command
	/// </summary>
	/// <returns></returns>
	std::string GetName() const override
	{
		return "RUN_FUNCTION";
	}

	/// <summary>
	/// Instantiates a command that adds the given value to the value of the given reference once the command is executed
	/// </summary>
	explicit RunFunctionCommand(FunctionType function)
	{
		mFunction = function;
		mCanBeUndone = false;
		mTrackable = false;
	}
};