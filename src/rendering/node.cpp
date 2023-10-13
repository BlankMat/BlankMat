#include "node.h"

// Creates a scene node with the given parent and name
Node::Node(Node* parent, std::string name)
{
	mParent = parent;
	mName = name;
}

// Draws the node recursively
void Node::Draw(glm::mat4 viewProj, Camera* camera, Light* light, glm::mat4 model)
{
	// Calculate the MVP of this stage
	glm::mat4 newModel = GetModelMatrix() * model;

	// Draw all child meshes
	for (unsigned int i = 0; i < mMeshes.size(); i++)
		if (mMeshes[i] != nullptr)
			mMeshes[i]->Draw(viewProj, camera, light, newModel);

	// Draw all child nodes
	for (unsigned int i = 0; i < mChildren.size(); i++)
		if (mChildren[i] != nullptr)
			mChildren[i]->Draw(viewProj, camera, light, newModel);
}

// Sets the shader of the node recursively
void Node::SetShader(Shader* shader)
{
	// Draw all child meshes
	for (unsigned int i = 0; i < mMeshes.size(); i++)
		if (mMeshes[i] != nullptr)
			mMeshes[i]->SetShader(shader);

	// Draw all child nodes
	for (unsigned int i = 0; i < mChildren.size(); i++)
		if (mChildren[i] != nullptr)
			mChildren[i]->SetShader(shader);
}

// Finds the node with the given name, recursively
Node* Node::FindNode(std::string name)
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
		Node* res = mChildren[i]->FindNode(name);
		if (res != nullptr)
			return res;
	}
	return nullptr;
}

// Returns the index of the given mesh or -1 if not found
int Node::GetMeshIndex(IMesh* mesh)
{
	for (unsigned int i = 0; i < (unsigned int)mMeshes.size(); i++)
	{
		if (mMeshes[i] != nullptr)
			if (mMeshes[i]->GetName() == mesh->GetName())
				return i;
	}
	return -1;
}

// Returns the index of the given node or -1 if not found
int Node::GetNodeIndex(std::string child)
{
	for (unsigned int i = 0; i < (unsigned int)mChildren.size(); i++)
	{
		if (mChildren[i] != nullptr)
			if (mChildren[i]->mName == child)
				return i;
	}
	return -1;
}

// Adds a mesh to the node
void Node::AddMesh(IMesh* mesh)
{
	if (HasMesh(mesh))
	{
		std::cout << "WARNING::NODE::EXISTS Node already has the given mesh" << std::endl;
		return;
	}
	mMeshes.push_back(mesh);
}

// Adds a child to the node
void Node::AddChild(Node* child)
{
	if (HasNode(child->mName))
	{
		std::cout << "WARNING::NODE::EXISTS Node already has the given node as a child" << std::endl;
		return;
	}
	mChildren.push_back(child);
	child->mParent = this;
}

// Moves the given mesh from this node to the given node. Returns whether it was successful
bool Node::MoveMesh(IMesh* mesh, Node* other)
{
	// Don't move null mesh
	if (mesh == nullptr)
	{
		std::cout << "ERROR::NODE::NULL Cannot move null mesh." << std::endl;
		return false;
	}

	// Don't move the mesh to a null node
	if (other == nullptr)
	{
		std::cout << "ERROR::NODE::NULL Cannot move mesh to null node. Mesh must have node parent." << std::endl;
		return false;
	}
	// Search for mesh index
	int index = GetMeshIndex(mesh);
	// Don't move a mesh that is not a child of this node
	if (index < 0)
	{
		std::cout << "ERROR::NODE::INVALID Attempted to move mesh that is not a child of the node." << std::endl;
		return false;
	}
	// Move the element
	mMeshes.erase(mMeshes.begin() + index);
	other->AddMesh(mesh);
	return true;
}

// Moves the given child node from this node to the given node. Returns whether it was successful
bool Node::MoveChild(std::string child, Node* other)
{
	// Don't move the node to a null node
	if (other == nullptr)
	{
		std::cout << "ERROR::NODE::NULL Cannot move node to null node. Node must have parent, and root node can't be moved." << std::endl;
		return false;
	}
	// Find the child node
	int index = GetNodeIndex(child);
	// Don't move a node that is not a child of this node
	if (index < 0)
	{
		std::cout << "ERROR::NODE::INVALID Attempted to move node that is not a child of the node." << std::endl;
		return false;
	}
	// Move the node
	mChildren.erase(mChildren.begin() + index);
	other->AddChild(mChildren[index]);
	return true;
}

// Removes the node with the given name along with its children. Returns whether the deletion happened
bool Node::DeleteNode(std::string name)
{
	// Do not delete the node itself
	if (mName == name)
	{
		std::cout << "ERROR::NODE::ROOT Attempted to delete root node." << std::endl;
		return false;
	}

	// Search for the node
	Node* node = FindNode(name);
	if (node == nullptr)
		return false;

	// Delete the node
	node->Delete();
	return true;
}

// Recursively deletes this node and all its children
void Node::Delete()
{
	for (unsigned int i = 0; i < (unsigned int)mChildren.size(); i++)
		mChildren[i]->Delete();
	delete this;
}