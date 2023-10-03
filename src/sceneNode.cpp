#include "sceneNode.h"

// Finds the node with the given name, recursively
SceneNode* SceneNode::FindNode(std::string name)
{
	// If this node is the node that is being looked for, return it
	if (mName == name)
		return this;
	for (unsigned int i = 0; i < (unsigned int)mChildren.size(); i++)
	{
		// Don't check null nodes
		if (mChildren[i] == nullptr)
			continue;

		// If the node is found, return it
		SceneNode* res = mChildren[i]->FindNode(name);
		if (res != nullptr)
			return res;
	}
	return nullptr;
}

// Returns the index of the given mesh or -1 if not found
int SceneNode::GetMeshIndex(unsigned int index)
{
	for (unsigned int i = 0; i < (unsigned int)mMeshes.size(); i++)
	{
		if (mMeshes[i] == index)
			return i;
	}
	return -1;
}

// Returns the index of the given node or -1 if not found
int SceneNode::GetNodeIndex(std::string child)
{
	for (unsigned int i = 0; i < (unsigned int)mChildren.size(); i++)
	{
		if (mChildren[i]->mName == child)
			return i;
	}
	return -1;
}

// Adds a mesh to the node
void SceneNode::AddMesh(unsigned int meshIndex)
{
	if (HasMesh(meshIndex))
	{
		std::cout << "NODE::WARNING Node already has the given mesh" << std::endl;
		return;
	}
	mMeshes.push_back(meshIndex);
}

// Adds a child to the node
void SceneNode::AddChild(SceneNode* child)
{
	if (HasNode(child->mName))
	{
		std::cout << "NODE::WARNING Node already has the given node as a child" << std::endl;
		return;
	}
	mChildren.push_back(child);
	child->mParent = this;
}

// Moves the given mesh from this node to the given node. Returns whether it was successful
bool SceneNode::MoveMesh(unsigned int meshIndex, SceneNode* other)
{
	// Don't move the mesh to a null node
	if (other == nullptr)
	{
		std::cout << "NODE::ERROR Cannot move mesh to null node. Mesh must have node parent." << std::endl;
		return false;
	}
	// Search for mesh index
	int index = GetMeshIndex(meshIndex);
	// Don't move a mesh that is not a child of this node
	if (index < 0)
	{
		std::cout << "NODE::ERROR Attempted to move mesh that is not a child of the node." << std::endl;
		return false;
	}
	// Move the element
	mMeshes.erase(mMeshes.begin() + index);
	other->AddMesh(meshIndex);
	return true;
}

// Moves the given child node from this node to the given node. Returns whether it was successful
bool SceneNode::MoveChild(std::string child, SceneNode* other)
{
	// Don't move the node to a null node
	if (other == nullptr)
	{
		std::cout << "NODE::ERROR Cannot move node to null node. Node must have parent, and root node can't be moved." << std::endl;
		return false;
	}
	// Find the child node
	int index = GetNodeIndex(child);
	// Don't move a node that is not a child of this node
	if (index < 0)
	{
		std::cout << "NODE::ERROR Attempted to move node that is not a child of the node." << std::endl;
		return false;
	}
	// Move the node
	mChildren.erase(mChildren.begin() + index);
	other->AddChild(mChildren[index]);
	return true;
}

// Removes the node with the given name along with its children. Returns whether the deletion happened
bool SceneNode::DeleteNode(std::string name)
{
	// Do not delete the node itself
	if (mName == name)
	{
		std::cout << "NODE::ERROR Attempted to delete root node." << std::endl;
		return false;
	}

	// Search for the node
	SceneNode* node = FindNode(name);
	if (node == nullptr)
		return false;

	// Delete the node
	node->Delete();
	return true;
}

// Recursively deletes this node and all its children
void SceneNode::Delete()
{
	for (unsigned int i = 0; i < (unsigned int)mChildren.size(); i++)
		mChildren[i]->Delete();
	delete this;
}