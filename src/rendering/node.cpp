#include "node.h"

// Recursively reads all nodes from the given file
Node* Node::ReadRecurse(std::ifstream& file, Node* parent)
{
	// Construct empty node
	Node* node;
	if (parent != nullptr)
		node = new Node(parent);
	else
		node = this;

	// Create variables to store all read information
	bool enabled = true;

	// Read the node
	std::string line;
	while (std::getline(file, line))
	{
		// Break when an empty line is encountered
		if (line == "")
			break;

		std::vector<std::string> parse;
		ParseStringByDelim(parse, line, " ");

		// Don't parse empty lines
		if (parse.empty())
			break;

		// Parse lines
		if (parse[0] == "NODE")
		{
			node->SetName(parse[1]);
		}
		else if (parse[0] == "pos")
		{
			node->SetPos(ReadVec3FromStrings(parse, 1));
		}
		else if (parse[0] == "rot")
		{
			node->SetRot(ReadVec3FromStrings(parse, 1));
		}
		else if (parse[0] == "scale")
		{
			node->SetScale(ReadVec3FromStrings(parse, 1));
		}
		else if (parse[0] == "enabled")
		{
			node->Enable(parse[1] == "1");
		}
		else if (parse[0] == "meshes")
		{
			int numMeshes = std::stoi(parse[1]);
			for (int i = 0; i < numMeshes; i++)
			{
				std::string meshName;
				std::getline(file, meshName);
				node->AddMeshName(TrimWhitespace(meshName));
			}
		}
		else if (parse[0] == "children")
		{
			int numChildren = std::stoi(parse[1]);
			for (int i = 0; i < numChildren; i++)
				node->AddChild(ReadRecurse(file, node));
		}
	}
	return node;
}

// Recursively writes all nodes into the given file
void Node::WriteRecurse(std::ofstream& file, Node* node, unsigned int depth)
{
	file << GetPadding(depth) << "NODE " << node->mName << std::endl;
	file << GetPadding(depth) << "pos " << Vec3ToString(node->GetPos()) << std::endl;
	file << GetPadding(depth) << "rot " << Vec3ToString(node->GetRot()) << std::endl;
	file << GetPadding(depth) << "scale " << Vec3ToString(node->GetScale()) << std::endl;
	file << GetPadding(depth) << "enabled " << (int)node->IsEnabled() << std::endl;

	// Write all child meshes
	file << GetPadding(depth) << "meshes " << node->MeshCount() << std::endl;
	for (unsigned int i = 0; i < node->MeshCount(); i++)
		if (node->mMeshes[i] != nullptr)
			file << GetPadding(depth + 1) << node->mMeshes[i]->GetName() << std::endl;

	// Write all child nodes
	file << GetPadding(depth) << "children " << node->ChildCount() << std::endl;
	for (unsigned int i = 0; i < node->ChildCount(); i++)
		if (node->mChildren[i] != nullptr)
			WriteRecurse(file, node->mChildren[i], depth + 1);

	file << std::endl;
}

// Reads child nodes for this node from the file
void Node::Read(std::ifstream& file, bool clear)
{
	if (clear)
		Clear();
	// Replace this node with the new one
	ReadRecurse(file, nullptr);
}

// Writes this node to the file
void Node::Write(std::ofstream& file)
{
	WriteRecurse(file, this, 0);
}

// Returns the number of elements that will be written
unsigned int Node::WriteCount()
{
	unsigned int count = (unsigned int)mChildren.size();
	for (unsigned int i = 0; i < mChildren.size(); i++)
		count += mChildren[i]->WriteCount();
	return count;
}

// Returns the number of child nodes
unsigned int Node::ChildCount()
{
	return (unsigned int)mChildren.size();
}

// Returns the number of child meshes
unsigned int Node::MeshCount()
{
	return (unsigned int)mMeshes.size();
}

// Draws the node recursively
void Node::Draw(Shader* shader, State* state, Material* defaultMat, const glm::mat4& viewProj, bool drawMats)
{
	// Don't draw disabled nodes or their children
	if (!mIsEnabled)
		return;

	// Draw all child meshes
	for (unsigned int i = 0; i < mMeshes.size(); i++)
		if (mMeshes[i] != nullptr)
			mMeshes[i]->Draw(shader, state, defaultMat, viewProj, drawMats);

	// Draw all child nodes
	for (unsigned int i = 0; i < mChildren.size(); i++)
		if (mChildren[i] != nullptr)
			mChildren[i]->Draw(shader, state, defaultMat, viewProj, drawMats);
}

// Draws the node's shadows
void Node::DrawShadows(Shader* shader, State* state)
{
	// Don't draw disabled nodes or their children
	if (!mIsEnabled)
		return;

	// Draw all child meshes
	for (unsigned int i = 0; i < mMeshes.size(); i++)
		if (mMeshes[i] != nullptr)
			mMeshes[i]->DrawShadows(shader, state);

	// Draw all child nodes
	for (unsigned int i = 0; i < mChildren.size(); i++)
		if (mChildren[i] != nullptr)
			mChildren[i]->DrawShadows(shader, state);
}

// Recalculates all matrices of the entity
void Node::RecalcMatrices()
{
	glm::mat4 identity = glm::mat4(1.0f);

	glm::mat4 rotateX = glm::rotate(identity, glm::radians(mRot.x), glm::vec3(1, 0, 0));
	glm::mat4 rotateY = glm::rotate(identity, glm::radians(mRot.y), glm::vec3(0, 1, 0));
	glm::mat4 rotateZ = glm::rotate(identity, glm::radians(mRot.z), glm::vec3(0, 0, 1));

	glm::mat4 translate = glm::translate(identity, mPos);
	glm::mat4 rotate = rotateY * rotateZ * rotateX;
	glm::mat4 scale = glm::scale(identity, mScale);

	mSelfModelMatrix = translate * rotate * scale;
	mModelMatrix = mParentModelMatrix * mSelfModelMatrix;
	mNormalModelMatrix = glm::mat3(glm::transpose(glm::inverse(mModelMatrix)));
	mRecalcMatrices = false;

	// Recalculate child meshes
	for (unsigned int i = 0; i < mMeshes.size(); i++)
		if (mMeshes[i] != nullptr)
			mMeshes[i]->SetParentModelMatrix(mModelMatrix);

	// Recalculate child nodes
	for (unsigned int i = 0; i < mChildren.size(); i++)
		if (mChildren[i] != nullptr)
			mChildren[i]->SetParentModelMatrix(mModelMatrix);
}

// Recursively loads the unloaded child meshes of this and child nodes
void Node::LoadMeshes(MeshContainer* meshes)
{
	// Load unloaded meshes
	for (auto iter = mUnloadedMeshes.begin(); iter != mUnloadedMeshes.end(); ++iter)
		mMeshes.push_back(meshes->GetItem(*iter));

	// Clear unloaded meshes
	mUnloadedMeshes.clear();

	// Load all meshes of child nodes
	for (unsigned int i = 0; i < mChildren.size(); i++)
		if (mChildren[i] != nullptr)
			mChildren[i]->LoadMeshes(meshes);
}

// Finds the node with the given name, recursively
Node* Node::FindNode(const std::string& name)
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

// Recursively searches for the mesh with the given name
Mesh* Node::FindMesh(const std::string& name)
{
	// Search child meshes
	for (unsigned int i = 0; i < (unsigned int)mMeshes.size(); i++)
	{
		// Don't check null meshes
		if (mMeshes[i] == nullptr)
			continue;
		// If the node is found, return it
		if (mMeshes[i]->GetName() == name)
			return mMeshes[i];
	}

	// Search child nodes
	for (unsigned int i = 0; i < (unsigned int)mChildren.size(); i++)
	{
		// Don't check null nodes
		if (mChildren[i] == nullptr)
			continue;

		// If the mesh is found, return it
		Mesh* res = mChildren[i]->FindMesh(name);
		if (res != nullptr)
			return res;
	}
	return nullptr;
}

// Returns the index of the given mesh or -1 if not found
int Node::GetMeshIndex(Mesh* mesh)
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
int Node::GetNodeIndex(const std::string& child)
{
	for (unsigned int i = 0; i < (unsigned int)mChildren.size(); i++)
	{
		if (mChildren[i] != nullptr)
			if (mChildren[i]->mName == child)
				return i;
	}
	return -1;
}

// Returns the child node with the given index, or none if out of bounds
Node* Node::GetChild(unsigned int index)
{
	// Don't search out of bounds
	if (index >= mChildren.size())
		return nullptr;
	return mChildren[index];
}

// Returns the mesh with the given index, or none if out of bounds
Mesh* Node::GetMesh(unsigned int index)
{
	// Don't search out of bounds
	if (index >= mMeshes.size())
		return nullptr;
	return mMeshes[index];
}

// Updates the enabled status of the object
void Node::UpdateEnabledStatus()
{
	mIsEnabled = mIsEnabledParent && mIsEnabledSelf;

	// Update enabled status child meshes
	for (unsigned int i = 0; i < mMeshes.size(); i++)
		if (mMeshes[i] != nullptr)
			mMeshes[i]->EnableParent(mIsEnabled);

	// Update enabled status child nodes
	for (unsigned int i = 0; i < mChildren.size(); i++)
		if (mChildren[i] != nullptr)
			mChildren[i]->EnableParent(mIsEnabled);
}

// Adds the name of the mesh to the node, so that the mesh can later be obtained from a mesh container
void Node::AddMeshName(const std::string& name)
{
	if (mUnloadedMeshes.contains(name))
	{
		std::cout << "WARNING::NODE::EXISTS Node already has the given mesh" << std::endl;
		return;
	}
	mUnloadedMeshes.emplace(name);
}

// Adds a mesh to the node
void Node::AddMesh(Mesh* mesh)
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
bool Node::MoveMesh(Mesh* mesh, Node* other)
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
bool Node::MoveChild(const std::string& child, Node* other)
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
bool Node::DeleteNode(const std::string& name)
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
	node->Clear();
	return true;
}

// Recursively deletes this node's children
void Node::Clear()
{
	for (unsigned int i = 0; i < (unsigned int)mChildren.size(); i++)
	{
		if (mChildren[i] != nullptr)
		{
			mChildren[i]->Clear();
			delete mChildren[i];
		}
	}
	mChildren.clear();
	mMeshes.clear();
	mUnloadedMeshes.clear();
}

// Returns whether the node has the given mesh
bool Node::HasMesh(Mesh* mesh)
{
	return GetMeshIndex(mesh) > 0;
}

// Returns whether the node has the given child
bool Node::HasNode(const std::string& child)
{
	return GetNodeIndex(child) > 0;
}

// Returns whether this node is the root node
bool Node::IsRootNode()
{
	return mParent == nullptr;
}

// Creates a default node under the given parent
Node::Node(Node* parent)
	: mParent(parent)
{
	mName = "null";
	mPos = glm::vec3(0.0f);
	mRot = glm::vec3(0.0f);
	mScale = glm::vec3(1.0f);
	mParentModelMatrix = (parent != nullptr) ? parent->GetModelMatrix() : glm::mat4(1.0f);
}


// Creates a scene node with the given parent and name
Node::Node(Node* parent, const std::string& name, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
	: mParent(parent)
{
	mName = name;
	mPos = pos;
	mRot = rot;
	mScale = scale;
	mParentModelMatrix = (parent != nullptr) ? parent->GetModelMatrix() : glm::mat4(1.0f);
}