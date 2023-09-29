#pragma once
#include "glIncludes.h"
#include "oldmesh.h"
#include <unordered_map>
#include <string>

class MeshStorage
{
private:
	std::unordered_map<std::string, OldMesh*> meshes;
public:
	void AddMesh(std::string key, OldMesh* mat);
	OldMesh* Get(std::string key);
	std::unordered_map<std::string, OldMesh*>& GetAll();
	void Clear();

	MeshStorage();
	~MeshStorage();
};