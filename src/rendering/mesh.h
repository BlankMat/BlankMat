#pragma once
#include "iMesh.h"
#include "material.h"
#include <unordered_map>

class Mesh : public IMesh
{
protected:
	Material* mMaterial;
	std::unordered_map<std::string, IMesh*> mChildren;
public:
	// Generates a mesh with the given vertices, indices, material, and child meshes
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, Material* material);

	// Draws the mesh to the screen
	void Draw(glm::mat4 viewProj) override;

	// Sets the shader of the mesh
	void SetShader(Shader* shader) override;

	// Adds the child mesh
	IMesh* AddChild(IMesh* child);

	// Returns the number of child meshes
	unsigned int GetChildCount() { return (unsigned int)mChildren.size(); }

	// Cleans up the mesh
	~Mesh();
};