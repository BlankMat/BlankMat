#pragma once
#include "glIncludes.h"
#include "iMesh.h"
#include <vector>

class Node : public IEntity
{
protected:
	std::string mName;
	Node* mParent;
	std::vector<Node*> mChildren;
	std::vector<IMesh*> mMeshes;

	void GenBuffers() override {}
public:
	// Creates a scene node with the given parent and name
	Node(Node* parent, std::string name);

	// Draws the node recursively
	void Draw(glm::mat4 viewProj, glm::mat4 model = glm::mat4(1.0f)) override;

	// Sets the shader of the node recursively
	void SetShader(Shader* shader) override;

	// Returns the number of child nodes
	unsigned int GetChildCount() { return (unsigned int)mChildren.size(); }

	// Returns the number of child meshes
	unsigned int GetMeshCount() { return (unsigned int)mMeshes.size(); }

	// Returns whether the node has the given mesh
	bool HasMesh(IMesh* mesh) { return GetMeshIndex(mesh) > 0; }

	// Returns whether the node has the given child
	bool HasNode(std::string child) { return GetNodeIndex(child) > 0; }

	// Returns whether the node is the root node
	bool IsRootNode() { return mParent == nullptr; }

	// Returns the index of the given mesh or -1 if not found
	int GetMeshIndex(IMesh* mesh);

	// Returns the index of the given node or -1 if not found
	int GetNodeIndex(std::string child);

	// Finds the node with the given name, recursively
	Node* FindNode(std::string name);

	// Adds a mesh for the node
	void AddMesh(IMesh* mesh);
	
	// Adds a child to the node
	void AddChild(Node* child);

	// Moves the given mesh from this node to the given node. Returns whether it was successful
	bool MoveMesh(IMesh* mesh, Node* other);

	// Moves the given child node from this node to the given node. Returns whether it was successful
	bool MoveChild(std::string child, Node* other);

	// Removes the node with the given name along with its children. Returns whether the deletion happened
	bool DeleteNode(std::string name);

	// Recursively deletes this node and all its children
	void Delete();
};