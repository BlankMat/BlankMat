#pragma once
#include "interfaces/iCommand.h"

class RedoCommand : public ICommand
{
public:
	void Execute() const override
	{

	}

	/// <summary>
	/// Redo cannot be undone
	/// </summary>
	void Undo() const override {}

	/// <summary>
	/// Returns the name of this command
	/// </summary>
	/// <returns></returns>
	const std::string GetName() override
	{
		return "REDO";
	}

	explicit RedoCommand()
	{
		mCanBeUndone = false;
	}
};