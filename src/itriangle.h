#pragma once
#include "triangle.h"
#include "indexStructs.h"
#include <unordered_map>

struct ITriangle {
	glm::vec3 normal;
	glm::vec3 center;
	std::string mat;
	TriTag tag;
	IndVertex vertices[TRI_VERTS];

	int triIndex = -1;
	int meshIndex = -1;
	int shadingGroup = -1;

	void SetMat(std::string _mat);
	void CalcNormal();
	void Reorient(std::vector<ITriangle>& tris);

	ITriangle();
	ITriangle(IndVertex i0, IndVertex i1, IndVertex i2, std::string _mat = "default", int _sg = -1);
	ITriangle(Triangle _tri, std::unordered_map<int, Vertex>& _verts, int _triIndex = -1, int _meshIndex = -1, TriTag _tag = TriTag::UNCONFIRMED);
};