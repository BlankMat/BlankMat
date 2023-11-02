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
		ICommand* mCommand;

		ActionNode* mPrevNode;
		ActionNode* mNextNode;

		bool HasPrev() { return mPrevNode != nullptr; }
		bool HasNext() { return mNextNode != nullptr; }
		bool HasCommand() { return mCommand != nullptr; }
		bool CanUndo() { return (mCommand != nullptr && mCommand->Undoable()); }

		ActionNode(ActionNode* prev, ActionNode* next, ICommand* command)
		{
			mPrevNode = prev;
			mNextNode = next;
			mCommand = command;
		}
	};

	/// <summary>
	/// Reference to the first node. Null if empty
	/// </summary>
	ActionNode* mFirstNode;

	/// <summary>
	/// Reference to the last node. Null if empty
	/// </summary>
	ActionNode* mLastNode;

	/// <summary>
	/// Reference to the current node. Should equal lastNode unless undo has been used
	/// </summary>
	ActionNode* mCurNode;

	/// <summary>
	/// Current number of actions stored. Cannot be higher than maxSize
	/// </summary>
	unsigned int mCurSize;

	/// <summary>
	/// Maximum number of actions that can be stored.
	/// </summary>
	unsigned int mMaxSize;

	/// <summary>
	/// Returns the node at the given index or nullptr if the index does not exist
	/// </summary>
	/// <param name="index">Node at the index or nullptr</param>
	/// <returns></returns>
	ActionNode* Get(unsigned int index)
	{
		// Don't allow accessing nodes out of range
		if (index >= mCurSize)
			return nullptr;

		// Iterate forward index times
		ActionNode* curNode = mFirstNode;
		for (unsigned int i = 0; i < index; i++)
		{
			// If there is no next node, return null
			if (curNode == nullptr || !curNode->HasNext())
				return nullptr;
			curNode = curNode->mNextNode;
		}
		return curNode;
	}

	/// <summary>
	/// Resizes the stack to be at most the max size, removing elements from the beginning if necessary
	/// </summary>
	void Resize()
	{
		// Don't resize if the stack is already the correct size
		if (mCurSize <= mMaxSize)
			return;

		// Delete the first curSize - maxSize elements so that curSize equals maxSize
		ActionNode* newFirst = Get(mCurSize - mMaxSize);
		mFirstNode->mNextNode = newFirst;
		ClearBefore(newFirst);
	}

	/// <summary>
	/// Clears all nodes before the given node (not inclusive)
	/// </summary>
	/// <param name="node">Node to clear before</param>
	void ClearBefore(ActionNode* node)
	{
		// Don't clear an empty node
		if (node == nullptr)
			return;

		// Iterate through all nodes, deleting everything before the current node
		ActionNode* curNode = node->mPrevNode;
		node->mPrevNode = nullptr;
		while (curNode != nullptr)
		{
			ActionNode* tempNextNode = curNode->mPrevNode;
			delete curNode;
			curNode = tempNextNode;
			mCurSize--;
		}
	}

	/// <summary>
	/// Clears all nodes after the given node (not inclusive)
	/// </summary>
	/// <param name="node">Node to clear after</param>
	void ClearAfter(ActionNode* node)
	{
		// Don't clear an empty node
		if (node == nullptr)
			return;

		// Iterate through all nodes, deleting everything after the current node
		ActionNode* curNode = node->mNextNode;
		node->mNextNode = nullptr;
		while (curNode != nullptr)
		{
			ActionNode* tempNextNode = curNode->mNextNode;
			delete curNode;
			curNode = tempNextNode;
			mCurSize--;
		}
	}

	/// <summary>
	/// Appends a new node with the given command and undo command to the end of the stack.
	/// If no undo command is given, the new command will be the only command in the stack as it cannot be undone.
	/// </summary>
	/// <param name="command">Command executed</param>
	void AppendNode(ICommand* command)
	{
		// Don't add empty commands
		if (command == nullptr)
			return;

		// If there is no undo, clear all previous commands
		if (!command->Undoable())
			Clear();

		// Make new node for action
		ActionNode* newNode = new ActionNode(mCurNode, nullptr, command);
		mCurSize++;

		// If no nodes have been added, make this the first and only node
		if (mCurNode == nullptr || mLastNode == nullptr)
		{
			mCurNode = newNode;
			mLastNode = newNode;
			mFirstNode->mNextNode = newNode;
			return;
		}

		// If an interaction happens after an undo, remove redo branch
		if (mCurNode->HasNext())
		{
			ClearAfter(mCurNode);
			mLastNode = mCurNode;
		}

		// If there is at least one node, move the last and current node pointers
		mLastNode->mNextNode = newNode;
		newNode->mPrevNode = mLastNode;
		mLastNode = newNode;
		mCurNode = newNode;
		Resize();
	}
public:
	/// <summary>
	/// Returns the number of currently stored actions
	/// </summary>
	/// <returns></returns>
	unsigned int Size()
	{
		return mCurSize;
	}

	/// <summary>
	/// Returns the maximum number of undoes stored
	/// </summary>
	/// <returns></returns>
	unsigned int MaxSize()
	{
		return mMaxSize;
	}

	/// <summary>
	/// Sets the maximum number of undoes
	/// </summary>
	/// <param name="maxSize">New number of undoes allowed</param>
	void SetMaxSize(unsigned int maxSize)
	{
		mMaxSize = maxSize;
		Resize();
	}

	/// <summary>
	/// Attempts to undo the most recent command. Does nothing if the most recent command cannot be undone.
	/// </summary>
	void Undo()
	{
		// If there are no previous commands, do nothing
		if (mCurNode == nullptr || mCurNode == mFirstNode || !mCurNode->HasPrev())
			return;

		// Undo the current command
		mCurNode->mCommand->Undo();
		mCurNode = mCurNode->mPrevNode;
	}

	/// <summary>
	/// Attempts to redo the most recently undone command. Does nothing if used without undo.
	/// </summary>
	void Redo()
	{
		// If there are no previous commands, do nothing
		if (mCurNode == nullptr || mCurNode == mLastNode || !mCurNode->HasNext())
			return;

		// Redo the current command
		mCurNode->mCommand->Execute();
		mCurNode = mCurNode->mNextNode;
	}

	/// <summary>
	/// Executes the given command, emplacing it as the most recent item in the stack.
	/// </summary>
	/// <param name="command">Command to execute</param>
	void Execute(ICommand* command)
	{
		// Don't execute empty command
		if (command == nullptr)
			return;

		// Execute command and add it to undo stack
		command->Execute();
		AppendNode(command);
	}

	/// <summary>
	/// Clears the action stack
	/// </summary>
	void Clear()
	{
		if (mFirstNode != nullptr)
		{
			ClearAfter(mFirstNode);
		}
		mCurNode = nullptr;
		mLastNode = nullptr;
		mCurSize = 0;
	}

	/// <summary>
	/// Constructs a new action stack with no actions
	/// </summary>
	/// <param name="maxUndo">The max number of undoes before old ones start being removed</param>
	explicit ActionStack(unsigned int maxUndo = 50)
	{
		// Create first node that always points to the actual first node
		mFirstNode = new ActionNode(nullptr, nullptr, nullptr);
		mLastNode = nullptr;
		mCurNode = nullptr;
		mCurSize = 0;
		mMaxSize = maxUndo;
	}

	/// <summary>
	/// Destructs the whole action stack
	/// </summary>
	~ActionStack()
	{
		Clear();
		delete mFirstNode;
	}
};