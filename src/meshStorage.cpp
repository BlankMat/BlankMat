#include "meshStorage.h"

void MeshStorage::AddMesh(std::string key, Mesh* mat)
{
	meshes.emplace(key, mat);
}

Mesh* MeshStorage::Get(std::string key)
{
	return meshes[key];
}

std::unordered_map<std::string, Mesh*>& MeshStorage::GetAll()
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
	meshes = std::unordered_map<std::string, Mesh*>();
}

MeshStorage::~MeshStorage()
{
	Clear();
}