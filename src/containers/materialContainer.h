#pragma once
#include "rendering/material.h"
#include "interfaces/iContainer.h"
#include "containers/textureContainer.h"

class MaterialContainer : public IContainer<Material>
{
protected:
	/// <summary>
	/// Renames the given item to the given name
	/// </summary>
	/// <param name="item">Item to rename</param>
	void RenameItem(Material* item, const std::string& name) override
	{
		item->SetName(name);
	}

	/// <summary>
	/// Returns whether the item should be skipped or not
	/// </summary>
	/// <param name="item">Item to consider</param>
	/// <returns>Whether the item should be skipped</returns>
	bool SkipItem(Material* item) override
	{
		return item->IsInternal();
	} 

	/// <summary>
	/// Reads the next item from the input file stream
	/// </summary>
	/// <param name="file">File to read</param>
	/// <returns>Newly constructed item from file</returns>
	const std::pair<std::string, Material*> ReadItem(std::ifstream& file) override
	{
		// Set up variables to store read information
		std::string name = "default";
		std::string scope = "";
		glm::vec3 kd = glm::vec3(1.0f);
		glm::vec3 ka = glm::vec3(0.1f);
		glm::vec3 ks = glm::vec3(1.0f);
		glm::vec3 ke = glm::vec3(0.0f);
		float ns = 8.0f;
		float ni = 0.0f;
		float d = 1.0f;
		int illum = 2;
		std::string map_kd = "";
		std::string map_ka = "";
		std::string map_ks = "";
		std::string map_bump = "";
		std::string map_ns = "";
		std::string map_d = "";

		// Read the node
		std::string line;
		while (std::getline(file, line))
		{
			// Break when an empty line is encountered
			if (line == "")
				break;

			std::vector<std::string> parse;
			ParseStringByDelim(parse, line, " ");

			// Don't parse empty lines
			if (parse.empty())
				break;

			// Parse lines
			if (parse[0] == "MATERIAL" && parse.size() > 1)
				name = parse[1];
			else if (parse[0] == "kd" && parse.size() > 3)
				kd = ReadVec3FromStrings(parse, 1);
			else if (parse[0] == "ka" && parse.size() > 3)
				ka = ReadVec3FromStrings(parse, 1);
			else if (parse[0] == "ks" && parse.size() > 3)
				ks = ReadVec3FromStrings(parse, 1);
			else if (parse[0] == "ke" && parse.size() > 3)
				ke = ReadVec3FromStrings(parse, 1);
			else if (parse[0] == "ns" && parse.size() > 1)
				ns = std::stof(parse[1]);
			else if (parse[0] == "ni" && parse.size() > 1)
				ni = std::stof(parse[1]);
			else if (parse[0] == "d" && parse.size() > 1)
				d = std::stof(parse[1]);
			else if (parse[0] == "illum" && parse.size() > 1)
				illum = std::stoi(parse[1]);
			else if (parse[0] == "map_kd" && parse.size() > 1)
				map_kd = parse[1];
			else if (parse[0] == "map_ka" && parse.size() > 1)
				map_ka = parse[1];
			else if (parse[0] == "map_ks" && parse.size() > 1)
				map_ks = parse[1];
			else if (parse[0] == "map_bump" && parse.size() > 1)
				map_bump = parse[1];
			else if (parse[0] == "map_ns" && parse.size() > 1)
				map_ns = parse[1];
			else if (parse[0] == "map_d" && parse.size() > 1)
				map_d = parse[1];
		}

		return std::pair<std::string, Material*>(name, new Material(UnscopeName(name), UnscopeScope(name), map_kd, map_ka, map_ks, map_bump, map_ns, map_d, ka, kd, ks, ns, ni, d, ke, illum));
	}

	/// <summary>
	/// Writes the given item into the output file stream
	/// </summary>
	/// <param name="key">Key of item</param>
	/// <param name="item">Item to write</param>
	/// <param name="file">File to write to</param>
	void WriteItem(const std::string& key, Material* item, std::ofstream& file) override
	{
		file << "MATERIAL " << key << std::endl;
		file << "kd " << Vec3ToString(item->mKD) << std::endl;
		file << "ka " << Vec3ToString(item->mKA) << std::endl;
		file << "ks " << Vec3ToString(item->mKS) << std::endl;
		file << "ke " << Vec3ToString(item->mKE) << std::endl;
		file << "ns " << item->mNS << std::endl;
		file << "ni " << item->mNI << std::endl;
		file << "d " << item->mD << std::endl;
		file << "illum " << item->mIllum << std::endl;

		// Write textures only if they are used
		if (item->mMapKD != nullptr)
			file << "map_kd " << item->mMapKD->GetScopedName() << std::endl;
		if (item->mMapKA != nullptr)
			file << "map_ka " << item->mMapKA->GetScopedName() << std::endl;
		if (item->mMapKS != nullptr)
			file << "map_ks " << item->mMapKS->GetScopedName() << std::endl;
		if (item->mMapBump != nullptr)
			file << "map_bump " << item->mMapBump->GetScopedName() << std::endl;
		if (item->mMapNS != nullptr)
			file << "map_ns " << item->mMapNS->GetScopedName() << std::endl;
		if (item->mMapD != nullptr)
			file << "map_d " << item->mMapD->GetScopedName() << std::endl;
	}

public:
	/// <summary>
	/// Adds the material to the container
	/// </summary>
	/// <param name="item">The item to store</param>
	/// <param name="replace">Whether to replace the existing item if this is a duplicate</param>
	/// <returns>The element stored in the container</returns>
	Material* AddMaterial(Material* item, bool replace = false)
	{
		return Add(item->GetScopedName(), item, replace);
	}

	/// <summary>
	/// Returns the default material
	/// </summary>
	/// <returns>Default material</returns>
	Material* GetDefault()
	{
		return GetItem("default");
	}

	/// <summary>
	/// Loads the textures of materials in this container
	/// </summary>
	/// <param name="textures">Texture list of the scene</param>
	void LoadTextures(TextureContainer* textures)
	{
		for (auto iter = mData.begin(); iter != mData.end(); ++iter)
			iter->second->LoadMaterialTextures(textures);
	}

	/// <summary>
	/// Initializes a default material container with the default material
	/// </summary>
	/// <param name="textures">Texture container with default textures</param>
	explicit MaterialContainer(TextureContainer* textures)
	{
		mCurSelectedItem = Add("default", new Material(textures), true);
	}
};