#pragma once
#include "glIncludes.h"
#include "texture.h"
#include "vertex.h"
#include "material.h"
#include "interfaces/iMesh.h"
#include <vector>

class Mesh : public IMesh
{
protected:
	// Calculates the center of the mesh
	const glm::vec3 CalcCenter();
public:
	// Instantiates an empty mesh
	Mesh(const std::string& name, Material* material)
		: IMesh(name, material) {}

	// Instantiates a mesh with the given vertices, indices, and textures
	Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, Material* material);
	
	// Cleans up the mesh
	~Mesh();
};