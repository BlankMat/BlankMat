#pragma once
#include "glIncludes.h"
#include "iMesh.h"
#include <vector>

class Node : public IEntity
{
protected:
	/// <summary>
	/// Name of the node
	/// </summary>
	std::string mName;
	/// <summary>
	/// Parent node of this node. Nullptr if this is the root node
	/// </summary>
	Node* mParent;
	/// <summary>
	/// List of child nodes
	/// </summary>
	std::vector<Node*> mChildren;
	/// <summary>
	/// List of contained meshes
	/// </summary>
	std::vector<IMesh*> mMeshes;

	void GenBuffers() override {}
public:
	/// <summary>
	/// Creates a scene node with the given parent and name
	/// </summary>
	/// <param name="parent">Parent node or nullptr</param>
	/// <param name="name">Name of this node</param>
	Node(Node* parent, std::string name);

	/// <summary>
	/// Recursively draws the node and its children and child meshes
	/// </summary>
	/// <param name="viewProj">View*Projection Matrix of the camera</param>
	/// <param name="camera">Camera used for viewing</param>
	/// <param name="light">Light used for viewing</param>
	/// <param name="model">Combined model matrix of all parents</param>
	void Draw(glm::mat4 viewProj, Camera* camera, Light* light, glm::mat4 model = glm::mat4(1.0f)) override;

	/// <summary>
	/// Sets the shader of the node recursively
	/// </summary>
	/// <param name="shader">Shader to use</param>
	void SetShader(Shader* shader) override;

	/// <summary>
	/// Returns the number of child nodes
	/// </summary>
	/// <returns>Number of child nodes</returns>
	unsigned int GetChildCount() { return (unsigned int)mChildren.size(); }

	/// <summary>
	/// Returns the number of child meshes
	/// </summary>
	/// <returns>Number of child meshes</returns>
	unsigned int GetMeshCount() { return (unsigned int)mMeshes.size(); }

	/// <summary>
	/// Returns the index of the given mesh or -1 if not found
	/// </summary>
	/// <param name="mesh">Mesh to search for</param>
	/// <returns>Index of the given mesh or -1</returns>
	int GetMeshIndex(IMesh* mesh);

	/// <summary>
	/// Returns the index of the given node or -1 if not found
	/// </summary>
	/// <param name="child">Child node to search for</param>
	/// <returns>Index of the node with the given name</returns>
	int GetNodeIndex(std::string child);

	/// <summary>
	/// Returns the child node with the given index, or none if out of bounds
	/// </summary>
	/// <param name="index">Index of the child node to find</param>
	/// <returns>Node with given index or null</returns>
	Node* GetChild(unsigned int index);

	/// <summary>
	/// Returns the mesh with the given index, or none if out of bounds
	/// </summary>
	/// <param name="index">Index of the mesh to find</param>
	/// <returns>Mesh with given index or null</returns>
	IMesh* GetMesh(unsigned int index);

	/// <summary>
	/// Returns whether the node has the given mesh
	/// </summary>
	/// <param name="mesh">Mesh to search for</param>
	/// <returns>Whether the mesh was found or not</returns>
	bool HasMesh(IMesh* mesh) { return GetMeshIndex(mesh) > 0; }

	/// <summary>
	/// Returns whether the node has the given child
	/// </summary>
	/// <param name="child">Child node to search for</param>
	/// <returns>Whether the node was found or not</returns>
	bool HasNode(std::string child) { return GetNodeIndex(child) > 0; }

	/// <summary>
	/// Returns whether this node is the root node
	/// </summary>
	/// <returns>Whether this node is the root node</returns>
	bool IsRootNode() { return mParent == nullptr; }

	/// <summary>
	/// Recursively find the node with the given name
	/// </summary>
	/// <param name="name">Node to search for</param>
	/// <returns>Node with the given name or null if not found</returns>
	Node* FindNode(std::string name);

	/// <summary>
	/// Adds a mesh for the node
	/// </summary>
	/// <param name="mesh">Mesh to add</param>
	void AddMesh(IMesh* mesh);
	
	/// <summary>
	/// Adds a child to the node
	/// </summary>
	/// <param name="child">Child node to add</param>
	void AddChild(Node* child);

	/// <summary>
	/// Moves the given mesh from this node to the given node
	/// </summary>
	/// <param name="mesh">Mesh to move</param>
	/// <param name="other">Node to move mesh to</param>
	/// <returns>Whether the moving was successful</returns>
	bool MoveMesh(IMesh* mesh, Node* other);

	/// <summary>
	/// Moves the given child node from this node to the given node
	/// </summary>
	/// <param name="child">Name of the child node to move</param>
	/// <param name="other">Node to move child node to</param>
	/// <returns>Whether the moving was successful</returns>
	bool MoveChild(std::string child, Node* other);

	/// <summary>
	/// Removes the node with the given name along with its children
	/// </summary>
	/// <param name="name">Name of child node to delete</param>
	/// <returns>Whether the deletion happened or not</returns>
	bool DeleteNode(std::string name);

	/// <summary>
	/// Recursively deletes this node and all its children
	/// </summary>
	void Delete();
};