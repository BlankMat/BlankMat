#pragma once
#include "interfaces/iCommand.h"
#include <memory>

/// <summary>
/// Class that keeps track of all commands that have been run.
/// Allows functionality for undo/redo.
/// </summary>
class ActionStack
{
private:
	/// <summary>
	/// Doubly linked node that stores the do and undo command of an action
	/// </summary>
	struct ActionNode
	{
		ICommand* mCommandDo;
		ICommand* mCommandUndo;

		ActionNode* mPrevNode;
		ActionNode* mNextNode;

		bool HasPrev() { return mPrevNode != nullptr; }
		bool HasNext() { return mNextNode != nullptr; }
		bool HasDo() { return mCommandDo != nullptr; }
		bool HasUndo() { return mCommandUndo != nullptr; }

		ActionNode(ActionNode* prev, ActionNode* next, ICommand* commandDo, ICommand* commandUndo)
		{
			mPrevNode = prev;
			mNextNode = next;
			mCommandDo = commandDo;
			mCommandUndo = commandUndo;
		}
	};

	ActionNode* mFirstNode;
	ActionNode* mLastNode;
	ActionNode* mCurNode;

	/// <summary>
	/// Traverses the current action node (most recent command) forward if possible
	/// </summary>
	void TraverseForward()
	{
		if (mCurNode != nullptr && mCurNode->HasNext())
		{
			mCurNode = mCurNode->mNextNode;
		}
	}

	/// <summary>
	/// Traverses the current action node (most recent command) backward if possible
	/// </summary>
	void TraverseBackward()
	{
		if (mCurNode != nullptr && mCurNode->HasPrev())
		{
			mCurNode = mCurNode->mPrevNode;
		}
	}

	/// <summary>
	/// Appends a new node with the given command and undo command to the end of the stack.
	/// If no undo command is given, the new command will be the only command in the stack as it cannot be undone.
	/// </summary>
	/// <param name="commandDo">Do command</param>
	/// <param name="commandUndo">Undo command, or null if the action cannot be undone</param>
	void AppendNode(ICommand* commandDo, ICommand* commandUndo = nullptr)
	{
		// If there is no undo, clear all previous commands
		if (commandUndo == nullptr)
		{
			Clear();
		}

		ActionNode* prevNode = (mCurNode != nullptr) ? mCurNode->mPrevNode : nullptr;
		ActionNode* newNode = new ActionNode(prevNode, nullptr, commandDo, commandUndo);

		// If no nodes have been added, make this the first and only node
		if (mCurNode == nullptr || mFirstNode == nullptr || mLastNode == nullptr)
		{
			mCurNode = newNode;
			mFirstNode = newNode;
			mLastNode = newNode;
			return;
		}
		// If there is at least one node, move the last and current node pointers

	}
public:
	/// <summary>
	/// Attempts to undo the most recent command. Does nothing if the most recent command cannot be undone.
	/// </summary>
	void Undo()
	{

	}

	/// <summary>
	/// Attempts to redo the most recently undone command. Does nothing if used without undo.
	/// </summary>
	void Redo()
	{

	}

	/// <summary>
	/// Executes the given command, emplacing it as the most recent item in the stack.
	/// </summary>
	/// <param name="command"></param>
	void Execute(ICommand* command, ICommand* undoCommand = nullptr)
	{
		if (command != nullptr)
		{
			command->Execute();
		}
	}

	/// <summary>
	/// Clears the action stack
	/// </summary>
	void Clear()
	{
		if (mFirstNode != nullptr)
		{
			ActionNode* curNode = mFirstNode->mNextNode;
			mFirstNode = nullptr;
			while (curNode != nullptr)
			{
				ActionNode* tempNextNode = curNode->mNextNode;
				delete curNode;
				curNode = tempNextNode;
			}
		}
		mCurNode = nullptr;
		mLastNode = nullptr;
	}

	/// <summary>
	/// Constructs a new action stack with no actions
	/// </summary>
	explicit ActionStack()
	{
		mFirstNode = nullptr;
		mLastNode = nullptr;
		mCurNode = nullptr;
	}

	/// <summary>
	/// Destructs the whole action stack
	/// </summary>
	~ActionStack()
	{
		Clear();
	}
};