#pragma once
#include "interfaces/iPrimitive.h"

class PWireCube : public IPrimitive<glm::vec3, glm::uvec4>
{
public:
	PWireCube(std::string name, float size, Material* material, float lineWidth, bool drawOver = true,
		glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
		: IPrimitive(name, material, lineWidth, drawOver, pos, rot, scale)
	{
		float h = size * 0.5f;
		// Generate verts of cube
		mVertices.push_back(glm::vec3(-h, -h, h));	//0
		mVertices.push_back(glm::vec3(h, -h, h));	//1
		mVertices.push_back(glm::vec3(-h, h, h));	//2
		mVertices.push_back(glm::vec3(h, h, h));	//3
		mVertices.push_back(glm::vec3(-h, -h, -h));	//4
		mVertices.push_back(glm::vec3(h, -h, -h));	//5
		mVertices.push_back(glm::vec3(-h, h, -h));	//6
		mVertices.push_back(glm::vec3(h, h, -h));	//7

		// Generate indices of cube
		mIndices.push_back(glm::uvec4(0, 4, 4, 5));	// Bottom
		mIndices.push_back(glm::uvec4(5, 1, 1, 0));	// Bottom
		mIndices.push_back(glm::uvec4(7, 6, 6, 2));	// Top
		mIndices.push_back(glm::uvec4(2, 3, 3, 7));	// Top
		mIndices.push_back(glm::uvec4(1, 3, 5, 7));	// Sides
		mIndices.push_back(glm::uvec4(0, 2, 4, 6));	// Sides

		mIsWireframe = true;
		IPrimitive::GenBuffers();
	}
};