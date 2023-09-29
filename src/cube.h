#pragma once
#include "glIncludes.h"
#include "shader.h"
#include "primitive.h"
#include <vector>

class Cube : public Primitive<glm::vec3, glm::uvec3>
{
public:
	Cube(float size, Shader* shader, glm::vec3 color, float lineWidth,
		glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
		: Primitive(shader, color, lineWidth, pos, rot, scale)
	{
		float h = size * 0.5f;
		// Generate verts of cube
		mVertices.push_back(glm::vec3(-h, -h,  h));	//0
		mVertices.push_back(glm::vec3( h, -h,  h));	//1
		mVertices.push_back(glm::vec3(-h,  h,  h));	//2
		mVertices.push_back(glm::vec3( h,  h,  h));	//3
		mVertices.push_back(glm::vec3(-h, -h, -h));	//4
		mVertices.push_back(glm::vec3( h, -h, -h));	//5
		mVertices.push_back(glm::vec3(-h,  h, -h));	//6
		mVertices.push_back(glm::vec3( h,  h, -h));	//7

		// Generate indices of cube
		mIndices.push_back(glm::uvec3(7, 6, 2));	// Top
		mIndices.push_back(glm::uvec3(2, 3, 7));	// Top
		mIndices.push_back(glm::uvec3(0, 4, 5));	// Bottom
		mIndices.push_back(glm::uvec3(5, 1, 0));	// Bottom
		mIndices.push_back(glm::uvec3(0, 2, 6));	// Left
		mIndices.push_back(glm::uvec3(6, 4, 0));	// Left
		mIndices.push_back(glm::uvec3(7, 3, 1));	// Right
		mIndices.push_back(glm::uvec3(1, 5, 7));	// Right
		mIndices.push_back(glm::uvec3(3, 2, 0));	// Front
		mIndices.push_back(glm::uvec3(0, 1, 3));	// Front
		mIndices.push_back(glm::uvec3(4, 6, 7));	// Back
		mIndices.push_back(glm::uvec3(7, 5, 4));	// Back
		
		mIsWireframe = false;
		Primitive::GenBuffers();
	}
};