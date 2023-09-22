#pragma once
#include "glIncludes.h"
#include "mesh.h"
#include <unordered_map>
#include <string>

class MeshStorage
{
private:
	std::unordered_map<std::string, Mesh*> meshes;
public:
	void AddMesh(std::string key, Mesh* mat);
	Mesh* Get(std::string key);
	std::unordered_map<std::string, Mesh*>& GetAll();
	void Clear();

	MeshStorage();
	~MeshStorage();
};