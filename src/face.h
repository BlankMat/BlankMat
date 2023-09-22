#pragma once
#include "vertex.h"
#include "material.h"
#include "indexStructs.h"
#include "math.h"
#include <vector>
#include <unordered_map>

// Define triangle here to prevent circular dependency
struct Triangle;

struct Face {
	glm::vec3 normal;
	glm::vec3 center;
	std::string mat;
	std::vector<int> vertices;
	int shadingGroup = -1;

	void GetTri(std::vector<Triangle>& tris, std::unordered_map<int, Vertex>& tempVerts, int _index);
	int GetNumVerts();
	int GetVertex(int _index);
	void SetVertex(int _vertex, int _index);
	void SetVertices(std::vector<int>& _verts);

	void SetMat(std::string _mat);
	void SetNormal(glm::vec3 _norm);
	void SetCenter(glm::vec3 _center);
	void CalcCenter(std::unordered_map<int, Vertex>& _verts);
	void CalcNormal(std::unordered_map<int, Vertex>& _verts);
	Triangle GetClockwiseTri(IndVertex a, IndVertex b, IndVertex c, std::string _mat, int _sg, int _index);
	void Clear();

	Face(std::string _mat = "default", int _sg = -1);
	Face(int i0, int i1, int i2, std::string _mat = "default", int _sg = -1);
	Face(int i0, int i1, int i2, int i3, std::string _mat = "default", int _sg = -1);
	Face(std::vector<int>& _verts, std::string _mat = "default", int _sg = -1);
	Face(std::vector<IndVertex>& _verts, std::string _mat = "default", int _sg = -1);
	~Face();
};