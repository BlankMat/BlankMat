#pragma once
#include "interfaces/iCommand.h"
#include <functional>

class RunFunctionCommand : public ICommand
{
protected:
	std::function<void()> mFunction = nullptr;
	std::function<void()> mReverseFunction = nullptr;
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
		if (mReverseFunction != nullptr)
			mReverseFunction();
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
		return "RUN_FUNCTION_" + mName;
	}

	/// <summary>
	/// Instantiates a command that runs the given function once the command is executed
	/// </summary>
	explicit RunFunctionCommand(const std::string& name, std::function<void()> function, std::function<void()> reverseFunction = std::function<void()>(nullptr))
		: mName(name)
	{
		mFunction = function;
		mReverseFunction = reverseFunction;
		mCanBeUndone = (reverseFunction != nullptr);
		mTrackable = (reverseFunction != nullptr);
	}
};