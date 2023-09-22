#include "materialStorage.h"

void MaterialStorage::AddMat(std::string key, Material mat)
{
	Material* tempMat = new Material(mat);
	materials.emplace(key, tempMat);
}

void MaterialStorage::AddMat(std::string key, Material* mat)
{
	materials.emplace(key, mat);
}

Material* MaterialStorage::Get(std::string key)
{
	return materials[key];
}

std::unordered_map<std::string, Material*>& MaterialStorage::GetAll()
{
	return materials;
}

void MaterialStorage::Clear()
{
	for (auto iter = materials.begin(); iter != materials.end(); iter++) {
		delete iter->second;
	}
	materials.clear();
}

bool MaterialStorage::IsNonDefault()
{
	for (auto iter = materials.begin(); iter != materials.end(); iter++) {
		// If any non-default elements are found, return
		if (iter->first != "default")
			return true;
	}
	return false;
}

MaterialStorage::MaterialStorage()
{
	materials = std::unordered_map<std::string, Material*>();
}

MaterialStorage::~MaterialStorage()
{
	Clear();
}