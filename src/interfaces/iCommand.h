#pragma once
#include <string>

/// <summary>
/// Command interface. Allows executing the command and undoing it.
/// </summary>
class ICommand
{
protected:
	bool mCanBeUndone = true;
	bool mTrackable = true;
public:
	/// <summary>
	/// Executes this command.
	/// </summary>
	virtual void Execute() = 0;

	/// <summary>
	/// Attempts to undo this command. May not be possible for a specific command.
	/// </summary>
	virtual void Undo() = 0;

	/// <summary>
	/// Combine this command with another command if they modify the same variable.
	/// WARNING: If combined, the other command is deleted and will be nullptr
	/// </summary>
	/// <param name="other">Other command to combine with</param>
	/// <returns>Whether the commands were combined</returns>
	virtual bool Combine(ICommand*& other) = 0;

	/// <summary>
	/// Returns whether the command is undoable
	/// </summary>
	/// <returns></returns>
	bool Undoable() const { return mCanBeUndone; }

	/// <summary>
	/// Returns whether the command is trackable (ie. should be stored in ActionStack)
	/// </summary>
	/// <returns></returns>
	bool Trackable() const { return mTrackable; }

	/// <summary>
	/// Returns the name of this command
	/// </summary>
	/// <returns></returns>
	virtual std::string GetName() const = 0;
};