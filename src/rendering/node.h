#pragma once
#include "glIncludes.h"
#include "rendering/mesh.h"
#include "interfaces/iWritable.h"
#include <vector>

class Node : public IEntity, public IWritable
{
protected:
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
	std::vector<Mesh*> mMeshes;

	/// <summary>
	/// Generates buffers. Nodes have no buffers.
	/// </summary>
	void GenBuffers() override {}
public:
	/// <summary>
	/// Recursively reads all nodes from the given file
	/// </summary>
	/// <param name="file">File to read</param>
	/// <param name="parent">Parent node</param>
	/// <returns>Node that was stored in the file</returns>
	static Node* ReadRecurse(std::ifstream& file, Node* parent);

	/// <summary>
	/// Recursively writes all nodes into the given file
	/// </summary>
	/// <param name="file">File to write to</param>
	/// <param name="node">Node to write</param>
	/// <param name="depth">Depth of the node to write</param>
	static void WriteRecurse(std::ofstream& file, Node* node, unsigned int depth = 0);

	/// <summary>
	/// Reads child nodes for this node from the file
	/// </summary>
	/// <param name="file">File to read from</param>
	void Read(std::ifstream& file) override;

	/// <summary>
	/// Writes this node to the file
	/// </summary>
	/// <param name="file">File to output to</param>
	void Write(std::ofstream& file) override;

	/// <summary>
	/// Recursively draws the node and its children and child meshes
	/// </summary>
	/// <param name="shader">Shader to draw to</param>
	/// <param name="state">Global state of the application</param>
	/// <param name="defaultMat">Default material of the scene</param>
	/// <param name="viewProj">Projection * View Matrix of the camera</param>
	/// <param name="model">Combined model matrix of all parents</param>
	void Draw(Shader* shader, State* state, Material* defaultMat, const glm::mat4& viewProj, bool drawMats = false) override;

	/// <summary>
	/// Recursively draws the node's and children's shadows
	/// </summary>
	/// <param name="shader">Shadow shader</param>
	/// <param name="state">Global state of the application</param>
	/// <param name="model">Model matrix</param>
	void DrawShadows(Shader* shader, State* state);

	/// <summary>
	/// Recalculates all matrices of the node
	/// </summary>
	void RecalcMatrices() override;

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
	/// Returns the index of the given node or -1 if not found
	/// </summary>
	/// <param name="child">Child node to search for</param>
	/// <returns>Index of the node with the given name</returns>
	int GetNodeIndex(const std::string& child);

	/// <summary>
	/// Returns the index of the given mesh or -1 if not found
	/// </summary>
	/// <param name="mesh">Mesh to search for</param>
	/// <returns>Index of the given mesh or -1</returns>
	int GetMeshIndex(Mesh* mesh);

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
	Mesh* GetMesh(unsigned int index);

	/// <summary>
	/// Returns whether the node has the given mesh
	/// </summary>
	/// <param name="mesh">Mesh to search for</param>
	/// <returns>Whether the mesh was found or not</returns>
	bool HasMesh(Mesh* mesh) { return GetMeshIndex(mesh) > 0; }

	/// <summary>
	/// Returns whether the node has the given child
	/// </summary>
	/// <param name="child">Child node to search for</param>
	/// <returns>Whether the node was found or not</returns>
	bool HasNode(const std::string& child) { return GetNodeIndex(child) > 0; }

	/// <summary>
	/// Returns whether this node is the root node
	/// </summary>
	/// <returns>Whether this node is the root node</returns>
	bool IsRootNode() { return mParent == nullptr; }

	/// <summary>
	/// Updates the enabled status of the object and its children
	/// </summary>
	void UpdateEnabledStatus() override;

	/// <summary>
	/// Recursively searches for the node with the given name
	/// </summary>
	/// <param name="name">Node to search for</param>
	/// <returns>Node with the given name or nullptr if not found</returns>
	Node* FindNode(const std::string& name);

	/// <summary>
	/// Recursively searches for the mesh with the given name
	/// </summary>
	/// <param name="name">Name of the mesh</param>
	/// <returns>Mesh with the given name or nullptr if not found</returns>
	Mesh* FindMesh(const std::string& name);

	/// <summary>
	/// Adds a mesh for the node
	/// </summary>
	/// <param name="mesh">Mesh to add</param>
	void AddMesh(Mesh* mesh);
	
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
	bool MoveMesh(Mesh* mesh, Node* other);

	/// <summary>
	/// Moves the given child node from this node to the given node
	/// </summary>
	/// <param name="child">Name of the child node to move</param>
	/// <param name="other">Node to move child node to</param>
	/// <returns>Whether the moving was successful</returns>
	bool MoveChild(const std::string& child, Node* other);

	/// <summary>
	/// Removes the node with the given name along with its children
	/// </summary>
	/// <param name="name">Name of child node to delete</param>
	/// <returns>Whether the deletion happened or not</returns>
	bool DeleteNode(const std::string& name);

	/// <summary>
	/// Recursively deletes this node and all its children
	/// </summary>
	void Delete();

	/// <summary>
	/// Creates a scene node with the given parent and name
	/// </summary>
	/// <param name="parent">Parent node or nullptr</param>
	/// <param name="name">Name of this node</param>
	/// <param name="pos">Position of the node</param>
	/// <param name="rot">Rotation of the node</param>
	/// <param name="scale">Scale of the node</param>
	Node(Node* parent, const std::string& name, const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& rot = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f));
};