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
};