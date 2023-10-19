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
	// Calculates the center of the mesh
	glm::vec3 CalcCenter();
public:
	// Instantiates an empty mesh
	Mesh(std::string name, Material* material, Material* defaultMat, State* state);
	// Instantiates a mesh with the given vertices, indices, and textures
	Mesh(std::string name, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, Material* material);
	// Draws the mesh
	void Draw(glm::mat4 viewProj, Camera* camera, Light* light, glm::mat4 model = glm::mat4(1.0f)) override;
	// Sets the shader of the mesh
	void SetShader(Shader* shader) override { mShader = shader; }
	// Sets the default material for the mesh
	void SetDefaultMat(Material* defaultMat) { mDefaultMat = defaultMat; }
	// Gives the mesh a reference to the global state
	void SetState(State* state) { mState = state; }

	// Cleans up the mesh
	~Mesh();
};