#pragma once
#include "interfaces/iCommand.h"

class UndoCommand : public ICommand
{
public:
	void Execute() const override
	{

	}

	/// <summary>
	/// Undo cannot be undone
	/// </summary>
	void Undo() const override {}

	/// <summary>
	/// Returns the name of this command
	/// </summary>
	/// <returns></returns>
	const std::string GetName() override
	{
		return "UNDO";
	}

	explicit UndoCommand()
	{
		mCanBeUndone = false;
	}
};