#pragma once
#include "interfaces/iCommand.h"

/// <summary>
/// Doubly linked node that stores the do and undo command of an action
/// </summary>
struct ActionNode
{
private:
	/// <summary>
	/// The command that the node executes
	/// </summary>
	ICommand* mCommand;

public:
	/// <summary>
	/// The previous node of the action
	/// </summary>
	ActionNode* mPrevNode;

	/// <summary>
	/// The next node of the action
	/// </summary>
	ActionNode* mNextNode;

	/// <summary>
	/// Returns whether this action has a previous node
	/// </summary>
	/// <returns></returns>
	bool HasPrev()
	{
		return mPrevNode != nullptr;
	}

	/// <summary>
	/// Returns whether this action has a next node
	/// </summary>
	/// <returns></returns>
	bool HasNext() 
	{ 
		return mNextNode != nullptr; 
	}
	
	/// <summary>
	/// Returns whether this action has a valid command
	/// </summary>
	/// <returns></returns>
	bool HasCommand() 
	{ 
		return mCommand != nullptr; 
	}
	
	/// <summary>
	/// Returns whether this action can be undone
	/// </summary>
	/// <returns></returns>
	bool CanUndo()
	{ 
		return (mCommand != nullptr && mCommand->Undoable());
	}

	/// <summary>
	/// Executes the command of the node
	/// </summary>
	void Execute()
	{
		if (mCommand != nullptr)
			mCommand->Execute();
	}

	/// <summary>
	/// Undoes the command of the node
	/// </summary>
	void Undo()
	{
		if (mCommand != nullptr)
			mCommand->Undo();
	}

	/// <summary>
	/// Returns the name of the command
	/// </summary>
	/// <returns></returns>
	const std::string GetName()
	{
		if (mCommand != nullptr)
			return mCommand->GetName();
		return "INVALID_NAME";
	}

	/// <summary>
	/// Creates a new action node for the given command
	/// </summary>
	/// <param name="prev">Previously run node</param>
	/// <param name="next">Next node (should be null)</param>
	/// <param name="command">Command to run</param>
	ActionNode(ActionNode* prev, ActionNode* next, ICommand* command)
	{
		mPrevNode = prev;
		mNextNode = next;
		mCommand = command;
	}
};