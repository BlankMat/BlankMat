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
	glm::vec3 CalcCenter();
public:
	// Instantiates an empty mesh
	Mesh(std::string name, Material* material);
	// Instantiates a mesh with the given vertices, indices, and textures
	Mesh(std::string name, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, Material* material);
	// Draws the mesh
	void Draw(Shader* shader, State* state, Material* defaultMat, glm::mat4 viewProj, glm::mat4 model = glm::mat4(1.0f)) override;
	// Draws the mesh's shadows
	void DrawShadows(Shader* shader, State* state, glm::mat4 model = glm::mat4(1.0f)) override;

	// Cleans up the mesh
	~Mesh();
};