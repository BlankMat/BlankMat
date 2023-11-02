#pragma once
#include "interfaces/iCommand.h"

class RedoCommand : public ICommand
{
	void Execute() const override
	{

	}

	/// <summary>
	/// Redo cannot be undone
	/// </summary>
	void Undo() const override {}
};