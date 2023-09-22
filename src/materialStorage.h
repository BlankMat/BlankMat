#pragma once
#include "glIncludes.h"
#include "material.h"
#include <unordered_map>
#include <string>

class MaterialStorage
{
private:
	std::unordered_map<std::string, Material*> materials;
public:
	void AddMat(std::string key, Material mat);
	void AddMat(std::string key, Material* mat);
	Material* Get(std::string key);
	std::unordered_map<std::string, Material*>& GetAll();
	void Clear();
	bool IsNonDefault();

	MaterialStorage();
	~MaterialStorage();
};