#pragma once
#include "rendering/mesh.h"

class VCube : public Mesh
{
public:
	VCube(std::string name, float size, Shader* shader, Material* material, Material* defaultMat, State* state,
		glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
		: Mesh(name, material, defaultMat, state)
	{
		float h = size * 0.5f;

		// Generate UV coords
		glm::vec2 uv1(0.0f, 1.0f);
		glm::vec2 uv2(0.0f, 0.0f);
		glm::vec2 uv3(1.0f, 0.0f);
		glm::vec2 uv4(1.0f, 1.0f);

		// Normal
		glm::vec3 nm(0.0f, 0.0f, 1.0f);

		// Generate verts of cube
		mVertices.push_back(Vertex(glm::vec3(-h, -h,  h), glm::vec3(), glm::vec2(), glm::vec3()));	//0
		mVertices.push_back(Vertex(glm::vec3( h, -h,  h), glm::vec3(), glm::vec2(), glm::vec3()));	//1
		mVertices.push_back(Vertex(glm::vec3(-h,  h,  h), glm::vec3(), glm::vec2(), glm::vec3()));	//2
		mVertices.push_back(Vertex(glm::vec3( h,  h,  h), glm::vec3(), glm::vec2(), glm::vec3()));	//3
		mVertices.push_back(Vertex(glm::vec3(-h, -h, -h), glm::vec3(), glm::vec2(), glm::vec3()));	//4
		mVertices.push_back(Vertex(glm::vec3( h, -h, -h), glm::vec3(), glm::vec2(), glm::vec3()));	//5
		mVertices.push_back(Vertex(glm::vec3(-h,  h, -h), glm::vec3(), glm::vec2(), glm::vec3()));	//6
		mVertices.push_back(Vertex(glm::vec3( h,  h, -h), glm::vec3(), glm::vec2(), glm::vec3()));	//7

		// Generate indices of cube
		std::vector<unsigned int> tempInd = {
			2, 3, 7,	// Top
			7, 6, 2,	// Top
			0, 4, 5,	// Bottom
			5, 1, 0,	// Bottom
			0, 2, 6,	// Left
			6, 4, 0,	// Left
			7, 3, 1,	// Right
			1, 5, 7,	// Right
			3, 2, 0,	// Front
			0, 1, 3,	// Front
			4, 6, 7,	// Back
			7, 5, 4		// Back
		};
		mIndices.insert(mIndices.begin(), tempInd.begin(), tempInd.end());

		mPos = pos;
		mRot = rot;
		mScale = scale;

		SetShader(shader);
		Mesh::GenBuffers();
	}
};