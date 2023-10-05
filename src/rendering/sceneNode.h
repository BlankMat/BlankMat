#pragma once
#include "glIncludes.h"
#include "iMesh.h"
#include <vector>

class SceneNode : IEntity
{
protected:
	std::string mName;
	SceneNode* mParent;
	std::vector<SceneNode*> mChildren;
	std::vector<unsigned int> mMeshes;
public:
	// Returns the number of child nodes
	unsigned int GetChildCount() { return (unsigned int)mChildren.size(); }

	// Returns the number of child meshes
	unsigned int GetMeshCount() { return (unsigned int)mMeshes.size(); }

	// Returns whether the node has the given mesh
	bool HasMesh(unsigned int index) { return GetMeshIndex(index) > 0; }

	// Returns whether the node has the given child
	bool HasNode(std::string child) { return GetNodeIndex(child) > 0; }

	// Returns the index of the given mesh or -1 if not found
	int GetMeshIndex(unsigned int index);

	// Returns the index of the given node or -1 if not found
	int GetNodeIndex(std::string child);

	// Finds the node with the given name, recursively
	SceneNode* FindNode(std::string name);

	// Adds a mesh for the node
	void AddMesh(unsigned int meshIndex);
	
	// Adds a child to the node
	void AddChild(SceneNode* child);

	// Moves the given mesh from this node to the given node. Returns whether it was successful
	bool MoveMesh(unsigned int meshIndex, SceneNode* other);

	// Moves the given child node from this node to the given node. Returns whether it was successful
	bool MoveChild(std::string child, SceneNode* other);

	// Removes the node with the given name along with its children. Returns whether the deletion happened
	bool DeleteNode(std::string name);

	// Recursively deletes this node and all its children
	void Delete();
};