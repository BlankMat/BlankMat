#pragma once
#include "interfaces/iCommand.h"
#include <functional>

class RunToggleFunctionCommand : public ICommand
{
protected:
	std::function<void()> mFunction = nullptr;
	std::string mName = "";
public:
	/// <summary>
	/// Executes this command
	/// </summary>
	void Execute() override
	{
		if (mFunction != nullptr)
			mFunction();
	}

	/// <summary>
	/// Undoes the command if it is undoable
	/// </summary>
	void Undo() override
	{
		if (mFunction != nullptr)
			mFunction();
	}

	/// <summary>
	/// Does not combine the commands
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
		return "RUN_TOGGLE_FUNCTION_" + mName;
	}

	/// <summary>
	/// Instantiates a command that runs the given function once the command is executed
	/// </summary>
	explicit RunToggleFunctionCommand(const std::string& name, std::function<void()> function)
		: mName(name)
	{
		mFunction = function;
		mCanBeUndone = true;
		mTrackable = true;
	}
};