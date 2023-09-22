#pragma once
#include "vertex.h"
#include "material.h"

const unsigned int TRI_VERTS = 3;
enum class TriTag { UNCONFIRMED = 0, CONFIRMED_CORRECT = 1, CONFIRMED_INCORRECT = 2 };

// Define face here to prevent circular dependency
struct Face;

struct Triangle {
	glm::vec3 normal;
	glm::vec3 center;
	std::string mat;
	TriTag tag;

	int vertices[TRI_VERTS];
	int triIndex = -1;
	int shadingGroup = -1;

	Face GetFace();
	void SetMat(std::string _mat);
	glm::vec3 CalcNormal();

	Triangle(int i0, int i1, int i2, 
		glm::vec3 _norm = glm::vec3(), glm::vec3 _center = glm::vec3(), std::string _mat = "default", 
		int _sg = -1, int _triIndex = -1, TriTag _tag = TriTag::UNCONFIRMED);
};