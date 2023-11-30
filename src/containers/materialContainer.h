#pragma once
#include "rendering/material.h"
#include "interfaces/iContainer.h"
#include "containers/textureContainer.h"

class MaterialContainer : public IContainer<Material>
{
protected:
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
			if (parse[0] == "MATERIAL")
				name = parse[1];
			else if (parse[0] == "kd")
				kd = ReadVec3FromStrings(parse, 1);
			else if (parse[0] == "ka")
				ka = ReadVec3FromStrings(parse, 1);
			else if (parse[0] == "ks")
				ks = ReadVec3FromStrings(parse, 1);
			else if (parse[0] == "ke")
				ke = ReadVec3FromStrings(parse, 1);
			else if (parse[0] == "ns")
				ns = std::stof(parse[1]);
			else if (parse[0] == "ni")
				ni = std::stof(parse[1]);
			else if (parse[0] == "d")
				d = std::stof(parse[1]);
			else if (parse[0] == "illum")
				illum = std::stoi(parse[1]);
			else if (parse[0] == "map_kd")
				map_kd = parse[1];
			else if (parse[0] == "map_ka")
				map_ka = parse[1];
			else if (parse[0] == "map_ks")
				map_ks = parse[1];
			else if (parse[0] == "map_bump")
				map_bump = parse[1];
			else if (parse[0] == "map_ns")
				map_ns = parse[1];
			else if (parse[0] == "map_d")
				map_d = parse[1];
		}

		return std::pair<std::string, Material*>(name, new Material(name, map_kd, map_ka, map_ks, map_bump, map_ns, map_d, ka, kd, ks, ns, ni, d, ke, illum));
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
		file << "kd " << Vec3ToString(item->kd) << std::endl;
		file << "ka " << Vec3ToString(item->ka) << std::endl;
		file << "ks " << Vec3ToString(item->ks) << std::endl;
		file << "ke " << Vec3ToString(item->ke) << std::endl;
		file << "ns " << item->ns << std::endl;
		file << "ni " << item->ni << std::endl;
		file << "d " << item->d << std::endl;
		file << "illum " << item->illum << std::endl;

		// Write textures only if they are used
		if (item->map_kd != nullptr)
			file << "map_kd " << item->map_kd->name << std::endl;
		if (item->map_ka != nullptr)
			file << "map_ka " << item->map_ka->name << std::endl;
		if (item->map_ks != nullptr)
			file << "map_ks " << item->map_ks->name << std::endl;
		if (item->map_bump != nullptr)
			file << "map_bump " << item->map_bump->name << std::endl;
		if (item->map_ns != nullptr)
			file << "map_ns " << item->map_ns->name << std::endl;
		if (item->map_d != nullptr)
			file << "map_d " << item->map_d->name << std::endl;
	}

public:
	/// <summary>
	/// Adds the material to the container, or if it already exists, updates the item to the given one
	/// </summary>
	/// <param name="item">The item to store</param>
	/// <returns>The element stored in the container</returns>
	Material* AddMaterial(Material* item)
	{
		return Add(item->name, item);
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
		Add("default", new Material(textures));
	}
};