#pragma once
#include "glIncludes.h"
#include "vertex.h"
#include <vector>
#include <string>

// An indexed vertex. Used to facilitate indexed triangle structures
struct IndVertex
{
	Vertex ver;
	int id;

	IndVertex(int _id = -1, Vertex _ver = Vertex()) : id(_id), ver(_ver) {}
};

// Data needed for constructing a face
struct FaceData
{
	std::vector<IndVertex> vertexInfo;
	std::string matName;
	int shadingGroup;
	int id;

	FaceData() : matName(""), shadingGroup(-1), id(-1) {};
	FaceData(std::vector<IndVertex>& _verts, std::string _mat = "", int _sg = -1, int _id = -1)
		: matName(_mat), vertexInfo(_verts), shadingGroup(_sg), id(_id) {}
};

// A string and an int. A strint, if you will
struct Strint
{
	std::string str;
	int id;

	Strint(int _id = -1, std::string _str = "") : id(_id), str(_str) {}
};

// An indexed vector. Used for storing data needed to build a vertex
struct IndVec3
{
	glm::vec3 pos;
	int id;

	IndVec3(int _id = -1, glm::vec3 _pos = glm::vec3()) : id(_id), pos(_pos) {}
};

int FindVertIndex(std::vector<IndVertex>& verts, int id);
int FindVertIndex(std::vector<IndVec3>& verts, int id);
int FindFaceIndex(std::vector<FaceData>& faces, int id);

IndVertex FindVert(std::vector<IndVertex>& verts, int id);
IndVec3 FindVert(std::vector<IndVec3>& verts, int id);
FaceData FindFace(std::vector<FaceData>& faces, int id);