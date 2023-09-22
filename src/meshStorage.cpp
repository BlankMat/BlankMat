#include "meshStorage.h"

void MeshStorage::AddMesh(std::string key, OldMesh* mat)
{
	meshes.emplace(key, mat);
}

OldMesh* MeshStorage::Get(std::string key)
{
	return meshes[key];
}

std::unordered_map<std::string, OldMesh*>& MeshStorage::GetAll()
{
	return meshes;
}

void MeshStorage::Clear()
{
	for (auto iter = meshes.begin(); iter != meshes.end(); iter++) {
		delete iter->second;
	}
	meshes.clear();
}

MeshStorage::MeshStorage()
{
	meshes = std::unordered_map<std::string, OldMesh*>();
}

MeshStorage::~MeshStorage()
{
	Clear();
}