#pragma once
#include "glIncludes.h"
#include "texture.h"
#include "vertex.h"
#include "imesh.h"
#include <vector>

class Mesh : public IMesh
{
protected:
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	std::vector<Texture> mTextures;

	void GenBuffers() override;
	glm::vec3 CalcCenter();
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
	void Draw(glm::mat4 viewProj);

	Vertex* GetVertex(unsigned int index) override;
};