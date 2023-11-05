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
		// Undo does not need to be executed
	}

	/// <summary>
	/// Undo cannot be undone
	/// </summary>
	void Undo() override 
	{
		// Undo has no undo
	}

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
	std::string GetName() const override
	{
		return "UNDO";
	}

	explicit UndoCommand()
	{
		mCanBeUndone = false;
	}
};