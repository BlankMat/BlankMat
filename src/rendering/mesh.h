#pragma once
#include "glIncludes.h"
#include "texture.h"
#include "vertex.h"
#include "material.h"
#include "iMesh.h"
#include <vector>

class Mesh : public IMesh
{
protected:
	Material* mMaterial;

	// Calculates the center of the mesh
	glm::vec3 CalcCenter();
public:
	// Instantiates a mesh with the given vertices, indices, and textures
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, Material* material);
	// Draws the mesh
	void Draw(glm::mat4 viewProj, glm::mat4 model = glm::mat4(1.0f)) override;
	// Sets the shader of the mesh
	void SetShader(Shader* shader) override { mShader = shader; }

	// Cleans up the mesh
	~Mesh();
};