#pragma once
#include "interfaces/iCommand.h"

class UndoCommand : public ICommand
{
public:
	/// <summary>
	/// Executes this command
	/// </summary>
	void Execute() override
	{

	}

	/// <summary>
	/// Undo cannot be undone
	/// </summary>
	void Undo() override {}

	/// <summary>
	/// Undo cannot be combined
	/// </summary>
	/// <param name="other">Other command</param>
	/// <returns>False</returns>
	bool Combine(ICommand*& other) override
	{
		return false;
	}

	/// <summary>
	/// Returns the name of this command
	/// </summary>
	/// <returns></returns>
	const std::string GetName() const override
	{
		return "UNDO";
	}

	explicit UndoCommand()
	{
		mCanBeUndone = false;
	}
};