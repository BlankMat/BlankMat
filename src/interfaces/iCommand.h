#pragma once

/// <summary>
/// Command interface. Allows executing the command and undoing it.
/// </summary>
class ICommand
{
protected:
	bool mCanBeUndone = true;
public:
	/// <summary>
	/// Executes this command.
	/// </summary>
	virtual void Execute() const = 0;

	/// <summary>
	/// Attempts to undo this command. May not be possible for a specific command.
	/// </summary>
	virtual void Undo() const = 0;

	/// <summary>
	/// Returns whether the command is undoable
	/// </summary>
	/// <returns></returns>
	bool Undoable() { return mCanBeUndone; }

	virtual ~ICommand() {}
};