#pragma once
#include "glIncludes.h"
#include "texture.h"
#include "vertex.h"
#include "iMesh.h"
#include <vector>

class ModelMesh : public IMesh
{
protected:
	std::vector<Texture> mTextures;

	// Calculates the center of the mesh
	glm::vec3 CalcCenter();
public:
	// Instantiates a mesh with the given vertices, indices, and textures
	ModelMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
	// Draws the mesh
	void Draw(glm::mat4 viewProj) override;
	// Sets the shader of the mesh
	void SetShader(Shader* shader) override { mShader = shader; }

	// Cleans up the mesh
	~ModelMesh();
};